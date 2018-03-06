#pragma once

void MenuV2::DrawLegitBotTab()
{
	ImGui::Checkbox(MENU_TRANSLATE("Ativar LegitBot", "ENABLE"), &cfg.LegitBot.enabled);

	if (cfg.LegitBot.enabled)
	{
		ImGui::Checkbox(MENU_TRANSLATE("AimBot", "AIMBOT"), &cfg.LegitBot.aimbot);

		if (cfg.LegitBot.aimbot)
		{
			ImGui::Text(MENU_TRANSLATE("Aimbot FOV", "AIMBOT FOV"));
			ImGui::SliderFloat(xorstr("##dummylegitbot0"), &cfg.LegitBot.fov, 0, 35.0f, "%.1f");

			ImGui::Text(MENU_TRANSLATE("Rifle logsmooth", "RIFLE LOGSMOOTH"));
			ImGui::SliderFloat(xorstr("##dummylegitbot1"), &cfg.LegitBot.smooth, 0, 1.0f, "%.1f");

			ImGui::Text(MENU_TRANSLATE("Aimbot delay [s]", "AIMBOT DELAY [S]"));
			ImGui::SliderFloat(xorstr("##dummyaimbot2"), &cfg.LegitBot.delay, 0, 1.0f, "%.1f");

			ImGui::Checkbox(MENU_TRANSLATE("AimLock", "AIMLOCK"), &cfg.LegitBot.aimLock);

			ImGui::Checkbox(MENU_TRANSLATE("Backtrack", "BACKTRACK"), &cfg.LegitBot.backtrack);
			ImGui::Checkbox(MENU_TRANSLATE("Fogo amigo", "FRIENDLY FIRE"), &cfg.LegitBot.friendlyFire);
			ImGui::Checkbox(MENU_TRANSLATE("Silent", "SILENT"), &cfg.LegitBot.silent);
		}

		ImGui::Separator();

		ImGui::Checkbox(MENU_TRANSLATE("TriggerBot", "TRIGGETBOT"), &cfg.LegitBot.triggerBot);

		if (cfg.LegitBot.triggerBot)
		{
			ImGui::Text(MENU_TRANSLATE("Trigger delay [s]", "TRIGGER DELAY [S]"));
			ImGui::SliderFloat(xorstr("##dummyaimbot3"), &cfg.LegitBot.triggerDelay, 0, 0.3f);
		}
	}
}
