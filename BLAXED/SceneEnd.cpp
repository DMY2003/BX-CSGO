#include "SDK.h"
#include "Global.h"
#include "SceneEnd.h"

#include "ESP.h"
#include "Chams.h"

SceneEnd_t oSceneEnd = NULL;

void __fastcall SceneEnd(void* _this, void* edx)
{
	if (G::exit)
		return oSceneEnd(_this);

	if (I::pEngineClient->IsInGame() && cfg.Visual.enabled)
	{
		Chams::enablePlayersDraw = true;

		for (int i = 1; i <= I::pGlobals->maxClients; i++)
		{
			CBaseEntity* entity = I::pClientEntityList->GetClientEntity(i);

			if (ESP::Filter(entity) || entity->IsDormant())
				continue;
			
			Chams::DoPlayersChams(entity);			
		}
	}

	I::pModelRender->ForcedMaterialOverride(NULL);

	return oSceneEnd(_this);
}
