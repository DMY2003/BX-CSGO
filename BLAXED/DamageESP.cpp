#include "SDK.h"
#include "Global.h"
#include "Render.h"
#include "MenuV2.h"

#include "DamageESP.h"

namespace DamageESP
{
	std::array<FloatingText, MAX_FLOATING_TEXTS> floatingTexts;
	int floatingTextsIdx = 0;

	void HandleGameEvent(IGameEvent* pEvent)
	{
		if (!cfg.Visual.damageESP)
			return;

		const char *name = pEvent->GetName();

		static Vector lastImpactPos = Vector(0, 0, 0);

		if (!strcmp(name, xorstr("player_hurt")))
		{
			float curTime = I::pGlobals->curtime;

			int userid = pEvent->GetInt(xorstr("userid"));
			int attackerid = pEvent->GetInt(xorstr("attacker"));
			int dmg_health = pEvent->GetInt(xorstr("dmg_health"));
			int hitgroup = pEvent->GetInt(xorstr("hitgroup"));

			CBaseEntity *entity = I::pClientEntityList->GetClientEntity(I::pEngineClient->GetPlayerForUserID(userid));
			CBaseEntity *attacker = I::pClientEntityList->GetClientEntity(I::pEngineClient->GetPlayerForUserID(attackerid));

			if (!entity || attacker != G::pLocal)
				return;

			FloatingText txt;
			txt.startTime = curTime;
			txt.hitgroup = hitgroup;
			txt.hitPosition = lastImpactPos;
			txt.damage = dmg_health;
			txt.randomIdx = Math::Random(-5, 5);
			txt.valid = true;

			floatingTexts[floatingTextsIdx++ % MAX_FLOATING_TEXTS] = txt;
		}
		else if (!strcmp(name, xorstr("bullet_impact")))
		{
			int userid = pEvent->GetInt(xorstr("userid"));
			float x = pEvent->GetFloat(xorstr("x"));
			float y = pEvent->GetFloat(xorstr("y"));
			float z = pEvent->GetFloat(xorstr("z"));

			CBaseEntity *entity = I::pClientEntityList->GetClientEntity(I::pEngineClient->GetPlayerForUserID(userid));

			if (!entity || entity != G::pLocal)
				return;

			lastImpactPos = Vector(x, y, z);
		}
	}

	void Draw()
	{
		if (!cfg.Visual.damageESP)
			return;

		float curTime = I::pGlobals->curtime;

		for (int i = 0; i < MAX_FLOATING_TEXTS; i++)
		{
			FloatingText *txt = &floatingTexts[i % MAX_FLOATING_TEXTS];

			if (!txt->valid)
				continue;

			float endTime = txt->startTime + 1.1f;

			if (endTime < curTime)
			{
				txt->valid = false;
				continue;
			}

			Vector screen;

			if (render->WorldToScreen(txt->hitPosition, screen))
			{
				float t = 1.0f - (endTime - curTime) / (endTime - txt->startTime);
				
				screen.y -= t * (35.0f);
				screen.x -= (float)txt->randomIdx * t * 3.0f;

				char msg[12];
				sprintf_s(msg, 12, "-%dHP", txt->damage);

				int width = render->GetTextWitdh(msg, render->fntVerdana10);

				render->DrawStringWithFont(render->fntVerdana10, screen.x - width / 2, screen.y - 11 + 1, D3D_COLOR_BLACK((int)((1.0f - t) * (float)cfg.Visual.espAlpha + 55.0f)), msg);
				render->DrawStringWithFont(render->fntVerdana10, screen.x - width / 2, screen.y - 11, D3D_COLOR_GREEN(cfg.Visual.espAlpha + 55), msg);
			}
		}
	}
};
