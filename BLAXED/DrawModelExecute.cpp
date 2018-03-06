#include "SDK.h"
#include "Global.h"
#include "DrawModelExecute.h"
#include "Chams.h"
#include "ESP.h"
#include "Hooks.h"
#include "ResolverV2.h"

DrawModelExecute_t oDrawModelExecute = nullptr;

void __stdcall DrawModelExecute(/*IMatRenderContext*/void* context, void* state, ModelRenderInfo_t& info, matrix3x4_t* pCustomBoneToWorld)
{
	if (G::exit)
	{
		oDrawModelExecute(I::pModelRender, context, state, info, pCustomBoneToWorld);
		return;
	}

	CBaseEntity *entity = I::pClientEntityList->GetClientEntity(info.entity_index);

	if ((cfg.Visual.chams || cfg.Visual.wallChams || cfg.Visual.handChams) && cfg.Visual.enabled)
	{
		if (G::pLocal && I::pEngineClient->IsInGame())
		{
			if (info.pModel)
			{
				//modelrender_hook->ReleaseVMT();
				bool chams = Chams::DrawModelExecute(oDrawModelExecute, context, state, info, pCustomBoneToWorld);
				//modelrender_hook->ApplyVMT();

				if (chams)
				{					
					I::pModelRender->ForcedMaterialOverride(NULL);

					return;
				}
			}
		}
	}	
	
	oDrawModelExecute(I::pModelRender, context, state, info, pCustomBoneToWorld);
}
