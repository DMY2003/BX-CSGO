#include "SDK.h"
#include "Draw.h"

HFont hFntIndicators;
HFont hFntESP;

void D::SetupFonts()
{
	I::pSurface->SetFontGlyphSet(hFntIndicators = I::pSurface->Create_Font(), xorstr("Verdana"), 20, FW_DONTCARE, NULL, NULL, FONTFLAG_OUTLINE, 0, 0);
	I::pSurface->SetFontGlyphSet(hFntESP = I::pSurface->Create_Font(), xorstr("Verdana"), 12, FW_DONTCARE, NULL, NULL, FONTFLAG_DROPSHADOW, 0, 0);
}

void D::DrawString(HFont font, int x, int y, Color color, DWORD alignment, const char* msg, ...)
{
	va_list va_alist;
	char buf[1024];
	va_start(va_alist, msg);
	_vsnprintf(buf, sizeof(buf), msg, va_alist);
	va_end(va_alist);
	wchar_t wbuf[1024];
	MultiByteToWideChar(CP_UTF8, 0, buf, 256, wbuf, 256);

	uint8_t r = 255, g = 255, b = 255, a = 255;
	color.GetColor(r, g, b, a);

	int width, height;
	I::pSurface->GetTextSize(font, wbuf, width, height);

	if (alignment & FONT_RIGHT)
		x -= width;
	if (alignment & FONT_CENTER)
		x -= width / 2;

	I::pSurface->DrawSetTextFont(font);
	I::pSurface->DrawSetTextColor(r, g, b, a);
	I::pSurface->DrawSetTextPos(x, y - height / 2);
	I::pSurface->DrawPrintText(wbuf, wcslen(wbuf), FONT_DRAW_DEFAULT);
}

void D::DrawStringUnicode(HFont font, int x, int y, Color color, bool bCenter, const wchar_t* msg, ...)
{
	uint8_t r = 255, g = 255, b = 255, a = 255;
	color.GetColor(r, g, b, a);

	int iWidth, iHeight;

	I::pSurface->GetTextSize(font, msg, iWidth, iHeight);
	I::pSurface->DrawSetTextFont(font);
	I::pSurface->DrawSetTextColor(r, g, b, a);
	I::pSurface->DrawSetTextPos(!bCenter ? x : x - iWidth / 2, y - iHeight / 2);
	I::pSurface->DrawPrintText(msg, wcslen(msg), FONT_DRAW_DEFAULT);
}

void D::DrawRect(int x, int y, int w, int h, Color col)
{
	I::pSurface->DrawSetColor(col);
	I::pSurface->DrawFilledRect(x, y, x + w, y + h);
}

void D::DrawRectRainbow(int x, int y, int width, int height, float flSpeed, float &flRainbow)
{
	Color colColor(0, 0, 0);

	flRainbow += flSpeed;
	if (flRainbow > 1.f) flRainbow = 0.f;

	for (int i = 0; i < width; i++)
	{
		float hue = (1.f / (float)width) * i;
		hue -= flRainbow;
		if (hue < 0.f) hue += 1.f;

		Color colRainbow = colColor.FromHSB(hue, 1.f, 1.f);
		D::DrawRect(x + i, y, 1, height, colRainbow);
	}
}

void D::DrawRectGradientHorizontal(int x, int y, int width, int height, Color color1, Color color2)
{
	float flDifferenceR = (float)(color2.r() - color1.r()) / (float)width;
	float flDifferenceG = (float)(color2.g() - color1.g()) / (float)width;
	float flDifferenceB = (float)(color2.b() - color1.b()) / (float)width;

	for (float i = 0.f; i < width; i++)
	{
		Color colGradient = Color((uint8_t)(color1.r() + (flDifferenceR * i)), (uint8_t)(color1.g() + (flDifferenceG * i)), (uint8_t)(color1.b() + (flDifferenceB * i)), (uint8_t)(color1.a()));
		D::DrawRect((int)(x + i), (int)(y), 1, height, colGradient);
	}
}

void D::DrawPixel(int x, int y, Color col)
{
	I::pSurface->DrawSetColor(col);
	I::pSurface->DrawFilledRect(x, y, x + 1, y + 1);
}

void D::DrawOutlinedRect(int x, int y, int w, int h, Color col)
{
	I::pSurface->DrawSetColor(col);
	I::pSurface->DrawOutlinedRect(x, y, x + w, y + h);
}

void D::DrawOutlinedCircle(int x, int y, int r, Color col)
{
	I::pSurface->DrawSetColor(col);
	I::pSurface->DrawOutlinedCircle(x, y, r, 1);
}

