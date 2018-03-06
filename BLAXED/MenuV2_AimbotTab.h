#pragma once

void MenuV2::DrawAimbotTab()
{
	ImGui::Checkbox(MENU_TRANSLATE("Ativar aimbot", "ENABLE"), &cfg.Aimbot.enabled);

	if (cfg.Aimbot.enabled)
	{
		const char *opt_MultiHitboxes[14] = { "HEAD", "PELVIS", "UPPER CHEST", "CHEST", "NECK", "LEFT FOREARM", "RIGHT FOREARM", "HANDS", "LEFT THIGH", "RIGHT THIGH", "LEFT CALF", "RIGHT CALF", "LEFT FOOT", "RIGHT FOOT" };
		const char *opt_AimHitboxSpot[] = { "HEAD", "NECK", "BODY", "PELVIS" };
		ImGui::Checkbox(MENU_TRANSLATE("Apenas yaw", "ONLY YAW"), &cfg.Aimbot.onlyX);
		ImGui::Text("HITSCAN");
		ImGui::Separator();
		ImGui::Columns(1, NULL, true);
		{
			ImGui::Checkbox("PRIORITIZE SELECTED HITBOX", &cfg.Rage.AutoFire.prioritize);
			ImGui::Checkbox(MENU_TRANSLATE("MULTI POINT", "MULTI POINT"), &cfg.Rage.AutoFire.multiPoint);
			if (cfg.Rage.AutoFire.multiPoint)
			{
				ImGui::Text(MENU_TRANSLATE("POINT SCALE", "POINT SCALE"));
				ImGui::SliderFloat(xorstr("##dummyaimbot3"), &cfg.Rage.AutoFire.pointScale, 0.1f, 1.0f, "%.1f");
			}
		}
		ImGui::Columns(1);
		ImGui::Separator();
		for (int i = 0; i < ARRAYSIZE(opt_MultiHitboxes); ++i)
		{
			ImGui::Selectable(opt_MultiHitboxes[i], &cfg.Rage.AutoFire.rage_multiHitboxes[i]);
		}
		ImGui::Columns(1);
		ImGui::Separator();
		vector<string> aimbotSpots;

		char aimbotSpotsStr[1024];

		memset(aimbotSpotsStr, 0, 1024);

		aimbotSpots.push_back(MENU_TRANSLATE("Barriga", "PELVIS"));
		aimbotSpots.push_back(MENU_TRANSLATE("Peito", "BODY"));
		aimbotSpots.push_back(MENU_TRANSLATE(u8"Pescoço", "NECK"));
		aimbotSpots.push_back(MENU_TRANSLATE(u8"Cabeça", "HEAD"));
		VectorToArray(aimbotSpots, aimbotSpotsStr);

		ImGui::Text(MENU_TRANSLATE(u8"Posição", "SELECT HITBOX"));
		ImGui::Combo(xorstr(""), &cfg.Aimbot.position, opt_AimHitboxSpot, 4);

		ImGui::Text(MENU_TRANSLATE("Aimbot FOV", "AIMBOT FOV"));
		ImGui::SliderFloat(xorstr("##dummyaimbot1"), &cfg.Aimbot.fov, 0, 180.0f, "%.1f");

		if (cfg.Aimbot.fov < 0.0f)
			cfg.Aimbot.fov = 0.0f;
		if (cfg.Aimbot.fov > 180.0f)
			cfg.Aimbot.fov = 180.0f;

		ImGui::Text(MENU_TRANSLATE("Tempo de espera", "DELAY"));
		ImGui::SliderInt(xorstr("##dummyaimbot5"), &cfg.Aimbot.recoverTime, 0, 1000);
		ImGui::Separator();

		ImGui::Checkbox(MENU_TRANSLATE("Usar suavidade [BETA]", "USE SMOOTH [BETA]"), &cfg.Aimbot.Smooth.enabled);
		if (cfg.Aimbot.Smooth.enabled)
		{
			ImGui::Text(MENU_TRANSLATE("Suavidade", "SMOOTH"));
			ImGui::SliderFloat(xorstr("##dummyaimbot4"), &cfg.Aimbot.Smooth.ammount, 1.0f, 6.0f);
		}

		if (cfg.Aimbot.silent)
			cfg.Aimbot.Smooth.enabled = false;

		if (cfg.Aimbot.Aimstep.enabled)
			cfg.Aimbot.onlyX = false;

		ImGui::Checkbox(MENU_TRANSLATE("Usar aimstep", "USE AIMSTEP"), &cfg.Aimbot.Aimstep.enabled);
		if (cfg.Aimbot.Aimstep.enabled)
		{
			ImGui::Text(MENU_TRANSLATE("Graus/tick", "DEGREE/TICK"));
			ImGui::SliderFloat(xorstr("##dummyaimbot6"), &cfg.Aimbot.Aimstep.ammount, 0.5f, 30.0f, "%.0f");
		}

		ImGui::Separator();

		ImGui::Checkbox(MENU_TRANSLATE("Silent", "SILENT"), &cfg.Aimbot.silent);
		ImGui::Separator();

		ImGui::Checkbox(MENU_TRANSLATE(u8"Aliados são alvos", "FRIENDLY FIRE"), &cfg.Aimbot.friendlyFire);
	}
}
