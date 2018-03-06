#include "SDK.h"
#include "Global.h"
#include "Render.h"
#include "ESP.h"
#include "AimbotV2.h"
#include "Hitbox.h"
#include "BunnyHop.h"
#include "HitMarkers.h"
#include "GranadePrediction.h"
#include "ResolverV2.h"
#include "MenuV2.h"
#include "SpreadCrosshair.h"
#include "DamageESP.h"
#include "LagCompensation.h"

bool ESP::canUseSetupBones = false;

wchar_t GetWeaponIcon(CBaseCombatWeapon *weapon);
void GetWeaponName(char *modelName, char *ret);

bool ESP::Filter(CBaseEntity *entity)
{
	if (!entity) return true;
	if (!G::pLocal) return true;
	if (entity->IsDormant() && !cfg.Visual.dormantEsp) return true;
	if (entity->GetLifeState() != LIFE_ALIVE || entity->GetHealth() <= 0) return true;
	if (entity->GetTeam() == G::pLocal->GetTeam() && !cfg.Visual.showTeam) return true;
	if (entity->GetTeam() != 2 && entity->GetTeam() != 3) return true;
	if (entity == G::pLocal && !I::pInput->m_fCameraInThirdPerson) return true;

	return false;
}

bool ESP::CanShowDormant(CBaseEntity *entity)
{
	if (!cfg.Visual.dormantEsp)
		return false;

	float fadeTime = 3.0f;

	if (fabsf(I::pGlobals->curtime - G::lastSeenTime[entity->GetIndex()]) <= fadeTime)
		return true;

	return false;
}

int ESP::GetDormantAlpha(CBaseEntity *entity)
{
	if (!entity->IsDormant())
		return cfg.Visual.espAlpha;

	float elapsed = I::pGlobals->curtime - G::lastSeenTime[entity->GetIndex()];
	float fadeTime = 3.0f;

	return min(cfg.Visual.espAlpha, 255 - (int)(elapsed * 255 / fadeTime));
}

