#pragma once

#include "SDK.h"
#include "Global.h"

namespace KillSounds
{
	void HandleGameEvent(IGameEvent *pEvent)
	{
		const char *name = pEvent->GetName();

		if (!strcmp(name, xorstr("player_hurt")))
		{
			int userid = pEvent->GetInt(xorstr("userid"));
			int attackerid = pEvent->GetInt(xorstr("attacker"));
			int dmg_health = pEvent->GetInt(xorstr("dmg_health"));

			CBaseEntity *entity = I::pClientEntityList->GetClientEntity(I::pEngineClient->GetPlayerForUserID(userid));
			CBaseEntity *attacker = I::pClientEntityList->GetClientEntity(I::pEngineClient->GetPlayerForUserID(attackerid));

			if (entity && attacker && attacker == G::pLocal)
			{
				if(cfg.Visual.hitmarkersound)
				I::pEngineClient->ExecuteClientCmd("play buttons\\arena_switch_press_02.wav"); // No other fitting sound. Probs should use a resource
			}
		}
	}
};

