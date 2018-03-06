#pragma once

// credits: teamgamerfood

extern HFont hFntIndicators;
extern HFont hFntESP;

namespace D
{
	void SetupFonts();
	void DrawString(HFont font, int x, int y, Color color, DWORD alignment, const char* msg, ...);
	void DrawStringUnicode(HFont font, int x, int y, Color color, bool bCenter, const wchar_t* msg, ...);
	void DrawRect(int x, int y, int w, int h, Color col);
	void DrawRectRainbow(int x, int y, int w, int h, float flSpeed, float &flRainbow);
	void DrawRectGradientVertical(int x, int y, int w, int h, Color color1, Color color2);
	void DrawRectGradientHorizontal(int x, int y, int w, int h, Color color1, Color color2);
	void DrawPixel(int x, int y, Color col);
	void DrawOutlinedRect(int x, int y, int w, int h, Color col);
	void DrawOutlinedCircle(int x, int y, int r, Color col);
	void DrawLine(int x0, int y0, int x1, int y1, Color col);
	void DrawCorner(int iX, int iY, int iWidth, int iHeight, bool bRight, bool bDown, Color colDraw);
	void DrawRoundedBox(int x, int y, int w, int h, int r, int v, Color col);
	void DrawPolygon(int count, Vertex_t* Vertexs, Color color);
	bool ScreenTransform(const Vector &aimPoint, Vector &screen);
	bool WorldToScreen(const Vector &origin, Vector &screen);
	int GetStringWidth(HFont font, const char* msg, ...);
	void Draw3DBox(Vector* boxVectors, Color color);
	void DrawCircle(float x, float y, float r, float s, Color color);
};
