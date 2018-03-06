#include "SDK.h"
#include "Global.h"
#include "Render.h"
#include "WndProc.h"
#include "ESP.h"
#include "MenuV2.h"

Render *render = new Render();

Render::Render()
{
	pDevice = nullptr;
	line = nullptr;

	setupComplete = false;
	resourcesCreated = false;
	locked = false;

	line = NULL;
	fntLucidaConsole10 = NULL;
	fntLucidaConsole15 = NULL;
	fntTahoma = NULL;
	fntVerdana9 = NULL;
	fntVerdana8 = NULL;
	fntVerdana10 = NULL;

	numScreenShots = 0;
}

Render::~Render()
{
	ImGui_ImplDX9_InvalidateDeviceObjects();
	ImGui_ImplDX9_Shutdown();

	cfg.Visual.enabled = false;
	
	SAFE_RELEASE(line);
	SAFE_RELEASE(fntLucidaConsole10);
	SAFE_RELEASE(fntLucidaConsole15);
	SAFE_RELEASE(fntTahoma);
	SAFE_RELEASE(fntVerdana10);
	SAFE_RELEASE(fntVerdana9);
	SAFE_RELEASE(fntVerdana8);
	SAFE_RELEASE(fntWeaponIcon);

	resourcesCreated = false;
	locked = true;

	ResetWindow();
}

void Render::Shutdown()
{
	cfg.Visual.enabled = false;

	locked = true;
}

void Render::Setup(LPDIRECT3DDEVICE9 pDevice)
{
	if (locked) return;

	this->pDevice = pDevice;

	if (window)
	{
		if (GetCursorPos(&mouse))
			ScreenToClient(window, &mouse);		
	}

	// misc
	D3DDEVICE_CREATION_PARAMETERS cp;

	pDevice->GetCreationParameters(&cp);
	pDevice->GetViewport(&viewPort);

	window = cp.hFocusWindow;
	
	if (setupComplete)
		return;

	SetupWindow();

	if (!G::GUIInited)
		MenuV2::GUI_Init();	

	// resources
	if (!resourcesCreated)
	{
		D3DXCreateLine(pDevice, &line);
		D3DXCreateFont(pDevice, 10, 0, FW_NORMAL, 1, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, xorstr("Lucida Console"), &fntLucidaConsole10);
		D3DXCreateFont(pDevice, 15, 0, FW_NORMAL, 1, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, xorstr("Lucida Console"), &fntLucidaConsole15);
		D3DXCreateFont(pDevice, 12, 0, FW_NORMAL, 1, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DRAFT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, xorstr("Tahoma"), &fntTahoma);
		D3DXCreateFont(pDevice, 9, 0, FW_BOLD, 1, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DRAFT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, xorstr("Verdana"), &fntVerdana9);
		D3DXCreateFont(pDevice, 8, 0, FW_BOLD, 1, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DRAFT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, xorstr("Verdana"), &fntVerdana8);
		D3DXCreateFont(pDevice, 10, 5, FW_NORMAL, 1, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, xorstr("Verdana"), &fntVerdana10);
		D3DXCreateFont(pDevice, 11, 0, FW_NORMAL, 1, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, xorstr("csgo_icons"), &fntWeaponIcon);
		
		font = fntVerdana10;
		resourcesCreated = true;
	}

	setupComplete = true;
}

void Render::OnLostDevice()
{
	if (line)
		line->OnLostDevice();

	if (fntLucidaConsole10)
		fntLucidaConsole10->OnLostDevice();

	if (fntLucidaConsole15)
		fntLucidaConsole15->OnLostDevice();

	if (fntTahoma)
		fntTahoma->OnLostDevice();

	if (fntVerdana10)
		fntVerdana10->OnLostDevice();

	if (fntVerdana9)
		fntVerdana9->OnLostDevice();

	if (fntVerdana8)
		fntVerdana8->OnLostDevice();

	if (fntWeaponIcon)
		fntWeaponIcon->OnLostDevice();

	ImGui_ImplDX9_InvalidateDeviceObjects();
}