void ESP::Render()
{
	if (!cfg.Visual.enabled)
		return;

	CBaseEntity *pLocal = G::pLocal;
	
	if (!render->setupComplete)
		return;

	if (!render->pDevice)
		return;

	if (!G::pLocal)
		return;

	if (!I::pEngineClient || !I::pClientEntityList)
		return;

	if (!I::pEngineClient->IsInGame() || !I::pEngineClient->IsConnected())
		return;

	//if (!G::canDrawESP)
	//	return;
	//G::canDrawESP = false;

	if (cfg.Visual.debugAutowall)
	{
		Vector s[2];

		for (int i = 0; i < 1024; i++)
		{
			if (render->WorldToScreen(G::dbg_AutoWall[i], s[0]) && render->WorldToScreen(G::dbg_AutoWall[(i - 1 + 1024) % 1024], s[1]))
			{
				render->DrawBoxFast(s[1].x - 1, s[1].y - 1, 2, 2, D3D_COLOR_WHITE(150));
				//render->DrawLineFast(s[0].x, s[0].y, s[1].x, s[1].y, D3D_COLOR_RED(130));
			}
		}
	}

	if (cfg.Visual.spreadCrosshair)
		spreadCrosshair->Draw();

	/*{
	QAngle viewAngles;
	Vector arrow, s[2], start, end;

	I::pEngineClient->GetViewAngles(viewAngles);

	start = pLocal->GetOrigin();
	end = start + Vector(pLocal->GetVelocity().x, pLocal->GetVelocity().y, 0) / 5.0f;

	if (render->WorldToScreen(start, s[0]) && render->WorldToScreen(end, s[1]))
	{
	render->DrawLineFast(s[0].x, s[0].y, s[1].x, s[1].y, D3D_COLOR_WHITE(255));
	}

	Math::AngleVectors(bunnyHop->circleStrafeDbg * -1, arrow);
	arrow.NormalizeInPlace();

	start = pLocal->GetOrigin();
	end = start + arrow * 30.0f;

	if (render->WorldToScreen(start, s[0]) && render->WorldToScreen(end, s[1]))
	{
	render->DrawLineFast(s[0].x, s[0].y, s[1].x, s[1].y, D3D_COLOR_WHITE(255));
	}

	if (bunnyHop->d_col != Vector(0, 0, 0) && render->WorldToScreen(start, s[0]) && render->WorldToScreen(bunnyHop->d_col, s[1]))
	{
	render->DrawLineFast(s[0].x, s[0].y, s[1].x, s[1].y, D3D_COLOR_GREEN(255));
	}

	if (bunnyHop->d_colL != Vector(0, 0, 0) && render->WorldToScreen(start, s[0]) && render->WorldToScreen(bunnyHop->d_colL, s[1]))
	{
	render->DrawLineFast(s[0].x, s[0].y, s[1].x, s[1].y, D3D_COLOR_RED(255));
	}

	if (bunnyHop->d_colR != Vector(0, 0, 0) && render->WorldToScreen(start, s[0]) && render->WorldToScreen(bunnyHop->d_colR, s[1]))
	{
	render->DrawLineFast(s[0].x, s[0].y, s[1].x, s[1].y, D3D_COLOR_BLUE(255));
	}
	}*/

	/*if (pLocal->IsAlive())
	{

	float headOffset = 25.0f;
	Vector head = pLocal->GetEyePosition() + Vector(0, 0, pLocal->GetDucked() ? 3.5f : -5.0f);
	Vector f;
	Math::AngleVectors(Vector(0, pLocal->GetLowerBodyYawTarget(), 0), f);
	f.Normalize();

	Vector s;

	if (render->WorldToScreen(head + f*headOffset, s))
	{
	render->DrawFilledRectangle(s.x-2.5f, s.y -2.5f, 5, 5, D3D_COLOR_WHITE(255));
	}
	}*/

	//DrawAimbotFOV(cfg.Aimbot.fov, false, D3D_COLOR_BLACK(125));
	//DrawAimbotFOV(cfg.Aimbot.fov, true, D3D_COLOR_BLACK(50));
	DrawAimbotFOV(cfg.LegitBot.fov, cfg.Visual.dinamicFOV, D3D_COLOR_BLACK(70));
	DrawCrossHair(6.5f, D3D_COLOR_BLACK(240));
	DrawCrossHair(4.5f, D3D_COLOR_WHITE(240));

	hitMarkers->Draw();
	DamageESP::Draw();

	int max = I::pEngineClient->GetMaxClients();

	for (int i = 0; i < max; i++)
	{
		if (!G::boneCacheESP[i].valid)
			continue;

		CBaseEntity *entity = I::pClientEntityList->GetClientEntity(i);

		if (!Filter(entity))
		{
			if (!entity->IsDormant())
				G::lastSeenTime[i] = I::pGlobals->curtime;

			if ((entity->GetOrigin() - pLocal->GetOrigin()).Length() < 80.0f && (entity == G::pLocal && !cfg.Visual.localPlayerVisuals)) return;
			if ((entity->GetOrigin() - Vector(10000, 10000, 10000)).Length() < 45.0f) return;
			if (entity->GetClientClass()->m_ClassID != _CCSPlayer) return;

			if (cfg.Visual.drawTracers) DrawTracer(entity);

			//if (pLocal->GetHealth() <= 0 || pLocal->GetLifeState() != LIFE_ALIVE) continue;

			if (cfg.Visual.drawNames) DrawName(entity);
			if (cfg.Visual.drawWeapons) DrawWeapon(entity);
			if (cfg.Visual.drawBox || cfg.Visual.boxFill) DrawBox(entity);
			if (cfg.Visual.drawHealth) DrawHealth(entity);
			if (cfg.Visual.drawBones) DrawBoneESP(entity);
			if (cfg.Visual.drawNames) DrawName(entity);

			if (cfg.Rage.Resolver.drawRecord)
			{
				auto records = &CMBacktracking::Get().m_LagRecord[entity->GetIndex()];

				if (records->size() < 3)
					return;

				Vector previous_screenpos = Vector(-1, 0, 0);
				for (auto record = records->begin(); record != records->end(); record++)
				{
					if (!CMBacktracking::Get().IsTickValid(TIME_TO_TICKS(record->m_flSimulationTime)))
						continue;

					Vector screen_pos;
					if (!render->WorldToScreen(record->HeadPosition, screen_pos))
						continue;

					if (previous_screenpos.x >= 0)
					{
						DWORD BacktrackColor;

						if (*record == CMBacktracking::Get().m_RestoreLagRecord[entity->GetIndex()].first)
							BacktrackColor = D3DCOLOR_ARGB(255, 255, 255, 0);
						else
							BacktrackColor = D3DCOLOR_ARGB(255, 255, 255, 255);

						render->DrawLine(screen_pos.x, screen_pos.y, previous_screenpos.x, previous_screenpos.y, 1.0f, false, BacktrackColor);
					}
					previous_screenpos = screen_pos;
				}
			}

			/*for (int i = 0; i < 20; i++)
			{
			Hitbox hitbox = Hitbox(i);

			if (!HitboxMgr::GetHitbox(entity, &hitbox))
			continue;

			Vector to = hitbox.points[0];
			Vector screen;

			if (render->WorldToScreen(to, screen))
			render->DrawString(screen.x, screen.y, D3D_COLOR_WHITE(255), "%d", i);
			}*/

			/*if (cfg.Visual.debugMultipoint && !entity->IsDormant() && entity->IsAlive())
			{
				std::vector<int> hitscan;

				aimbotV2->GetHitscanList(entity, hitscan);

				for (size_t i = 0; i < hitscan.size(); i++)
				{
					Hitbox h = Hitbox(hitscan[i]);
					Vector screen;

					if (!HitboxMgr::GetHitbox(entity, &h, aimbotV2->GetPointScaleValue(h.hitbox)))
						continue;

					/*if (render->WorldToScreen(h.points[0], screen))
					{
					render->DrawStringWithFont(render->fntLucidaConsole15, screen.x, screen.y + 1, D3D_COLOR_BLACK(255), "%d %s (%d)", h.hitbox, MenuV2::GetHitboxName(h.hitbox).c_str(), i);
					render->DrawStringWithFont(render->fntLucidaConsole15, screen.x, screen.y, D3D_COLOR_WHITE(255), "%d %s (%d)", h.hitbox, MenuV2::GetHitboxName(h.hitbox).c_str(), i);
					}*

					for (int j = 0; j <= aimbotV2->GetMultiPointValue(h.hitbox); j++)
					{
						if (render->WorldToScreen(h.points[j], screen))
						{
							render->DrawFilledRectangle(screen.x - 1, screen.y - 1, 2, 2, D3D_COLOR_WHITE(255));
							//render->DrawString(screen.x, screen.y + 1, D3D_COLOR_BLACK(255), "%d", j);
							//render->DrawString(screen.x, screen.y, D3D_COLOR_WHITE(255), "%d", j);
						}
					}
				}*/
			//}
		}
	}

	for (int i = I::pEngineClient->GetMaxClients(); i < I::pClientEntityList->GetHighestEntityIndex(); i++)
	{
		CBaseEntity *entity = I::pClientEntityList->GetClientEntity(i);//G::entityCache[i];

		if (!entity) return;
		if ((entity->GetOrigin() - Vector(0, 0, 0)).Length() < 10.0f) return;

		if (cfg.Visual.drawItems || cfg.Visual.drawItemsNames) DrawItem(entity);
	}

	if (cfg.Visual.granadePrediction)
	{
		if (GranadePrediction::pInstance->IsPredicted())
		{
			GranadePrediction::ShowPoint *points = GranadePrediction::pInstance->GetPoints();

			Vector screen[3];

			for (int i = 0; i < GranadePrediction::MAX_POINTS; i++)
			{
				if (!points[i].valid) return;

				if (render->WorldToScreen(points[i].start, screen[0]) && render->WorldToScreen(points[i].end, screen[1]))
				{
					render->DrawLineFast(screen[0].x, screen[0].y, screen[1].x, screen[1].y, D3D_COLOR_RED(cfg.Visual.espAlpha + 55));
					//render->DrawLineFast(screen[0].x, screen[0].y + 2, screen[1].x, screen[1].y + 2, D3D_COLOR_DARKRED(cfg.Visual.espAlpha));
					//render->DrawLineFast(screen[0].x, screen[0].y - 2, screen[1].x, screen[1].y - 2, D3D_COLOR_DARKRED(cfg.Visual.espAlpha));

					if (points[i].plane)
					{
						render->DrawLineFast(screen[0].x - 5, screen[0].y, screen[0].x + 5, screen[0].y, D3D_COLOR_WHITE(cfg.Visual.espAlpha));
					}
				}
			}
		}
	}

	if (cfg.Visual.showAimPoint)
	{
		if (aimbotV2->targetInfo.entity)
		{
			Vector screen;

			if (render->WorldToScreen(aimbotV2->targetInfo.position, screen))
			{
				render->DrawFilledRectangle(screen.x - 3, screen.y - 3, 6, 6, D3D_COLOR_WHITE(255));
				render->DrawFilledRectangle(screen.x - 2, screen.y - 2, 4, 4, D3D_COLOR_GREEN(255));
			}
		}

		for (int i = 0; i < cfg.Rage.AutoFire.hitchanceMaxSeed; i++)
		{
			Vector s;

			if (render->WorldToScreen(G::dbg_Spread[i].point, s))
			{
				render->DrawBoxFast(s.x - 1, s.y - 1, 2, 2, (G::dbg_Spread[i].hit ? D3D_COLOR_BLUE(70) : D3D_COLOR_RED(70)));
			}
		}
	}
}
void ESP::DrawAimbotFOV(float FOV, bool dinamic, D3DCOLOR color)
{
	if (!cfg.Visual.drawFOV)
		return;

	if (FOV > 90)
		return;

	CBaseEntity *pLocal = G::pLocal;

	if (pLocal->GetHealth() <= 0)
		return;

	//int circleFovRadius = (int)(Aimbot::fov * overlay->width / 360) * 2;

	float screenFov = 90;

	if (pLocal->GetFOV())
		screenFov = (float)pLocal->GetFOV();

	int circleFovRadius = (int)((tanf((float)DEG2RAD(FOV / 2)) / tanf((float)DEG2RAD(screenFov / 2))) * D3DX_PI * 180.0f);

	if (circleFovRadius > (int)render->viewPort.Width / 2)
		return;

	int x = render->viewPort.Width / 2;
	int y = render->viewPort.Height / 2;

	if (dinamic)
	{
		if (pLocal->GetFOV() == 0)
			PunchAnglesToScreen(render->viewPort.Height, render->viewPort.Width, 90, &x, &y);
		else
			PunchAnglesToScreen(render->viewPort.Height, render->viewPort.Width, pLocal->GetFOV(), &x, &y);
	}

	render->DrawCircleFast((float)(x), (float)(y), circleFovRadius * 2, circleFovRadius * 2, color);
}