void D::DrawLine(int x0, int y0, int x1, int y1, Color col)
{
	I::pSurface->DrawSetColor(col);
	I::pSurface->DrawLine(x0, y0, x1, y1);
}

void D::DrawCorner(int iX, int iY, int iWidth, int iHeight, bool bRight, bool bDown, Color colDraw)
{
	int iRealX = bRight ? iX - iWidth : iX;
	int iRealY = bDown ? iY - iHeight : iY;

	if (bDown && bRight)
		iWidth = iWidth + 1;

	D::DrawRect(iRealX, iY, iWidth, 1, colDraw);
	D::DrawRect(iX, iRealY, 1, iHeight, colDraw);

	D::DrawRect(iRealX, bDown ? iY + 1 : iY - 1, !bDown && bRight ? iWidth + 1 : iWidth, 1, Color(0, 0, 0, 255));
	D::DrawRect(bRight ? iX + 1 : iX - 1, bDown ? iRealY : iRealY - 1, 1, bDown ? iHeight + 2 : iHeight + 1, Color(0, 0, 0, 255));
}

void D::DrawPolygon(int count, Vertex_t* Vertexs, Color color)
{
	static int Texture = I::pSurface->CreateNewTextureID(true);
	unsigned char buffer[4] = { 255, 255, 255, 255 };

	I::pSurface->DrawSetTextureRGBA(Texture, buffer, 1, 1);
	I::pSurface->DrawSetColor(color);
	I::pSurface->DrawSetTexture(Texture);

	I::pSurface->DrawTexturedPolygon(count, Vertexs, true);
}

void D::DrawRoundedBox(int x, int y, int w, int h, int r, int v, Color col)
{
	std::vector<Vertex_t> p;
	for (int _i = 0; _i < 3; _i++)
	{
		int _x = x + (_i < 2 && r || w - r);
		int _y = y + (_i % 3 > 0 && r || h - r);
		for (int i = 0; i < v; i++)
		{
			float a = RAD2DEG((i / v) * -90 - _i * 90);
			p.push_back(Vertex_t(Vector2D(_x + sinf(a) * r, _y + cosf(a) * r)));
		}
	}

	D::DrawPolygon(4 * (v + 1), &p[0], col);
	/*
	function DrawRoundedBox(x, y, w, h, r, v, col)
	local p = {};
	for _i = 0, 3 do
	local _x = x + (_i < 2 && r || w - r)
	local _y = y + (_i%3 > 0 && r || h - r)
	for i = 0, v do
	local a = math.rad((i / v) * - 90 - _i * 90)
	table.insert(p, {x = _x + math.sin(a) * r, y = _y + math.cos(a) * r})
	end
	end

	surface.SetDrawColor(col.r, col.g, col.b, 255)
	draw.NoTexture()
	surface.DrawPoly(p)
	end
	*/

	// Notes: amount of vertexes is 4(v + 1) where v is the number of vertices on each corner bit.
	// I did it in lua cause I have no idea how the vertex_t struct works and i'm still aids at C++
}

bool D::ScreenTransform(const Vector &aimPoint, Vector &screen) // tots not pasted
{
	float w;
	const VMatrix &worldToScreen = I::pEngineClient->WorldToScreenMatrix();

	screen.x = worldToScreen[0][0] * aimPoint[0] + worldToScreen[0][1] * aimPoint[1] + worldToScreen[0][2] * aimPoint[2] + worldToScreen[0][3];
	screen.y = worldToScreen[1][0] * aimPoint[0] + worldToScreen[1][1] * aimPoint[1] + worldToScreen[1][2] * aimPoint[2] + worldToScreen[1][3];
	w = worldToScreen[3][0] * aimPoint[0] + worldToScreen[3][1] * aimPoint[1] + worldToScreen[3][2] * aimPoint[2] + worldToScreen[3][3];
	screen.z = 0.0f;

	bool behind = false;

	if (w < 0.001f)
	{
		behind = true;
		screen.x *= 100000;
		screen.y *= 100000;
	}
	else
	{
		behind = false;
		float invw = 1.0f / w;
		screen.x *= invw;
		screen.y *= invw;
	}

	return behind;
}