void Render::OnResetDevice()
{
	if (line)
		line->OnResetDevice();

	if (fntLucidaConsole10)
		fntLucidaConsole10->OnResetDevice();

	if (fntLucidaConsole15)
		fntLucidaConsole15->OnResetDevice();

	if (fntTahoma)
		fntTahoma->OnResetDevice();

	if (fntVerdana10)
		fntVerdana10->OnResetDevice();

	if (fntVerdana9)
		fntVerdana9->OnResetDevice();
	
	if (fntVerdana8)
		fntVerdana8->OnResetDevice();

	if (fntWeaponIcon)
		fntWeaponIcon->OnResetDevice();

	ImGui_ImplDX9_CreateDeviceObjects();
}

void Render::Present()
{
	ESP::Render();

	//Menu();
	MenuV2::Draw();
}


// Misc
void Render::SetFont(LPD3DXFONT font)
{
	this->font = font;
}


/*//Flicker with multicore rendering
bool Render::ScreenTransform(const Vector& point, Vector& screen)
{
static VMatrix *p_w2sMatrix = 0;

if (!p_w2sMatrix)
{
p_w2sMatrix = (VMatrix*)g_pEngineClient->WorldToScreenMatrix();
}

const VMatrix& w2sMatrix = *(VMatrix*)p_w2sMatrix;

screen.x = w2sMatrix.m[0][0] * point.x + w2sMatrix.m[0][1] * point.y + w2sMatrix.m[0][2] * point.z + w2sMatrix.m[0][3];
screen.y = w2sMatrix.m[1][0] * point.x + w2sMatrix.m[1][1] * point.y + w2sMatrix.m[1][2] * point.z + w2sMatrix.m[1][3];
screen.z = 0.0f;

float w = w2sMatrix.m[3][0] * point.x + w2sMatrix.m[3][1] * point.y + w2sMatrix.m[3][2] * point.z + w2sMatrix.m[3][3];

if (w < 0.001f) {
screen.x *= 100000;
screen.y *= 100000;
return true;
}

float invw = 1.0f / w;
screen.x *= invw;
screen.y *= invw;

return false;
}

bool Render::WorldToScreen(const Vector &origin, Vector &screen)
{
if (!ScreenTransform(origin, screen)) {
int iScreenWidth, iScreenHeight;
g_pEngineClient->GetScreenSize(iScreenWidth, iScreenHeight);

screen.x = (iScreenWidth / 2.0f) + (screen.x * iScreenWidth) / 2;
screen.y = (iScreenHeight / 2.0f) - (screen.y * iScreenHeight) / 2;

return true;
}
return false;
}*/

#define POINTER_TO_MATRIX(type,name,dimension,addr) type (&name)dimension = *(type(*)dimension)(addr);

bool Render::WorldToScreen(Vector vOrigin, Vector &vScreen)
{
	static uintptr_t clientDll = 0;
	while (!clientDll) { clientDll = (uintptr_t)GetModuleHandle(xorstr("client.dll")); Sleep(100); }

	//POINTER_TO_MATRIX(float, viewMatrix, [4][4], (clientDll + 0x4A78EE4));

	//if (!viewMatrix)
	//	return false;

	//if (!viewMatrix[0] || !viewMatrix[1] || !viewMatrix[2] || !viewMatrix[3])
	//	return false;

	float w = G::viewMatrix[3][0] * vOrigin.x + G::viewMatrix[3][1] * vOrigin.y + G::viewMatrix[3][2] * vOrigin.z + G::viewMatrix[3][3];

	float ScreenWidth = (float)viewPort.Width;
	float ScreenHeight = (float)viewPort.Height;

	if (w > 0.01)
	{
		float inverseWidth = 1 / w;
		vScreen.x = (float)((ScreenWidth / 2) + (0.5 * ((G::viewMatrix[0][0] * vOrigin.x + G::viewMatrix[0][1] * vOrigin.y + G::viewMatrix[0][2] * vOrigin.z + G::viewMatrix[0][3]) * inverseWidth) * ScreenWidth + 0.5));
		vScreen.y = (float)((ScreenHeight / 2) - (0.5 * ((G::viewMatrix[1][0] * vOrigin.x + G::viewMatrix[1][1] * vOrigin.y + G::viewMatrix[1][2] * vOrigin.z + G::viewMatrix[1][3]) * inverseWidth) * ScreenHeight + 0.5));
		return true;
	}

	return false;
}