void ESP::PunchAnglesToScreen(int height, int width, int FOV, int *resultX, int *resultY)
{
	CBaseEntity *pLocal = G::pLocal;

	*resultX = width / 2;
	*resultY = height / 2;
	float dy = height / (float)FOV;
	float dx = width / (float)FOV;

	if (!cfg.Rage.noRecoil)
	{
		*resultX -= (int)(dx*(pLocal->GetAimPunchAngles().y));
		*resultY += (int)(dy*(pLocal->GetAimPunchAngles().x));
	}
}

void ESP::DrawCrossHair(float size, D3DCOLOR color)
{
	if (!cfg.Visual.drawCrosshair)
		return;

	CBaseEntity *pLocal = G::pLocal;

	if (pLocal->GetHealth() <= 0)
		return;

	int x, y;

	if (pLocal->GetFOV() == 0)
		PunchAnglesToScreen(render->viewPort.Height, render->viewPort.Width, 90, &x, &y);
	else
		PunchAnglesToScreen(render->viewPort.Height, render->viewPort.Width, pLocal->GetFOV(), &x, &y);

	float halfSize = size / 2;

	float x1 = x - halfSize;
	float x2 = x + halfSize + 1;

	float y1 = y - halfSize;
	float y2 = y + halfSize + 1;

	//render->DrawLine((float)x1, (float)y, (float)x2, (float)y, 1.0f, false, color);
	//render->DrawLine((float)x, (float)y1, (float)x, (float)y2, 1.0f, false, color);

	render->DrawLineFast((float)x1, (float)y, (float)x2, (float)y, color);
	render->DrawLineFast((float)x, (float)y1, (float)x, (float)y2, color);
}

void ESP::DrawTracer(CBaseEntity *entity)
{
	bool enemy = entity->GetTeam() != G::pLocal->GetTeam() || cfg.Aimbot.friendlyFire;

	if (!enemy && !cfg.Aimbot.friendlyFire)
		return;

	if (entity->IsDormant())
		return;

	Vector screen, screen2;

	Vector to = entity->GetRenderOrigin();

	if (render->WorldToScreen(to, screen))
	{
		float x = (float)render->viewPort.Width / 2.0f;
		float y = (float)render->viewPort.Height;

		Vector ang = Math::AngleBetweenXY(Vector(x, y, 0), Vector(screen.x, screen.y, 0));

		Vector end;
		Math::AngleVectors(ang, end);
		end.Normalize();

		float max = (float)render->viewPort.Height / 4.0f;

		float distX = min(max, fabsf(x - screen.x));
		float distY = min(max, fabsf(y - screen.y));

		render->DrawLineFast(x, y, x + end.x * max(0, distX - 3), y + end.y * max(0, distY - 3), D3D_COLOR_WHITE(cfg.Visual.espAlpha));
		render->DrawCircleFast(x + end.x * distX, y + end.y * distY, 3, 9, D3D_COLOR_WHITE(cfg.Visual.espAlpha));
	}
}