bool D::WorldToScreen(const Vector &origin, Vector &screen)
{
	if (!ScreenTransform(origin, screen))
	{
		int ScreenWidth, ScreenHeight;
		I::pEngineClient->GetScreenSize(ScreenWidth, ScreenHeight);
		float x = (float)ScreenWidth / 2;
		float y = (float)ScreenHeight / 2;
		x += 0.5f * screen.x * ScreenWidth + 0.5f;
		y -= 0.5f * screen.y * ScreenHeight + 0.5f;
		screen.x = x;
		screen.y = y;

		//if (x >= 0 && x <= ScreenWidth && y >= 0 && y <= ScreenHeight)
			return true;
	}

	return false;
}

int D::GetStringWidth(HFont font, const char* msg, ...)
{
	va_list va_alist;
	char buf[1024];
	va_start(va_alist, msg);
	_vsnprintf(buf, sizeof(buf), msg, va_alist);
	va_end(va_alist);
	wchar_t wbuf[1024];
	MultiByteToWideChar(CP_UTF8, 0, buf, 256, wbuf, 256);

	int iWidth, iHeight;

	I::pSurface->GetTextSize(font, wbuf, iWidth, iHeight);

	return iWidth;
}

void D::Draw3DBox(Vector* boxVectors, Color color)
{
	Vector boxVectors0, boxVectors1, boxVectors2, boxVectors3,
		boxVectors4, boxVectors5, boxVectors6, boxVectors7;


	if (D::WorldToScreen(boxVectors[0], boxVectors0) &&
		D::WorldToScreen(boxVectors[1], boxVectors1) &&
		D::WorldToScreen(boxVectors[2], boxVectors2) &&
		D::WorldToScreen(boxVectors[3], boxVectors3) &&
		D::WorldToScreen(boxVectors[4], boxVectors4) &&
		D::WorldToScreen(boxVectors[5], boxVectors5) &&
		D::WorldToScreen(boxVectors[6], boxVectors6) &&
		D::WorldToScreen(boxVectors[7], boxVectors7))
	{

		/*
		.+--5---+
		.8 4    6'|
		+--7---+'  11
		9   |  10  |
		|  ,+--|3--+
		|.0    | 2'
		+--1---+'
		*/

		Vector2D lines[12][2];
		//bottom of box
		lines[0][0] = { boxVectors0.x, boxVectors0.y };
		lines[0][1] = { boxVectors1.x, boxVectors1.y };
		lines[1][0] = { boxVectors1.x, boxVectors1.y };
		lines[1][1] = { boxVectors2.x, boxVectors2.y };
		lines[2][0] = { boxVectors2.x, boxVectors2.y };
		lines[2][1] = { boxVectors3.x, boxVectors3.y };
		lines[3][0] = { boxVectors3.x, boxVectors3.y };
		lines[3][1] = { boxVectors0.x, boxVectors0.y };

		lines[4][0] = { boxVectors0.x, boxVectors0.y };
		lines[4][1] = { boxVectors6.x, boxVectors6.y };

		// top of box
		lines[5][0] = { boxVectors6.x, boxVectors6.y };
		lines[5][1] = { boxVectors5.x, boxVectors5.y };
		lines[6][0] = { boxVectors5.x, boxVectors5.y };
		lines[6][1] = { boxVectors4.x, boxVectors4.y };
		lines[7][0] = { boxVectors4.x, boxVectors4.y };
		lines[7][1] = { boxVectors7.x, boxVectors7.y };
		lines[8][0] = { boxVectors7.x, boxVectors7.y };
		lines[8][1] = { boxVectors6.x, boxVectors6.y };


		lines[9][0] = { boxVectors5.x, boxVectors5.y };
		lines[9][1] = { boxVectors1.x, boxVectors1.y };

		lines[10][0] = { boxVectors4.x, boxVectors4.y };
		lines[10][1] = { boxVectors2.x, boxVectors2.y };

		lines[11][0] = { boxVectors7.x, boxVectors7.y };
		lines[11][1] = { boxVectors3.x, boxVectors3.y };

		for (int i = 0; i < 12; i++)
		{
			D::DrawLine((int)lines[i][0].x, (int)lines[i][0].y, (int)lines[i][1].x, (int)lines[i][1].y, color);
		}
	}
}

void D::DrawCircle(float x, float y, float r, float s, Color color)
{
	float Step = M_PI * 2.0f / s;
	for (float a = 0; a < (M_PI*2.0); a += Step)
	{
		float x1 = r * cos(a) + x;
		float y1 = r * sin(a) + y;
		float x2 = r * cos(a + Step) + x;
		float y2 = r * sin(a + Step) + y;

		DrawLine((int)x1, (int)y1, (int)x2, (int)y2, color);
	}
}
