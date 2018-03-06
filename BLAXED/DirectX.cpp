#include "Global.h"
#include "DirectX.h"
#include "Chams.h"

#include "Protection.h"

#include "licence.h"
#include "Render.h"
#include "Render.cpp" // fix for the linker bug...

Reset_t Reset = nullptr;
Present_t Present = nullptr;
EndScene_t EndScene = nullptr;

VMT *hookDirectX = nullptr;

bool reseting = false;

void DirectX::ApplyHook(LPDIRECT3DDEVICE9 pDevice)
{
	hookDirectX = new VMT(pDevice);

	hookDirectX->HookVM(&PresentHook, 17);
	Present = hookDirectX->GetOriginalMethod<Present_t>(17);

	hookDirectX->HookVM(&ResetHook, 16);
	Reset = hookDirectX->GetOriginalMethod<Reset_t>(16);
	hookDirectX->HookVM(&EndSceneHook, 42);
	EndScene = hookDirectX->GetOriginalMethod<EndScene_t>(42);

	hookDirectX->ApplyVMT();
}

void DirectX::ReleaseHook()
{
	hookDirectX->ReleaseVMT();
	
	delete hookDirectX;
	hookDirectX = NULL;
}


HRESULT WINAPI PresentHook(LPDIRECT3DDEVICE9 pDevice, const RECT *pSourceRect, const RECT *pDestRect, HWND hDestWindowOverride, const RGNDATA *pDirtyRegion)
{
	if (!render || G::exit || reseting) return Present(pDevice, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);


	// backup render states
	static const D3DRENDERSTATETYPE backupStates[] = { D3DRS_COLORWRITEENABLE, D3DRS_ALPHABLENDENABLE, D3DRS_SRCBLEND, D3DRS_DESTBLEND, D3DRS_BLENDOP, D3DRS_FOGENABLE };
	static const int size = sizeof(backupStates) / sizeof(DWORD);

	DWORD oldStates[size] = { 0 };

	for (int i = 0; i < size; i++)
		pDevice->GetRenderState(backupStates[i], &oldStates[i]);

	////////////////////////////////////////////////////////////////////
	// no draw fix
	pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0xFFFFFFFF);

	render->Setup(pDevice);
	render->Present();
	////////////////////////////////////////////////////////////////////

	// restore render states	
	for (int i = 0; i < size; i++)
		pDevice->SetRenderState(backupStates[i], oldStates[i]);	
	
	// restore hook vmt
	static int elapsedFrames = 0;
	elapsedFrames++;
	if (elapsedFrames > 600)
	{
		//elapsedFrames = 0;
		hookDirectX->ApplyVMT();
	}
	
	return Present(pDevice, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
}


HRESULT WINAPI EndSceneHook(LPDIRECT3DDEVICE9 pDevice)
{
	if (!render || G::exit || reseting) return EndScene(pDevice);
	
	return EndScene(pDevice);
}

HRESULT WINAPI ResetHook(LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS *pPresentationParameters)
{
	if (!render || G::exit) return Reset(pDevice, pPresentationParameters);

	DirectX::ReleaseHook();

	reseting = true;
	render->OnLostDevice();

	HRESULT ret = Reset(pDevice, pPresentationParameters);

	DirectX::ApplyHook(pDevice);

	if (ret == D3D_OK)
	{
		reseting = false;
		render->OnResetDevice();
		render->Setup(pDevice);
	}
	
	return ret;
}