void ESP::DrawItem(CBaseEntity *entity)
{
	if (!entity->GetModel())
		return;

	char modelName[512];
	char name[512];

	memset(modelName, 0, 512);
	memset(name, 0, 512);

	entity->GetModel()->GetName(modelName);
	GetWeaponName(modelName, name);

	D3DCOLOR color = D3D_COLOR_GRAY(cfg.Visual.espAlpha + 55);

	bool colored = false;
	bool c4 = false;

	if (strstr(modelName, xorstr("weap")))
	{
		if (!entity->IsDormant())
			color = D3D_COLOR_WHITE(cfg.Visual.espAlpha + 55);

		colored = true;
	}

	if (strstr(modelName, xorstr("ied_dropped"))
		|| strstr(modelName, xorstr("c4")))
	{
		if (!entity->IsDormant())
			color = D3D_COLOR_RED(255);

		c4 = true;
		colored = true;
	}

	if (strstr(modelName, xorstr("flash"))
		|| strstr(modelName, xorstr("decoy"))
		|| strstr(modelName, xorstr("molotov"))
		|| strstr(modelName, xorstr("grenade"))
		|| strstr(modelName, xorstr("knife")))
	{
		if (!entity->IsDormant())
			color = D3D_COLOR_WHITE(cfg.Visual.espAlpha + 55);

		colored = true;
	}

	if (!colored)
		return;

	float flTimeToExplosion = 0;
	char explosion[64];
	D3DCOLOR colorC4 = D3D_COLOR_BLACK(255);

	strcpy(explosion, "");

	if (c4)
	{
		flTimeToExplosion = (entity->GetC4Blow() - I::pGlobals->curtime);

		sprintf(explosion, "%.2fs", flTimeToExplosion);

		if (!(entity->GetBombTicking() && flTimeToExplosion > 0 && flTimeToExplosion < 60))
			flTimeToExplosion = 0;

		if (flTimeToExplosion < 10.0f)
			colorC4 = D3D_COLOR_YELLOW(cfg.Visual.espAlpha + 55);
		else if (flTimeToExplosion < 5.0f)
			colorC4 = D3D_COLOR_RED(cfg.Visual.espAlpha + 55);
		else
			colorC4 = D3D_COLOR_WHITE(cfg.Visual.espAlpha + 55);
	}

	float height, width, x, y;

	if (GetItemBox(entity, x, y, width, height))
	{
		if (cfg.Visual.drawItems)
		{
			render->DrawBoxFast(x, y, height, width, color);
		}

		if (cfg.Visual.drawItemsNames)
		{
			CBaseAttributableItem *item = (CBaseAttributableItem*)entity;
			CBaseCombatWeapon* Weapon = (CBaseCombatWeapon*)entity;

			switch (cfg.Visual.drawWeaponIcons)
			{
			case 0:
			{
				// TO DO CLEAN UP / ADD SOME WEAPONS
				if (strstr(entity->GetClientClass()->m_pNetworkName, "CWeapon"))
				{
					int da = cfg.Visual.espAlpha + 55;//GetDormantAlpha(entity) + 55;
					float _x = x;
					float _y = y;

					DWORD outlineColor = D3D_COLOR_BLACK(da / 2);
					DWORD textColor = D3D_COLOR_WHITE(da);

					wchar_t weapIcon[2] = { GetWeaponIcon(Weapon), '\0' };

					float x_ = x + (width / 2) - render->GetTextWitdh(name, render->fntWeaponIcon) / 2;
					float y_ = y - 15;

					render->DrawStringWithFontW(render->fntWeaponIcon, x_, y_ + 1, outlineColor, L"%s", weapIcon);
					render->DrawStringWithFontW(render->fntWeaponIcon, x_, y_, textColor, L"%s", weapIcon);

					char ammo[16];
					sprintf_s(ammo, 16, "%d/%d", Weapon->GetClip1(), Weapon->GetClip2());

					x_ = x + (width / 2) - render->GetTextWitdh(ammo, render->fntVerdana9) / 2;
					y_ = y + height + 3;

					render->DrawStringWithFont(render->fntVerdana9, x_, y_, textColor, "%s", ammo);
				}

				if (!strstr(name, xorstr("dropped")))
				{
					if (c4 && flTimeToExplosion > 0)
					{
						float x_ = x + (width / 2) - (strlen(explosion) * 10 / 4);
						float y_ = y - 25;

						render->DrawString(x_, y_, colorC4, "%s", explosion);
					}
				}
			}
			break;
			case 1:
			{
				if (!strstr(name, xorstr("dropped")))
				{
					float x_ = x + (width / 2) - render->GetTextWitdh(name, render->font) / 2;
					float y_ = y - 15;

					D3DCOLOR color2 = entity->IsDormant() ? D3D_COLOR_BLACK(cfg.Visual.espAlpha) : D3D_COLOR_WHITE(cfg.Visual.espAlpha + 55);

					render->DrawString(x_, y_ + 1, D3D_COLOR_BLACK(cfg.Visual.espAlpha), "%s", name);
					render->DrawString(x_, y_, color2, "%s", name);

					if (c4 && flTimeToExplosion > 0)
					{
						float x_ = x + (width / 2) - (strlen(explosion) * 10 / 4);
						float y_ = y - 25;

						render->DrawString(x_, y_, colorC4, "%s", explosion);
					}
				}
			}

			}

		}
	}

	if (cfg.Visual.drawItemsNames || cfg.Visual.drawItems)
	{
		if (c4 && flTimeToExplosion > 0)
		{
			float x_ = (float)render->viewPort.Width / 2;
			float y_ = (float)render->viewPort.Height / 2;

			x_ = max(0, x_ - render->GetTextWitdh(explosion, render->font) / 2);
			y_ = max(0, y_ + 15);

			render->DrawString(x_, y_ + 1, D3D_COLOR_BLACK(cfg.Visual.espAlpha), "%s", explosion);
			render->DrawString(x_, y_, colorC4, "%s", explosion);
		}
	}
}

