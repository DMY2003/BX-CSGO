#include "SDK.h"
#include "Global.h"
#include "DoPostScreenSpaceEffects.h"
#include "ESP.h"

DoPostScreenSpaceEffects_t oDoPostScreenSpaceEffects = NULL;

void ApplyGlow()
{
	for (int i = 0; i < I::pGlowObjectManager->size; ++i)
	{
		CGlowObjectManager::GlowObjectDefinition_t* obj = &I::pGlowObjectManager->m_GlowObjectDefinitions[i];
		CBaseEntity* objEntity = obj->GetEntity();

		if (obj->IsEmpty() || !objEntity)
			continue;

		if (ESP::Filter(objEntity) || objEntity->GetTeam() == G::pLocal->GetTeam() || (objEntity == G::pLocal && !cfg.Visual.localPlayerVisuals))
		{
			//obj->Set(Color(0, 0, 0, 0));
			continue;
		}

		if (objEntity->GetClientClass()->m_ClassID == _CCSPlayer)
		{
			obj->Set(Color((uint8_t)cfg.Visual.colorGlow[0], (uint8_t)cfg.Visual.colorGlow[1], (uint8_t)cfg.Visual.colorGlow[2], (uint8_t)(cfg.Visual.espAlpha + 55) / 1.7f));
		}
		else
		{
			if (cfg.Visual.drawItems)
			{
				obj->Set(Color((uint8_t)cfg.Visual.colorGlow[0], (uint8_t)cfg.Visual.colorGlow[1], (uint8_t)cfg.Visual.colorGlow[2], (uint8_t)(cfg.Visual.espAlpha + 55) / 1.7f));
			}
		}
	}
}

void __fastcall DoPostScreenSpaceEffects(void* _this, void* edx, CViewSetup* setup)
{
	if (G::exit)
		return oDoPostScreenSpaceEffects(_this, setup);

	if (cfg.Visual.glow && cfg.Visual.enabled)
	{		
		ApplyGlow();
	}

	oDoPostScreenSpaceEffects(_this, setup);
}

