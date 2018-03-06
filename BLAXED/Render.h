#pragma once
#include <d3d9.h>
#include <d3dx9.h>

#define SAFE_RELEASE(p) { if (p) { (p)->Release(); (p)=NULL; } }

#define D3D_COLOR_LIME(a)	D3DCOLOR_ARGB(a, 204, 255, 153)
#define D3D_COLOR_WHITE(a)	D3DCOLOR_ARGB(a, 255, 255, 255)
#define D3D_COLOR_BLACK(a)	D3DCOLOR_ARGB(a, 0, 0, 0)
#define D3D_COLOR_RED(a)	D3DCOLOR_ARGB(a, 255, 0, 0)
#define D3D_COLOR_DARKRED(a)D3DCOLOR_ARGB(a, 153, 0, 0)
#define D3D_COLOR_GREEN(a)	D3DCOLOR_ARGB(a, 0, 255, 0)
#define D3D_COLOR_DARKGREEN(a)	D3DCOLOR_ARGB(a, 0, 153, 0)
#define D3D_COLOR_BLUE(a)	D3DCOLOR_ARGB(a, 0, 0, 255)
#define D3D_COLOR_DARKBLUE(a)D3DCOLOR_ARGB(a, 0, 0, 153)
#define D3D_COLOR_YELLOW(a)	D3DCOLOR_ARGB(a, 255, 255, 0)
#define D3D_COLOR_PINK(a)	D3DCOLOR_ARGB(a, 255, 0, 255)
#define D3D_COLOR_ORANGE(a)	D3DCOLOR_ARGB(a, 255, 153, 0)
#define D3D_COLOR_LIGHTBLUE(a)D3DCOLOR_ARGB(a, 0, 255, 255) 
#define D3D_COLOR_BROWN(a)	D3DCOLOR_ARGB(a, 153, 102, 0)
#define D3D_COLOR_GRAY(a)	D3DCOLOR_ARGB(a, 153, 153, 153)
#define D3D_COLOR_DARKGRAY(a)	D3DCOLOR_ARGB(a, 13, 13, 13)

struct D3DTLVERTEX
{
	float x, y, z, rhw;
	DWORD color;
};

class Render
{
	bool locked;
	bool resourcesCreated;

	LPD3DXLINE line;

public:
	bool setupComplete;

	LPDIRECT3DDEVICE9 pDevice;
	D3DVIEWPORT9 viewPort;
	HWND window;
	POINT mouse;
	LPD3DXFONT font;
	LPD3DXFONT fntLucidaConsole10;
	LPD3DXFONT fntLucidaConsole15;
	LPD3DXFONT fntTahoma;
	LPD3DXFONT fntVerdana10;
	LPD3DXFONT fntVerdana9;
	LPD3DXFONT fntVerdana8;
	LPD3DXFONT fntWeaponIcon;

	int numScreenShots;

	Render();
	~Render();
	void Shutdown();
	void Setup(LPDIRECT3DDEVICE9 pDevice);
	void OnLostDevice();
	void OnResetDevice();
	void Present();


	// Misc
	void SetFont(LPD3DXFONT font);
	bool WorldToScreen(Vector vOrigin, Vector &vScreen);

	// Rendering
	void DrawLine(float x1, float y1, float x2, float y2, float size, bool antiAlias, D3DCOLOR COLOR);
	void DrawLineFast(float x1, float y1, float x2, float y2, D3DCOLOR COLOR);
	void DrawString(float x, float y, D3DCOLOR color, char *format, ...);
	void DrawStringW(float x, float y, D3DCOLOR color, WCHAR *format, ...);
	void DrawStringWithFont(LPD3DXFONT fnt, float x, float y, D3DCOLOR color, char *format, ...);
	void DrawStringWithFontW(LPD3DXFONT fnt, float x, float y, D3DCOLOR color, wchar_t *format, ...);
	void DrawCircle(float x, float y, int radius, int numSides, float size, D3DCOLOR color);
	void DrawCircleFast(float x, float y, int radius, int numSides, D3DCOLOR color);
	void DrawBox(float x, float y, float height, float width, float size, D3DCOLOR COLOR);
	void DrawBoxFast(float x, float y, float height, float width, D3DCOLOR COLOR); 
	void DrawFilledRectangle(float x, float y, float height, float width, D3DCOLOR COLOR);
	int GetTextWitdh(char *text, LPD3DXFONT fnt);
	int GetTextWitdhW(wchar_t *text, LPD3DXFONT fnt);
};

extern Render *render;