void ESP::DrawName(CBaseEntity *entity)
{
	if (entity->IsDormant() && !CanShowDormant(entity) || entity == G::pLocal)
		return;

	float height, width, x, y;

	if (GetPlayerBox(entity, x, y, width, height))
	{
		int da = GetDormantAlpha(entity) + 55;
		float _x = x;
		float _y = y;

		DWORD outlineColor = D3D_COLOR_BLACK(da / 2);
		DWORD textColor = D3D_COLOR_WHITE(da);

		player_info_t info;
		I::pEngineClient->GetPlayerInfo(entity->GetIndex(), &info);

		std::string name = MenuV2::SanitizeName(info.m_szPlayerName);

		_x += (width / 2) - render->GetTextWitdh((char*)name.c_str(), render->font) / 2;
		_y -= 8;

		render->DrawString(_x, _y + 1, outlineColor, "%s", name.c_str());
		render->DrawString(_x, _y, textColor, "%s", name.c_str());
	}
}

void ESP::DrawWeapon(CBaseEntity *entity)
{
	bool enemy = entity->GetTeam() != G::pLocal->GetTeam() || cfg.Aimbot.friendlyFire;

	if (!enemy && !cfg.Aimbot.friendlyFire)
		return;

	if (entity->IsDormant() && !CanShowDormant(entity))
		return;

	CBaseCombatWeapon *w = entity->GetWeapon();

	if (!w)
		return;

	if (!w->GetModel())
		return;

	float height, width, x, y;

	if (GetPlayerBox(entity, x, y, width, height))
	{
		int da = GetDormantAlpha(entity) + 55;
		float boxHeight = 5;
		float padd = 1;
		float _x = x;
		float _y = y;

		DWORD outlineColor = D3D_COLOR_BLACK(da / 2);
		DWORD textColor = D3D_COLOR_WHITE(da);
		DWORD textColorScoped = D3D_COLOR_ORANGE(da);
		DWORD textColorEmpty = D3D_COLOR_YELLOW(da);

		WeaponInfo_t *weapInfo = &G::weapInfoCache[entity->GetIndex()];

		int green = 255;

		if (weapInfo->m_iMaxClip1 != 0)
		{
			green = min(255, w->GetClip1() * 225 / weapInfo->m_iMaxClip1);
		}

		//DWORD lineColor = D3DCOLOR_ARGB(da, 255, min(weapInfo->m_iMaxClip1, (w->GetClip1() * weapInfo->m_iMaxClip1) / 255), 170);
		DWORD lineColor = D3DCOLOR_ARGB(da, 255, green, 0);

		if (entity->IsDormant())
		{
			lineColor = D3D_COLOR_WHITE(da);
			textColor = D3D_COLOR_WHITE(da);
			textColorScoped = D3D_COLOR_WHITE(da);
			textColorEmpty = D3D_COLOR_WHITE(da);
		}

		if (cfg.Visual.drawAmmo)
		{
			if (w->GetClip1() != 255)
			{
				char ammo[20];
				sprintf_s(ammo, 20, "%d/%d", w->GetClip1(), w->GetClip2());

				if (cfg.Visual.drawHealth && weapInfo->m_iMaxClip1 != 0)
				{
					_y += height + padd;

					float barWidth = ((w->GetClip1() * width) / weapInfo->m_iMaxClip1);

					render->DrawFilledRectangle(_x, _y, boxHeight, width, outlineColor);
					render->DrawFilledRectangle(_x + 1, _y + 1, boxHeight - 2, barWidth - 2, lineColor);

					if (cfg.Visual.drawHealthDivision)
					{
						for (int i = 1; i < 4; i++)
							render->DrawLineFast(_x + i * (width / 4.0f), _y + 1, _x + i * (width / 4.0f), _y + boxHeight - 1, outlineColor);
					}
				}

				_x = x + width + padd;
				_y = y;

				if (w->GetClip1() <= 10)
				{
					if (w->GetClip1() > 0)
					{
						render->DrawStringWithFont(render->fntVerdana9, _x, _y + 1, outlineColor, "%s", ammo);
						render->DrawStringWithFont(render->fntVerdana9, _x, _y, textColor, "%s", ammo);
					}
					else
					{
						render->DrawStringWithFont(render->fntVerdana9, _x, _y + 1, outlineColor, "%s", xorstr("EMPTY"));
						render->DrawStringWithFont(render->fntVerdana9, _x, _y, textColorEmpty, "%s", xorstr("EMPTY"));
					}

					_y += 7;
				}

				if (entity->IsScoped())
				{
					render->DrawStringWithFont(render->fntVerdana9, _x, _y + 1, outlineColor, "%s", xorstr("SCOPE"));
					render->DrawStringWithFont(render->fntVerdana9, _x, _y, textColorScoped, "%s", xorstr("SCOPE"));
					_y += 7;
				}

				if (!entity->IsDormant() && !Prediction::TickIsValid(TIME_TO_TICKS(entity->GetSimulationTime())))
				{
					render->DrawStringWithFont(render->fntVerdana9, _x, _y + 1, outlineColor, "%s", xorstr("AIRSTUCK"));
					render->DrawStringWithFont(render->fntVerdana9, _x, _y, textColor, "%s", xorstr("AIRSTUCK"));
					_y += 7;
				}

				if (cfg.Rage.Resolver.enabled)
				{
					if (!ResolverV2::Filter(entity))
					{
						ResolverV2::CResolver& resolver = ResolverV2::GetResolver(entity->GetIndex());

						if (resolver.IsValid())
						{
							std::string destinationstring;

							switch (resolver.lastResolverMethod)
							{
							case ResolverV2::BREAKING_LBY:
								destinationstring = xorstr("Breaking LBY");
								break;
							case ResolverV2::FAKE_WALK:
								destinationstring = xorstr("Fake Walk");
								break;
							case ResolverV2::MOVING_LBY:
								destinationstring = xorstr("Moving LBY");
								break;
							case ResolverV2::DUCKING_RESOLVE:
								destinationstring = xorstr("Ducking Resolving");
								break;
							case ResolverV2::STANDING_RESOLVE:
								destinationstring = xorstr("Standing Resolving");
								break;
							case ResolverV2::STATIC_YAWDIFF:
								destinationstring = xorstr("Static Yaw Diff");
								break;
							case ResolverV2::DELTA_CHANGE:
								destinationstring = xorstr("Delta Change");
								break;
							case ResolverV2::LBY_CHANGE:
								destinationstring = xorstr("LBY Change");
								break;
							case ResolverV2::STANDARD_LBY:
								destinationstring = xorstr("Standard LBY");
								break;
							case ResolverV2::STATIC_YAW:
								destinationstring = xorstr("Real Static Yaw");
								break;
							case ResolverV2::STATIC_YAW_FAKE:
								destinationstring = xorstr("Static Yaw Diff");
								break;
							case ResolverV2::BODYEYE_DELTA:
								destinationstring = xorstr("Body Eye Delta");
								break;
							case ResolverV2::LBY_TICKS:
								destinationstring = xorstr("Body Eye Ticks");
								break;
							case ResolverV2::LBY_CALCULATE:
								destinationstring = xorstr("LBY Calculate");
								break;
							case ResolverV2::LBY_UPDATE:
								destinationstring = xorstr("LBY Update");
								break;
							case ResolverV2::LAST_LBYUPDATE:
								destinationstring = xorstr("Last LBY Update");
								break;
							case ResolverV2::LBY_OVERRIDE:
								destinationstring = xorstr("LBY OverRide");
								break;

							}

							render->DrawStringWithFont(render->fntVerdana9, _x, _y + 1, outlineColor, "%s", destinationstring.c_str());
							render->DrawStringWithFont(render->fntVerdana9, _x, _y, textColor, "%s", destinationstring.c_str());
							_y += 7;
						}
					}
				}
			}
		}

		if (cfg.Visual.drawWeapons)
		{
			wchar_t weapIcon[2] = { GetWeaponIcon(w), '\0' };

			_x = x + (width - render->GetTextWitdhW(weapIcon, render->fntWeaponIcon)) / 2;
			_y = y + height + 7 + 1;

			render->DrawStringWithFontW(render->fntWeaponIcon, _x, _y + 1, outlineColor, L"%s", weapIcon);
			render->DrawStringWithFontW(render->fntWeaponIcon, _x, _y, textColor, L"%s", weapIcon);
		}


		if (cfg.Rage.Resolver.enabled)
		{
			if (!ResolverV2::Filter(entity))
			{
				ResolverV2::CResolver& resolver = ResolverV2::GetResolver(entity->GetIndex());

				if (resolver.IsValid())
				{
					_x = x + width;
					_y = y;

					if (cfg.Visual.lagometer)
					{
						_x += 3;

						int choked = (I::pGlobals->tickcount + 1 - TIME_TO_TICKS(entity->GetSimulationTime() - Prediction::GetLatencyIn()));

						//render->DrawString(_x, _y, color, "%d", (int)choked);

						float max = (y + height) - _y;
						float ch = (max * (15 - choked)) / 15;



						_x += 2;
					}

					_x += 4;

					std::string state = "";

					if (resolver.lastLby)
						state = std::string(xorstr("FAKE (")) + std::to_string((int)Math::NormalizeYawInPlace(resolver.oLby - entity->GetEyeAngles().y)) + std::string(xorstr(")"));
					else
						state = xorstr("");



					_y += 8;
				}
			}
		}
	}
}
void ESP::DrawHealth(CBaseEntity *entity)
{
	if (entity->IsDormant() && !CanShowDormant(entity))
		return;

	bool enemy = entity->GetTeam() != G::pLocal->GetTeam() || cfg.Aimbot.friendlyFire;

	if (!enemy && !cfg.Aimbot.friendlyFire)
		return;

	float height, width, x, y;

	if (GetPlayerBox(entity, x, y, width, height))
	{
		int da = GetDormantAlpha(entity) + 55;
		float boxWidth = 5;
		float padd = 1;
		float _x = x;
		float _y = y;

		DWORD lineColor = D3DCOLOR_ARGB(da, 153, min(255, entity->GetHealth() * 225 / 100), 0);
		DWORD outlineColor = D3D_COLOR_BLACK(da / 2);
		DWORD textColor = D3D_COLOR_WHITE(da);

		if (entity->IsDormant())
		{
			lineColor = D3D_COLOR_WHITE(da);
		}

		if (cfg.Visual.drawHealth)
		{
			_x -= padd;

			float healthBarHeight = entity->GetHealth() * height / 100.0f;

			render->DrawFilledRectangle(_x - boxWidth, _y, height, boxWidth, outlineColor);
			render->DrawFilledRectangle(_x - boxWidth + 1, _y + (height - healthBarHeight) + 1, healthBarHeight - 2, boxWidth - 2, lineColor);

			if (cfg.Visual.drawHealthDivision)
			{
				for (int i = 1; i < 4; i++)
					render->DrawLineFast(_x - 1, _y + i * (height / 4.0f), _x - boxWidth + 1, _y + i * (height / 4.0f), outlineColor);
			}

			if (entity->GetHealth() < 100)
			{
				char health[6];
				sprintf_s(health, 6, "%d", entity->GetHealth());

				_x -= render->GetTextWitdh(health, render->fntVerdana9) - 4;
				_y = y + (height - healthBarHeight) + 1;

				render->DrawStringWithFont(render->fntVerdana9, _x, _y + 1, outlineColor, "%s", health);
				render->DrawStringWithFont(render->fntVerdana9, _x, _y, textColor, "%s", health);
			}
		}
	}
}

