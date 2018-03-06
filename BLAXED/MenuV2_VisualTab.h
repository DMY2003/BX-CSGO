#pragma once

void MenuV2::DrawVisualTab()
{
	vector<string> tabs;

	tabs.push_back(MENU_TRANSLATE("Visual", "VISUAL"));
	tabs.push_back(MENU_TRANSLATE("Cores", "COLORS"));

	static int selectedTab = 0;

	ImGui::TabLabels(tabs, selectedTab, true);
	{
		switch (selectedTab)
		{
		case 0: DrawVisualVisualTab(); break;
		case 1: DrawVisualColorsTab(); break;
		}
	}	
}

void MenuV2::DrawVisualVisualTab()
{

	ImGui::Checkbox(MENU_TRANSLATE("Ativar visuais", "ENABLE"), &cfg.Visual.enabled);
	ImGui::Columns(2);

	if (cfg.Visual.enabled)
	{
		ImGui::Text(MENU_TRANSLATE(u8"Transparência", "OPACITY"));
		ImGui::SliderInt(xorstr("##dummyvisuals0"), &cfg.Visual.espAlpha, 0, 200);

		ImGui::Checkbox(MENU_TRANSLATE("FOV do aimbot", "AIMBOT FOV"), &cfg.Visual.drawFOV);

		if (cfg.Visual.drawFOV)
		{
			ImGui::SameLine();
			ImGui::Checkbox(MENU_TRANSLATE(u8"Dinâmico", "DYNAMIC"), &cfg.Visual.dinamicFOV);
		}

		ImGui::Text(MENU_TRANSLATE(u8"FOV da camera", "CAMERA FOV"));
		ImGui::SliderFloat(xorstr("##dummyvisuals1"), &cfg.Misc.fovChanger, 0.0f, 120.0f);
		ImGui::Checkbox(MENU_TRANSLATE("Travar FOV", "LOCK FOV"), &cfg.Visual.lockFov);

		ImGui::Checkbox(MENU_TRANSLATE("Mira", "CROSSHAIR"), &cfg.Visual.drawCrosshair);
		ImGui::Checkbox(MENU_TRANSLATE("Exibir aliados", "SHOW TEAM"), &cfg.Visual.showTeam);
		ImGui::Checkbox(MENU_TRANSLATE("Exibir dormentes", "SHOW DORMANTS"), &cfg.Visual.dormantEsp);
		ImGui::Checkbox(MENU_TRANSLATE("Exibir no jogador", "SHOW IN LOCAL PLAYER"), &cfg.Visual.localPlayerVisuals);
		ImGui::Checkbox(MENU_TRANSLATE("Itens dropados", "DROPPED ITEMS"), &cfg.Visual.drawItems);
		ImGui::Checkbox(MENU_TRANSLATE("Nome de itens dropados", "DROPPED ITEMS NAMES"), &cfg.Visual.drawItemsNames);
		if (cfg.Visual.drawItemsNames) {
			ImGui::Combo("##ItemNames", &cfg.Visual.drawWeaponIcons, "Icon\0Text\0");
		}

		ImGui::Checkbox(MENU_TRANSLATE(u8"Traçado", "TRACERS"), &cfg.Visual.drawTracers);
		ImGui::Checkbox(MENU_TRANSLATE("Nome", "NAME"), &cfg.Visual.drawNames);
		ImGui::Checkbox(MENU_TRANSLATE("Arma", "WEAPON"), &cfg.Visual.drawWeapons);
		ImGui::Checkbox(MENU_TRANSLATE(u8"Mira Spread", "SPREAD CROSSHAIR"), &cfg.Visual.spreadCrosshair);
		ImGui::Checkbox(MENU_TRANSLATE(u8"Mostrar munição", "BULLETS"), &cfg.Visual.drawAmmo);


		ImGui::NextColumn();


		ImGui::Checkbox(MENU_TRANSLATE("Som do hitmarker", "HITMARKER SOUND"), &cfg.Visual.hitmarkersound);
		ImGui::Checkbox(MENU_TRANSLATE("Exibir dano", "SHOW DAMAGE"), &cfg.Visual.damageESP);
		ImGui::Checkbox(MENU_TRANSLATE("Vida", "HEALTH"), &cfg.Visual.drawHealth);
		ImGui::SameLine();
		ImGui::Checkbox(MENU_TRANSLATE(u8"Divisão", "DIVISISON"), &cfg.Visual.drawHealthDivision);
		//ImGui::Checkbox(MENU_TRANSLATE("Lagometer", "Lagometer"), &cfg.Visual.lagometer);
		//ImGui::Checkbox(MENU_TRANSLATE("Fill", "Fill"), &cfg.Visual.boxFill);
		ImGui::Checkbox(MENU_TRANSLATE("Caixa", "BOX"), &cfg.Visual.drawBox);
		ImGui::Checkbox(MENU_TRANSLATE("Esqueleto", "SKELETON"), &cfg.Visual.drawBones);



		ImGui::Checkbox(MENU_TRANSLATE("Glow", "GLOW"), &cfg.Visual.glow);
		ImGui::Checkbox(MENU_TRANSLATE("Chams", "CHAMS"), &cfg.Visual.chams);
		ImGui::Checkbox(MENU_TRANSLATE("Chams Ignore-Z", "CHAMS IGNORE-Z"), &cfg.Visual.wallChams);
		ImGui::Checkbox(MENU_TRANSLATE(u8"Chams para mãos", "HAND CHAMS"), &cfg.Visual.handChams);
		ImGui::Checkbox(MENU_TRANSLATE("Vidro", "GLASS"), &cfg.Visual.glassHands);
		ImGui::SameLine();
		ImGui::Checkbox(MENU_TRANSLATE("Escuro", "DARK"), &cfg.Visual.darkHandChams);
		ImGui::Checkbox(MENU_TRANSLATE("Chams HvH", "CHAMS HVH"), &cfg.Visual.hvhChams);


		ImGui::Checkbox(MENU_TRANSLATE("Noite", "NIGHT MODE"), &cfg.Visual.nightMode);
		ImGui::Checkbox(MENU_TRANSLATE(u8"Trajetória da granada", "GRENADE TRAJECTORY"), &cfg.Visual.granadePrediction);
		ImGui::Checkbox(MENU_TRANSLATE("Destravar FPS", "UNLOCK FPS"), &cfg.Misc.unlockFps);
		ImGui::Checkbox(MENU_TRANSLATE("Remover scope", "REMOVE SCOPE"), &cfg.Visual.noScope);
		ImGui::Checkbox(MENU_TRANSLATE("Remover pos processamento", "REMOVE POSTPROCESS"), &cfg.Visual.disablePostProcess);
		ImGui::Checkbox(MENU_TRANSLATE("Boost de FPS", "FPS BOOST"), &cfg.Visual.fpsBoost);

		ImGui::Checkbox(MENU_TRANSLATE("Debug autowall", "DEBUG AUTOWALL"), &cfg.Visual.debugAutowall);
		ImGui::Checkbox(MENU_TRANSLATE("Debug aimbot", "DEBUG AIMBOT"), &cfg.Visual.showAimPoint);
		ImGui::Checkbox(MENU_TRANSLATE("Debug multipoint", "DEBUG MULTIPOINT"), &cfg.Visual.debugMultipoint);

		//ImGui::Separator();
		ImGui::Checkbox(xorstr("SCREENSHOT CLEANER"), &cfg.Visual.screenshotCleaner);
	}
}

