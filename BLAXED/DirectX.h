#pragma once

#include <d3d9.h>
#include <d3dx9.h>

#define SAFE_RELEASE(p) { if (p) { (p)->Release(); (p)=NULL; } }

typedef HRESULT(WINAPI *Present_t)(LPDIRECT3DDEVICE9 pDevice, const RECT *pSourceRect, const RECT *pDestRect, HWND hDestWindowOverride, const RGNDATA *pDirtyRegion);
typedef HRESULT(WINAPI *Reset_t)(LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS *pPresentationParameters);
typedef HRESULT(WINAPI *GetBackBuffer_t)(LPDIRECT3DDEVICE9 pDevice, UINT iSwapChain, UINT BackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface9 **ppBackBuffer);
typedef HRESULT(WINAPI *GetRenderTargetData_t)(LPDIRECT3DDEVICE9 pDevice, IDirect3DSurface9 *pRenderTarget, IDirect3DSurface9 *pDestSurface);
typedef HRESULT(WINAPI *EndScene_t)(LPDIRECT3DDEVICE9 pDevice);

HRESULT WINAPI PresentHook(LPDIRECT3DDEVICE9 pDevice, const RECT *pSourceRect, const RECT *pDestRect, HWND hDestWindowOverride, const RGNDATA *pDirtyRegion);
HRESULT WINAPI ResetHook(LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS *pPresentationParameters);
HRESULT WINAPI GetBackBufferHook(LPDIRECT3DDEVICE9 pDevice, UINT iSwapChain, UINT BackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface9 **ppBackBuffer);
HRESULT WINAPI GetRenderTargetDataHook(LPDIRECT3DDEVICE9 pDevice, IDirect3DSurface9 *pRenderTarget, IDirect3DSurface9 *pDestSurface);
HRESULT WINAPI EndSceneHook(LPDIRECT3DDEVICE9 pDevice);

class DirectX
{
public:
	static void ApplyHook(LPDIRECT3DDEVICE9 pDevice);
	static void ReleaseHook();
};