// Rendering
void Render::DrawLine(float x1, float y1, float x2, float y2, float size, bool antiAlias, D3DCOLOR COLOR)
{
	D3DXVECTOR2 dLine[2];

	line->SetWidth(size);

	dLine[1] = { x1, y1 };
	dLine[0] = { x2, y2 };

	if (antiAlias)
		line->SetAntialias(1);
	else
		line->SetAntialias(0);
	
	line->Begin();
	line->Draw(dLine, 2, COLOR);
	line->End();
}

void Render::DrawLineFast(float x1, float y1, float x2, float y2, D3DCOLOR COLOR)
{
	D3DTLVERTEX qV[2] = {
		{ x1, y1, 0.0f, 1.0f, COLOR },
		{ x2, y2, 0.0f, 1.0f, COLOR },
	};

	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA); //pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCALPHA); // neon mode
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_FOGENABLE, false);

	DWORD oldFVF;

	pDevice->GetFVF(&oldFVF);
	pDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
	pDevice->SetTexture(0, 0);
	pDevice->DrawPrimitiveUP(D3DPT_LINELIST, 1, qV, sizeof(D3DTLVERTEX));
	pDevice->SetFVF(oldFVF);
}

void Render::DrawString(float x, float y, D3DCOLOR color, char *format, ...)
{
	char buffer[256];
	RECT fontRect = { (int)x, (int)y, (int)x, (int)y };

	va_list va_argList;

	va_start(va_argList, format);
	wvsprintf(buffer, format, va_argList);
	va_end(va_argList);

	font->DrawText(NULL, buffer, strlen(buffer), &fontRect, DT_NOCLIP, color);
}

void Render::DrawStringW(float x, float y, D3DCOLOR color, WCHAR *format, ...)
{
	wchar_t buffer[256];
	RECT fontRect = { (int)x, (int)y, (int)x, (int)y };

	va_list va_argList;

	va_start(va_argList, format);
	wvsprintfW(buffer, format, va_argList);
	va_end(va_argList);

	font->DrawTextW(NULL, buffer, -1, &fontRect, DT_NOCLIP, color);
}

void Render::DrawStringWithFont(LPD3DXFONT fnt, float x, float y, D3DCOLOR color, char *format, ...)
{	
	char buffer[256];
	RECT fontRect = { (int)x, (int)y, (int)x, (int)y };

	va_list va_argList;

	va_start(va_argList, format);
	wvsprintf(buffer, format, va_argList);
	va_end(va_argList);

	fnt->DrawText(NULL, buffer, strlen(buffer), &fontRect, DT_NOCLIP, color);
}

void Render::DrawStringWithFontW(LPD3DXFONT fnt, float x, float y, D3DCOLOR color, wchar_t *format, ...)
{
	wchar_t buffer[256];
	RECT fontRect = { (int)x, (int)y, (int)x, (int)y };

	va_list va_argList;

	va_start(va_argList, format);
	wvsprintfW(buffer, format, va_argList);
	va_end(va_argList);

	fnt->DrawTextW(NULL, buffer, -1, &fontRect, DT_NOCLIP, color);
}

int Render::GetTextWitdh(char *text, LPD3DXFONT fnt)
{
	RECT fontRect = { 0,0,0,0 };

	fnt->DrawText(NULL, text, strlen(text), &fontRect, DT_CALCRECT, D3DCOLOR_XRGB(0, 0, 0));

	return fontRect.right - fontRect.left;
}

