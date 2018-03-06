#pragma once

enum LightType_t
{
	MATERIAL_LIGHT_DISABLE = 0,
	MATERIAL_LIGHT_POINT,
	MATERIAL_LIGHT_DIRECTIONAL,
	MATERIAL_LIGHT_SPOT,
};

struct LightDesc_t
{
	LightType_t m_Type;										//< MATERIAL_LIGHT_xxx
	Vector m_Color;											//< color+intensity 
	Vector m_Position;										//< light source center position
	Vector m_Direction;										//< for SPOT, direction it is pointing
	float  m_Range;											//< distance range for light.0=infinite
	float m_Falloff;										//< angular falloff exponent for spot lights
	float m_Attenuation0;									//< constant distance falloff term
	float m_Attenuation1;									//< linear term of falloff
	float m_Attenuation2;									//< quadatic term of falloff
	float m_Theta;											//< inner cone angle. no angular falloff 
															//< within this cone
	float m_Phi;											//< outer cone angle

															// the values below are derived from the above settings for optimizations
															// These aren't used by DX8. . used for software lighting.
	float m_ThetaDot;
	float m_PhiDot;
	unsigned int m_Flags;
protected:
	float OneOver_ThetaDot_Minus_PhiDot;
	float m_RangeSquared;
public:
};

struct DrawModelInfo_t
{
	studiohdr_t		*m_pStudioHdr;
	void	*m_pHardwareData;
	void* m_Decals;
	int				m_Skin;
	int				m_Body;
	int				m_HitboxSet;
	DWORD			*m_pClientEntity;
	int				m_Lod;
	void	*m_pColorMeshes;
	bool			m_bStaticLighting;
	Vector			m_vecAmbientCube[6];		// ambient, and lights that aren't in locallight[]
	int				m_nLocalLightCount;
	LightDesc_t		m_LocalLightDescs[4];
};

//-----------------------------------------------------------------------------
// DrawModel flags
//-----------------------------------------------------------------------------
enum
{
	STUDIORENDER_DRAW_ENTIRE_MODEL = 0,
	STUDIORENDER_DRAW_OPAQUE_ONLY = 0x01,
	STUDIORENDER_DRAW_TRANSLUCENT_ONLY = 0x02,
	STUDIORENDER_DRAW_GROUP_MASK = 0x03,

	STUDIORENDER_DRAW_NO_FLEXES = 0x04,
	STUDIORENDER_DRAW_STATIC_LIGHTING = 0x08,

	STUDIORENDER_DRAW_ACCURATETIME = 0x10,		// Use accurate timing when drawing the model.
	STUDIORENDER_DRAW_NO_SHADOWS = 0x20,
	STUDIORENDER_DRAW_GET_PERF_STATS = 0x40,

	STUDIORENDER_DRAW_WIREFRAME = 0x80,

	STUDIORENDER_DRAW_ITEM_BLINK = 0x100,

	STUDIORENDER_SHADOWDEPTHTEXTURE = 0x200,

	STUDIORENDER_SSAODEPTHTEXTURE = 0x1000,

	STUDIORENDER_GENERATE_STATS = 0x8000,
};

class IStudioRender : public IAppSystem
{
public:
	virtual void unk0() = 0;
	virtual void unk1() = 0;
	virtual void unk2() = 0;
	virtual void unk3() = 0;
	virtual void unk4() = 0;
	virtual void unk5() = 0;
	virtual void unk6() = 0;
	virtual void unk7() = 0;
	virtual void unk8() = 0;
	virtual void unk9() = 0;
	virtual void unk10() = 0;
	virtual void unk11() = 0;
	virtual void unk12() = 0;
	virtual void unk13() = 0;
	virtual void unk14() = 0;
	virtual void unk15() = 0;
	virtual void unk16() = 0;
	virtual void unk17() = 0;
	virtual void unk18() = 0;
	virtual void unk19() = 0;
	virtual void unk20() = 0;
	virtual void unk21() = 0;
	virtual void unk22() = 0;
	virtual void unk23() = 0;
	virtual void unk24() = 0;
	virtual void unk25() = 0;
	virtual void unk26() = 0;
	virtual void unk27() = 0;
	virtual void unk28() = 0;
	virtual void DrawModel(void *pResults, const ModelRenderInfo_t& info,
		matrix3x4_t *pBoneToWorld, float *pFlexWeights, float *pFlexDelayedWeights, const Vector &modelOrigin, int flags = STUDIORENDER_DRAW_ENTIRE_MODEL) = 0;
	virtual void unk30() = 0;
	virtual void unk31() = 0;
	virtual void unk32() = 0;
	virtual void ForcedMaterialOverride(IMaterial *newMaterial, OverrideType_t nOverrideType = OVERRIDE_NORMAL) = 0;
};