void ESP::DrawBox(CBaseEntity *entity)
{
	if (entity == G::pLocal)
		return;

	if (entity->IsDormant() && !CanShowDormant(entity))
		return;

	float height, width, x, y;

	bool enemy = entity->GetTeam() != G::pLocal->GetTeam() || cfg.Aimbot.friendlyFire;

	if (!enemy)
		return;

	if (GetPlayerBox(entity, x, y, width, height))
	{
		x += 1;
		y += 1;
		width -= 2;
		height -= 2;

		int da = GetDormantAlpha(entity);

		DWORD boxColor = D3D_COLOR_WHITE(da / 2);
		DWORD outlineColor = D3D_COLOR_BLACK(da / 3);

		if (!enemy)
			boxColor = D3D_COLOR_BLACK(da);

		if (entity->IsDormant())
		{
			boxColor = D3D_COLOR_WHITE(da);
		}

		if (cfg.Visual.drawBox)
		{
			/*if (!ResolverV2::Filter(entity) && cfg.Rage.Resolver.enabled)
			{
			ResolverV2::CResolver& resolver = ResolverV2::GetResolver(entity->GetIndex());

			if (resolver.IsValid())
			{
			if (resolver.lbyUpdated)
			boxColor = D3D_COLOR_GREEN(da / 2);
			else if (resolver.fakingLby)
			boxColor = D3D_COLOR_RED(da / 2);
			else
			boxColor = D3D_COLOR_YELLOW(da / 2);
			}
			}*/

			/*render->DrawBox(x - 1, y - 1, height + 2 , width + 2, 1.0f, outlineColor);
			render->DrawBox(x, y, height, width, 1.0f, boxColor);
			render->DrawBox(x + 1, y + 1, height - 2, width - 2, 1.0f, outlineColor);*/

			render->DrawBoxFast(x - 1, y - 1, height + 2, width + 2, outlineColor);
			render->DrawBoxFast(x, y, height, width, boxColor);
			render->DrawBoxFast(x + 1, y + 1, height - 2, width - 2, outlineColor);
		}
	}
}