int Render::GetTextWitdhW(wchar_t *text, LPD3DXFONT fnt)
{
	RECT fontRect = { 0,0,0,0 };

	fnt->DrawTextW(NULL, text, -1, &fontRect, DT_CALCRECT, D3DCOLOR_XRGB(0, 0, 0));

	return fontRect.right - fontRect.left;
}


void Render::DrawCircle(float x, float y, int radius, int numSides, float size, D3DCOLOR color)
{
	float step = (float)(D3DX_PI * 2.0 / numSides);

	if (step <= 0)
		return;

	for (float a = 0; a < D3DX_PI*2.0; a += step)
	{
		float X1 = radius * cosf(a) + x;
		float Y1 = radius * sinf(a) + y;
		float X2 = radius * cosf(a + step) + x;
		float Y2 = radius * sinf(a + step) + y;

		DrawLine(X1, Y1, X2, Y2, size, true, color);
	}
}


void Render::DrawCircleFast(float x, float y, int radius, int numSides, D3DCOLOR color)
{
	float step = (float)(D3DX_PI * 2.0 / numSides);

	if (step <= 0)
		return;

	for (float a = 0; a < D3DX_PI*2.0; a += step)
	{
		float X1 = radius * cosf(a) + x;
		float Y1 = radius * sinf(a) + y;
		float X2 = radius * cosf(a + step) + x;
		float Y2 = radius * sinf(a + step) + y;

		DrawLineFast(X1, Y1, X2, Y2, color);
	}
}


void Render::DrawBox(float x, float y, float height, float width, float size, D3DCOLOR COLOR)
{
	D3DXVECTOR2 points[5];

	points[0] = D3DXVECTOR2(x, y);
	points[1] = D3DXVECTOR2(x + width, y);
	points[2] = D3DXVECTOR2(x + width, y + height);
	points[3] = D3DXVECTOR2(x, y + height);
	points[4] = D3DXVECTOR2(x, y);
		
	line->SetWidth(size);
	line->SetAntialias(1);
	line->Begin();
	line->Draw(points, 5, COLOR);
	line->End();
}

void Render::DrawBoxFast(float x, float y, float height, float width, D3DCOLOR COLOR)
{
	D3DXVECTOR2 points[5];

	points[0] = D3DXVECTOR2(x, y);
	points[1] = D3DXVECTOR2(x + width, y);
	points[2] = D3DXVECTOR2(x + width, y + height);
	points[3] = D3DXVECTOR2(x, y + height);
	points[4] = D3DXVECTOR2(x, y);

	DrawLineFast(points[0].x, points[0].y, points[1].x, points[1].y, COLOR);
	DrawLineFast(points[1].x, points[1].y, points[2].x, points[2].y, COLOR);
	DrawLineFast(points[2].x, points[2].y, points[3].x, points[3].y, COLOR);
	DrawLineFast(points[3].x, points[3].y, points[4].x, points[4].y, COLOR);
}

void Render::DrawFilledRectangle(float x, float y, float height, float width, D3DCOLOR COLOR)
{
	D3DXVECTOR2 points[5];

	/*
	x	y
	h	w
	*/

	D3DTLVERTEX t1[3] = {
		{ x, y, 0.0f, 1.0f, COLOR },
		{ x + width, y, 0.0f, 1.0f, COLOR },
		{ x + width, y + height, 0.0f, 1.0f, COLOR },		
	};

	D3DTLVERTEX t2[3] = {		
		{ x , y, 0.0f, 1.0f, COLOR },
		{ x + width , y + height, 0.0f, 1.0f, COLOR },
		{ x, y + height, 0.0f, 1.0f, COLOR },
	};

	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA); //pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCALPHA); // neon mode
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_FOGENABLE, false);

	DWORD oldFVF;

	pDevice->GetFVF(&oldFVF);
	pDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
	pDevice->SetTexture(0, 0);
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 1, t1, sizeof(D3DTLVERTEX));
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 1, t2, sizeof(D3DTLVERTEX));
	pDevice->SetFVF(oldFVF);
}
