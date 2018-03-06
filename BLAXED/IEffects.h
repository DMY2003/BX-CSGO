#pragma once

struct ColorRGBExp32
{
	byte r, g, b;
	signed char exponent;

	ColorRGBExp32(byte r, byte g, byte b, byte e)
	{
		this->r = r;
		this->g = g;
		this->b = b;
		this->exponent = e;
	}
};

enum
{
	DLIGHT_NO_WORLD_ILLUMINATION = 0x1,
	DLIGHT_NO_MODEL_ILLUMINATION = 0x2,

	// NOTE: These two features are used to dynamically tweak the alpha on displacements
	// which is a special effect for selecting which texture to use. If 
	// we ever change how alpha is stored for displacements, we'll have to kill this feature
	DLIGHT_ADD_DISPLACEMENT_ALPHA = 0x4,
	DLIGHT_SUBTRACT_DISPLACEMENT_ALPHA = 0x8,
	DLIGHT_DISPLACEMENT_MASK = (DLIGHT_ADD_DISPLACEMENT_ALPHA | DLIGHT_SUBTRACT_DISPLACEMENT_ALPHA),
};

struct dlight_t
{
	int		flags;
	Vector	origin;
	float	radius;
	ColorRGBExp32	color;
	float	die;
	float	decay;
	float	minlight;
	int		key;
	int		style;
	Vector	m_Direction;
	float	m_InnerAngle;
	float	m_OuterAngle;

	float getrad() const
	{
		return radius;
	}

	float getradsqr() const
	{
		return radius * radius;
	}

	float check() const
	{
		return radius > 0.0f;
	}
};

class IVEffects
{
public:
	dlight_t* CL_AllocDlight(int key)
	{
		typedef dlight_t*(__thiscall* OriginalFn)(void*, int);
		return Mem::CallVirtualFunction<OriginalFn>(this, 4)(this, key);
	}

	dlight_t* CL_AllocElight(int key)
	{
		typedef dlight_t*(__thiscall* OriginalFn)(void*, int);
		return Mem::CallVirtualFunction<OriginalFn>(this, 5)(this, key);
	}

	dlight_t *GetElightByKey(int key)
	{
		typedef dlight_t*(__thiscall* OriginalFn)(void*, int);
		return Mem::CallVirtualFunction<OriginalFn>(this, 8)(this, key);
	}
};
