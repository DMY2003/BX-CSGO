#pragma once

class IVModelRender
{
public:
	virtual void DrawModel(void* results, ModelRenderInfo_t& info, matrix3x4_t* bone_to_world, float* flex_weights, float* flex_delayed_weights, const Vector& model_origin, int flags) = 0;
	virtual void ForcedMaterialOverride(IMaterial *newMaterial, OverrideType_t nOverrideType = OVERRIDE_NORMAL, int unknown = NULL) = 0;
	virtual void vfunc2() = 0;
	virtual void vfunc3() = 0;
	virtual void vfunc4() = 0;
	virtual void vfunc5() = 0;
	virtual void vfunc6() = 0;
	virtual void vfunc7() = 0;
	virtual void vfunc8() = 0;
	virtual void vfunc9() = 0;
	virtual void vfunc10() = 0;
	virtual void vfunc11() = 0;
	virtual void vfunc12() = 0;
	virtual void vfunc13() = 0;
	virtual void vfunc14() = 0;
	virtual void vfunc15() = 0;
	virtual void vfunc16() = 0;
	virtual void vfunc17() = 0;
	virtual void vfunc18() = 0;
	virtual void vfunc19() = 0;
	virtual void vfunc20() = 0;
	virtual void DrawModelExecute(void* ctx, void *state, const ModelRenderInfo_t &pInfo, matrix3x4_t *pCustomBoneToWorld = NULL) = 0;
};