bool ESP::GetPlayerBox(CBaseEntity *entity, float &x, float &y, float &width, float &height, Vector offset)
{
	Vector top, down, s[2];

	Vector adjust = Vector(0, 0, -16) * entity->GetDuckAmount();

	down = G::absOriginCache[entity->GetIndex()] - Vector(0, 0, 1);//entity->GetRenderOrigin(); //entity->GetAbsOrigin();//entity->GetNetworkOrigin();
	top = down + Vector(0, 0, 72) + offset + adjust;

	if (render->WorldToScreen(top, s[1]) && render->WorldToScreen(down, s[0]))
	{
		Vector delta = s[1] - s[0];

		height = fabsf(delta.y);
		width = height / 2.0f;

		x = s[1].x - (width / 2);
		y = s[1].y;

		return true;
	}

	return false;
}

bool ESP::GetItemBox(CBaseEntity *entity, float &x, float &y, float &width, float &height, Vector offset)
{
	Vector top, down, s[2];

	down = entity->GetAbsOrigin();
	top = down + Vector(0, 0, 2.25f) + offset;

	if (render->WorldToScreen(top, s[1]) && render->WorldToScreen(down, s[0]))
	{
		Vector delta = s[1] - s[0];

		height = fabsf(delta.y);
		width = height * 2;

		x = s[1].x - (width / 2);
		y = s[1].y;

		return true;
	}

	return false;
}

void ESP::DrawBoneESP(CBaseEntity *entity)
{
	int id = entity->GetIndex();

	if (!G::boneCacheESP[id].valid)
		return;

	if (entity->IsDormant()) //&& !cfg.Visual.dormantEsp)
		return;

	if (entity == G::pLocal)
		return;

	bool enemy = entity->GetTeam() != G::pLocal->GetTeam() || cfg.Aimbot.friendlyFire;

	{
		if (enemy)
		{
			DWORD color = D3D_COLOR_WHITE((int)(cfg.Visual.espAlpha / 1.2));

			if (!enemy)
				color = D3D_COLOR_BLACK((int)(cfg.Visual.espAlpha / 1.2));

			if (entity->IsDormant())
				color = D3D_COLOR_WHITE(cfg.Visual.espAlpha);

			DrawBone(entity, G::boneCacheESP[id].pBoneToWorldOut, color);
		}
	}
}

void ESP::DrawBone(CBaseEntity *entity, matrix3x4_t *pBoneToWorldOut, D3DCOLOR color)
{
	studiohdr_t* pStudioModel = I::pModelInfo->GetStudioModel((model_t*)entity->GetModel());

	if (pStudioModel)
	{
		int num = 0;

		for (int i = 0; i < pStudioModel->numbones; i++)
		{
			mstudiobone_t* pBone = pStudioModel->pBone(i);

			if (!pBone)
				continue;

			if (!(pBone->flags & 256) || pBone->parent == -1)
				continue;

			Vector vBonePos1;
			if (!render->WorldToScreen(Vector(pBoneToWorldOut[i][0][3], pBoneToWorldOut[i][1][3], pBoneToWorldOut[i][2][3]), vBonePos1))
				continue;

			Vector vBonePos2;
			if (!render->WorldToScreen(Vector(pBoneToWorldOut[pBone->parent][0][3], pBoneToWorldOut[pBone->parent][1][3], pBoneToWorldOut[pBone->parent][2][3]), vBonePos2))
				continue;

			//render->DrawLine((float)vBonePos1.x, (float)vBonePos1.y, (float)vBonePos2.x, (float)vBonePos2.y, 1.5f, true, color);
			render->DrawLineFast((float)vBonePos1.x, (float)vBonePos1.y, (float)vBonePos2.x, (float)vBonePos2.y, color);
		}
	}
}

// https://www.unknowncheats.me/forum/counterstrike-global-offensive/169881-csgo-matWeapon-icons-ttf-font.html
wchar_t GetWeaponIcon(CBaseCombatWeapon *weapon)
{
	int code = 0xE000 + max(0, min(WEAPON_KNIFE_PUSH, *((CBaseAttributableItem*)weapon)->GetItemDefinitionIndex()));

	return (wchar_t)(code);
}