void MenuV2::DrawVisualColorsTab()
{
	{
		float col[3] = { min(255.f, cfg.Visual.colorGlow.r() / 255.f),  min(255.f, cfg.Visual.colorGlow.g() / 255.f),  min(255.f, cfg.Visual.colorGlow.b() / 255.f) };

		if (ImGui::ColorEdit3(MENU_TRANSLATE("Glow", "GLOW"), col))
			cfg.Visual.colorGlow = Color((BYTE)(col[0] * 255), (BYTE)(col[1] * 255), (BYTE)(col[2] * 255), 255);
	}

	{
		float col[3] = { min(255.f, cfg.Visual.colorChamsVisible.r() / 255.f),  min(255.f, cfg.Visual.colorChamsVisible.g() / 255.f),  min(255.f, cfg.Visual.colorChamsVisible.b() / 255.f) };

		if (ImGui::ColorEdit3(MENU_TRANSLATE(u8"Chams visível", "CHAMS VISIBLE"), col))
			cfg.Visual.colorChamsVisible = Color((BYTE)(col[0] * 255), (BYTE)(col[1] * 255), (BYTE)(col[2] * 255), 255);
	}

	{
		float col[3] = { min(255.f, cfg.Visual.colorChamsHidden.r() / 255.f),  min(255.f, cfg.Visual.colorChamsHidden.g() / 255.f),  min(255.f, cfg.Visual.colorChamsHidden.b() / 255.f) };

		if (ImGui::ColorEdit3(MENU_TRANSLATE(u8"Chams escondido", "CHAMS HIDDEN"), col))
			cfg.Visual.colorChamsHidden = Color((BYTE)(col[0] * 255), (BYTE)(col[1] * 255), (BYTE)(col[2] * 255), 255);
	}
}
