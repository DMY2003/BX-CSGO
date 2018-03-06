#pragma once

#include "ResolverV2.h"

void MenuV2::DrawPlayerListTab()
{
	if (G::pLocal && cfg.Rage.Resolver.enabled)
	{
		static vector<string> pitch;
		static vector<string> yaw;
		static char pitchStr[1024];
		static char yawStr[1024];

		vector<string> players;
		vector<CBaseEntity*> entities;
		char playersStr[4096];

		if (pitch.size() == 0)
		{
			pitch.push_back(xorstr("NONE"));
			pitch.push_back(xorstr("FORCE DOWN"));
			pitch.push_back(xorstr("FORCE ZERO"));
			pitch.push_back(xorstr("FORCE UP"));

			VectorToArray(pitch, pitchStr);
		}

		if (yaw.size() == 0)
		{
			yaw.push_back(xorstr("NONE"));
			yaw.push_back(xorstr("INVERSE"));
			yaw.push_back(xorstr("INVERSE IF NO MOVE"));
			yaw.push_back(xorstr("LEFT"));
			yaw.push_back(xorstr("RIGHT"));

			VectorToArray(yaw, yawStr);
		}

		static int idx = 0;

		players.clear();
		players.push_back(MENU_TRANSLATE("Selecionar", "SELECT"));

		int max = I::pEngineClient->GetMaxClients();
		for (int i = 0; i < max; i++)
		{
			CBaseEntity *entity = I::pClientEntityList->GetClientEntity(i);

			if (!entity) continue;
			if (entity->GetTeam() == G::pLocal->GetTeam() && !cfg.Aimbot.friendlyFire) continue;
			if (entity->GetClientClass()->m_ClassID != _CCSPlayer) continue;

			player_info_t info;
			if (I::pEngineClient->GetPlayerInfo(entity->GetIndex(), &info))
			{
				char name[128];

				/*for (int j = 0; j < (int)strlen(info.m_szPlayerName) && j < 64; j++)
				{
					if (!(info.m_szPlayerName[j] >= 'A' && info.m_szPlayerName[j] <= 'Z' ||
						info.m_szPlayerName[j] >= 'a' && info.m_szPlayerName[j] <= 'z' ||
						info.m_szPlayerName[j] >= '0' && info.m_szPlayerName[j] <= '9'))
						info.m_szPlayerName[j] = '?';
				}

				info.m_szPlayerName[63] = '\0';*/

				if (!strcmp(info.m_szPlayerName, xorstr("GOTV")))
					continue;

				sprintf(name, "%2d - %s", entity->GetIndex(), MenuV2::SanitizeName(info.m_szPlayerName).c_str());

				players.push_back(name);
				entities.push_back(entity);
			}
		}

		memset(playersStr, 0, 1024);
		VectorToArray(players, playersStr);

		static int plPitch = 0;
		static int plYaw = 0;

		//ImGui::Text(MENU_TRANSLATE("Jogadores", "Players"));

		if (ImGui::Combo(xorstr("##dummyplist0"), &idx, playersStr))
		{
			if (idx != 0)
			{
				CBaseEntity *entity = entities[idx - 1];
				ResolverV2::CResolver& resolver = ResolverV2::GetResolver(entity->GetIndex());

				if (entity)
				{
					plPitch = resolver.plPitch;
					plYaw = resolver.plYaw;
				}
			}
		}

		if (ImGui::Button(MENU_TRANSLATE(u8"Limpar alterações", "CLEAR CHANGES")))
		{
			for (int i = 0; i < MAX_PLAYERS; i++)
			{
				CBaseEntity *entity = I::pClientEntityList->GetClientEntity(i);

				if (!entity) continue;

				ResolverV2::CResolver& resolver = ResolverV2::GetResolver(entity->GetIndex());

				resolver.plPitch = 0;
				resolver.plYaw = 0;
			}
		}

		if (idx >= (int)players.size())
			idx = (int)players.size() - 1;

		if (idx > 0)
		{
			CBaseEntity *entity = entities[idx - 1];
			ResolverV2::CResolver& resolver = ResolverV2::GetResolver(entity->GetIndex());

			if (entity)
			{
				player_info_t info;
				I::pEngineClient->GetPlayerInfo(entity->GetIndex(), &info);

				ImGui::Separator();
				ImGui::Text("%s [%s]", MenuV2::SanitizeName(info.m_szPlayerName).c_str(), info.m_szSteamID);

				ImGui::Combo(xorstr("PITCH"), &plPitch, pitchStr);
				ImGui::Combo(xorstr("YAW"), &plYaw, yawStr);

				if (ImGui::Button(MENU_TRANSLATE("Aplicar", "APPLY")))
				{
					resolver.plPitch = plPitch;
					resolver.plYaw = plYaw;
				}

				/*ImGui::Text(xorstr("Historia"));
				ImGui::Separator();

				float yaws[EntityResolver::MAX_HISTORY];
				float pitchs[EntityResolver::MAX_HISTORY];
				float chokeds[EntityResolver::MAX_HISTORY];

				for (int i = 0; i < EntityResolver::MAX_HISTORY; i++)
				{
				yaws[i] = resolving->history[i].yaw;
				pitchs[i] = resolving->history[i].pitch;
				chokeds[i] = (float)resolving->history[i].choked;

				Math::NormalizeYaw(yaws[i]);
				Math::NormalizeYaw(pitchs[i]);
				}

				ImGui::PlotLines(xorstr("HYaws"), yaws, EntityResolver::MAX_HISTORY, 0, "", -180.0f, 180.0f, ImVec2(0, 40));
				ImGui::PlotLines(xorstr("HPitch"), pitchs, EntityResolver::MAX_HISTORY, 0, "", -180.0f, 180.0f, ImVec2(0, 40));
				ImGui::PlotLines(xorstr("HChoke"), chokeds, EntityResolver::MAX_HISTORY, 0, "", 0.0f, 15.0f, ImVec2(0, 40));*/
			}
		}
	}
}