void GetWeaponName(char *modelName, char *ret)
{
	strcpy(ret, modelName);

	if (strstr(modelName, XorStr("none"))) { strcpy(ret, XorStr("WEAPON_NONE")); return; }
	if (strstr(modelName, XorStr("deagle"))) { strcpy(ret, XorStr("DEAGLE")); return; }
	if (strstr(modelName, XorStr("elite"))) { strcpy(ret, XorStr("DUAL")); return; }
	if (strstr(modelName, XorStr("fiveseven"))) { strcpy(ret, XorStr("FIVESEVEN")); return; }
	if (strstr(modelName, XorStr("glock"))) { strcpy(ret, XorStr("GLOCK")); return; }
	if (strstr(modelName, XorStr("p228"))) { strcpy(ret, XorStr("P228")); return; }
	if (strstr(modelName, XorStr("usp"))) { strcpy(ret, XorStr("USP")); return; }
	if (strstr(modelName, XorStr("ak47"))) { strcpy(ret, XorStr("AK-47")); return; }
	if (strstr(modelName, XorStr("aug"))) { strcpy(ret, XorStr("AUG")); return; }
	if (strstr(modelName, XorStr("awp"))) { strcpy(ret, XorStr("AWP")); return; }
	if (strstr(modelName, XorStr("famas"))) { strcpy(ret, XorStr("FAMAS")); return; }
	if (strstr(modelName, XorStr("g3sg1"))) { strcpy(ret, XorStr("G3SG1")); return; }
	if (strstr(modelName, XorStr("galil"))) { strcpy(ret, XorStr("GALIL")); return; }
	if (strstr(modelName, XorStr("galilar"))) { strcpy(ret, XorStr("GALILAR")); return; }
	if (strstr(modelName, XorStr("m249"))) { strcpy(ret, XorStr("M249")); return; }
	if (strstr(modelName, XorStr("m3"))) { strcpy(ret, XorStr("M3")); return; }
	if (strstr(modelName, XorStr("m4a1"))) { strcpy(ret, XorStr("M4A1")); return; }
	if (strstr(modelName, XorStr("mac10"))) { strcpy(ret, XorStr("MAC10")); return; }
	if (strstr(modelName, XorStr("mp5navy"))) { strcpy(ret, XorStr("MP5 NAVY")); return; }
	if (strstr(modelName, XorStr("p90"))) { strcpy(ret, XorStr("P90")); return; }
	if (strstr(modelName, XorStr("scout"))) { strcpy(ret, XorStr("SCOUT")); return; }
	if (strstr(modelName, XorStr("sg550"))) { strcpy(ret, XorStr("SG550")); return; }
	if (strstr(modelName, XorStr("sg552"))) { strcpy(ret, XorStr("SG552")); return; }
	if (strstr(modelName, XorStr("tmp"))) { strcpy(ret, XorStr("TMP")); return; }
	if (strstr(modelName, XorStr("ump45"))) { strcpy(ret, XorStr("UMP45")); return; }
	if (strstr(modelName, XorStr("xm1014"))) { strcpy(ret, XorStr("XM1014")); return; }
	if (strstr(modelName, XorStr("bizon"))) { strcpy(ret, XorStr("BIZON")); return; }
	if (strstr(modelName, XorStr("mag7"))) { strcpy(ret, XorStr("MAG7")); return; }
	if (strstr(modelName, XorStr("negev"))) { strcpy(ret, XorStr("NEGEV")); return; }
	if (strstr(modelName, XorStr("sawedoff"))) { strcpy(ret, XorStr("SAWEDOFF")); return; }
	if (strstr(modelName, XorStr("tec9"))) { strcpy(ret, XorStr("TEC9")); return; }
	if (strstr(modelName, XorStr("taser"))) { strcpy(ret, XorStr("TASER")); return; }
	if (strstr(modelName, XorStr("hkp2000"))) { strcpy(ret, XorStr("HKP2000")); return; }
	if (strstr(modelName, XorStr("mp7"))) { strcpy(ret, XorStr("MP7")); return; }
	if (strstr(modelName, XorStr("mp9"))) { strcpy(ret, XorStr("MP9")); return; }
	if (strstr(modelName, XorStr("nova"))) { strcpy(ret, XorStr("NOVA")); return; }
	if (strstr(modelName, XorStr("p250"))) { strcpy(ret, XorStr("P250")); return; }
	if (strstr(modelName, XorStr("scar17"))) { strcpy(ret, XorStr("SCAR17")); return; }
	if (strstr(modelName, XorStr("scar20"))) { strcpy(ret, XorStr("SCAR-20")); return; }
	if (strstr(modelName, XorStr("sg556"))) { strcpy(ret, XorStr("SG556")); return; }
	if (strstr(modelName, XorStr("ssg08"))) { strcpy(ret, XorStr("SCOUT")); return; }
	if (strstr(modelName, XorStr("knifegg"))) { strcpy(ret, XorStr("KNIFE")); return; }
	if (strstr(modelName, XorStr("knife"))) { strcpy(ret, XorStr("KNIFE")); return; }
	if (strstr(modelName, XorStr("flashbang"))) { strcpy(ret, XorStr("FLASH")); return; }
	if (strstr(modelName, XorStr("hegrenade"))) { strcpy(ret, XorStr("HE")); return; }
	if (strstr(modelName, XorStr("fraggrenade"))) { strcpy(ret, XorStr("HE")); return; }
	if (strstr(modelName, XorStr("smokegrenade"))) { strcpy(ret, XorStr("SMOKE")); return; }
	if (strstr(modelName, XorStr("molotov"))) { strcpy(ret, XorStr("MOLOTOV")); return; }
	if (strstr(modelName, XorStr("decoy"))) { strcpy(ret, XorStr("DECOY")); return; }
	if (strstr(modelName, XorStr("incgrenade"))) { strcpy(ret, XorStr("MOLOTOV")); return; }
	if (strstr(modelName, XorStr("incendiary"))) { strcpy(ret, XorStr("MOLOTOV")); return; }
	if (strstr(modelName, XorStr("grenade"))) { strcpy(ret, XorStr("GRENADA")); return; }
	if (strstr(modelName, XorStr("c4"))) { strcpy(ret, XorStr("C4")); return; }
	if (strstr(modelName, XorStr("flash"))) { strcpy(ret, XorStr("FLASH")); return; }
	if (strstr(modelName, XorStr("decoy"))) { strcpy(ret, XorStr("DECOY")); return; }
	if (strstr(modelName, XorStr("pist_223"))) { strcpy(ret, XorStr("USP-S")); return; }
	if (strstr(modelName, XorStr("p2000"))) { strcpy(ret, XorStr("P2000")); return; }
	if (strstr(modelName, XorStr("v_ied"))) { strcpy(ret, XorStr("C4")); return; }
	if (strstr(modelName, XorStr("ied_dropped"))) { strcpy(ret, XorStr("C4")); return; }

	char *str = strtok(modelName, xorstr("/"));

	while (str != NULL)
	{
		strcpy(ret, str);
		str = strtok(NULL, xorstr("/"));
	}
}