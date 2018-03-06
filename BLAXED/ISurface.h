#pragma once

enum FontFlags_t
{
	FONTFLAG_NONE,
	FONTFLAG_ITALIC = 0x001,
	FONTFLAG_UNDERLINE = 0x002,
	FONTFLAG_STRIKEOUT = 0x004,
	FONTFLAG_SYMBOL = 0x008,
	FONTFLAG_ANTIALIAS = 0x010,
	FONTFLAG_GAUSSIANBLUR = 0x020,
	FONTFLAG_ROTARY = 0x040,
	FONTFLAG_DROPSHADOW = 0x080,
	FONTFLAG_ADDITIVE = 0x100,
	FONTFLAG_OUTLINE = 0x200,
	FONTFLAG_CUSTOM = 0x400,
	FONTFLAG_BITMAP = 0x800,
};

enum FontRenderFlag_t
{
	FONT_LEFT = 0,
	FONT_RIGHT = 1,
	FONT_CENTER = 2
};

enum FontDrawType_t
{
	FONT_DRAW_DEFAULT = 0,
	FONT_DRAW_NONADDITIVE,
	FONT_DRAW_ADDITIVE,
	FONT_DRAW_TYPE_COUNT = 2,
};

typedef unsigned long HFont;

struct FontVertex_t
{
	Vector2D m_Position;
	Vector2D m_TexCoord;

	FontVertex_t() {}
	FontVertex_t(const Vector2D &pos, const Vector2D &coord = Vector2D(0, 0))
	{
		m_Position = pos;
		m_TexCoord = coord;
	}
	void Init(const Vector2D &pos, const Vector2D &coord = Vector2D(0, 0))
	{
		m_Position = pos;
		m_TexCoord = coord;
	}
};

typedef FontVertex_t Vertex_t;

class ISurface
{
public:

	void DrawSetColor(int r, int g, int b, int a)
	{
		typedef void(__thiscall* OriginalFn)(void*, int, int, int, int);
		return Mem::CallVirtualFunction<OriginalFn>(this, 15)(this, r, g, b, a);
	}

	void DrawSetColor(Color col)
	{
		typedef void(__thiscall* OriginalFn)(void*, Color);
		return Mem::CallVirtualFunction<OriginalFn>(this, 14)(this, col);
	}

	void DrawFilledRect(int x0, int y0, int x1, int y1)
	{
		typedef void(__thiscall* OriginalFn)(void*, int, int, int, int);
		return Mem::CallVirtualFunction<OriginalFn>(this, 16)(this, x0, y0, x1, y1);
	}

	void DrawOutlinedRect(int x0, int y0, int x1, int y1)
	{
		typedef void(__thiscall* OriginalFn)(void*, int, int, int, int);
		return Mem::CallVirtualFunction<OriginalFn>(this, 18)(this, x0, y0, x1, y1);
	}

	void DrawLine(int x0, int y0, int x1, int y1)
	{
		typedef void(__thiscall* OriginalFn)(void*, int, int, int, int);
		return Mem::CallVirtualFunction<OriginalFn>(this, 19)(this, x0, y0, x1, y1);
	}

	void DrawPolyLine(int *px, int *py, int numPoints)
	{
		typedef void(__thiscall* OriginalFn)(void*, int*, int*, int);
		return Mem::CallVirtualFunction<OriginalFn>(this, 20)(this, px, py, numPoints);
	}

	void DrawSetTextFont(HFont font)
	{
		typedef void(__thiscall* OriginalFn)(void*, HFont);
		return Mem::CallVirtualFunction<OriginalFn>(this, 23)(this, font);
	}

	void DrawSetTextColor(int r, int g, int b, int a)
	{
		typedef void(__thiscall* OriginalFn)(void*, int, int, int, int);
		return Mem::CallVirtualFunction<OriginalFn>(this, 25)(this, r, g, b, a);
	}

	void DrawSetTextColor(Color col)
	{
		typedef void(__thiscall* OriginalFn)(void*, Color);
		return Mem::CallVirtualFunction<OriginalFn>(this, 24)(this, col);
	}

	void DrawSetTextPos(int x, int y)
	{
		typedef void(__thiscall* OriginalFn)(void*, int, int);
		return Mem::CallVirtualFunction<OriginalFn>(this, 26)(this, x, y);
	}

	void DrawPrintText(const wchar_t *text, int textLen, FontDrawType_t drawType)
	{
		typedef void(__thiscall* OriginalFn)(void*, const wchar_t*, int, FontDrawType_t);
		return Mem::CallVirtualFunction<OriginalFn>(this, 28)(this, text, textLen, drawType);
	}

	void DrawSetTextureRGBA(int id, const unsigned char *rgba, int wide, int tall)
	{
		typedef void(__thiscall* OriginalFn)(void*, int, const unsigned char*, int, int);
		return Mem::CallVirtualFunction<OriginalFn>(this, 37)(this, id, rgba, wide, tall);
	}

	void DrawSetTexture(int id)
	{
		typedef void(__thiscall* OriginalFn)(void*, int);
		return Mem::CallVirtualFunction<OriginalFn>(this, 38)(this, id);
	}

	int	CreateNewTextureID(bool procedural)
	{
		typedef int(__thiscall* OriginalFn)(void*, bool);
		return Mem::CallVirtualFunction<OriginalFn>(this, 43)(this, procedural);
	}

	HFont Create_Font()
	{
		typedef HFont(__thiscall* OriginalFn)(void*);
		return Mem::CallVirtualFunction<OriginalFn>(this, 71)(this);
	}

	bool SetFontGlyphSet(HFont font, const char *windowsFontName, int tall, int weight, int blur, int scanlines, int flags, int nRangeMin, int nRangeMax)
	{
		typedef bool(__thiscall* OriginalFn)(void*, HFont, const char*, int, int, int, int, int, int, int);
		return Mem::CallVirtualFunction<OriginalFn>(this, 72)(this, font, windowsFontName, tall, weight, blur, scanlines, flags, nRangeMin, nRangeMax);
	}

	void GetTextSize(HFont font, const wchar_t *text, int &wide, int &tall)
	{
		typedef void(__thiscall* OriginalFn)(void*, HFont, const wchar_t*, int&, int&);
		return Mem::CallVirtualFunction<OriginalFn>(this, 79)(this, font, text, wide, tall);
	}

	void DrawOutlinedCircle(int x, int y, int radius, int segments)
	{
		typedef void(__thiscall* OriginalFn)(void*, int, int, int, int);
		return Mem::CallVirtualFunction<OriginalFn>(this, 103)(this, x, y, radius, segments);
	}

	void DrawTexturedPolygon(int n, Vertex_t *pVertice, bool bClipVertices)
	{
		typedef void(__thiscall* OriginalFn)(void*, int, Vertex_t*, bool);
		return Mem::CallVirtualFunction<OriginalFn>(this, 106)(this, n, pVertice, bClipVertices);
	}
};
