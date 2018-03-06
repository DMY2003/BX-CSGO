#include "SDK.h"
#include "Global.h"
#include "DrawModel.h"
#include "Chams.h"
#include "DoPostScreenSpaceEffects.h"

DrawModel_t oDrawModel = NULL;

void __fastcall DrawModel(void* _this, void* edx, void* results, DrawModelInfo_t& info, matrix3x4_t* pBoneToWorld, float* pFlexWeights, float* pFlexDelayedWeights, Vector& origin, int drawFlags)
{
	if (G::exit)
		return oDrawModel(_this, results, info, pBoneToWorld, pFlexWeights, pFlexDelayedWeights, origin, drawFlags);

	if (I::pEngineClient->IsInGame() && cfg.Visual.enabled)
	{
		if (Chams::DrawModel(oDrawModel, _this, results, info, pBoneToWorld, pFlexWeights, pFlexDelayedWeights, origin, drawFlags))
		{
			I::pModelRender->ForcedMaterialOverride(NULL);
			return;
		}
	}

	return oDrawModel(_this, results, info, pBoneToWorld, pFlexWeights, pFlexDelayedWeights, origin, drawFlags);
}
