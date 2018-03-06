#pragma once

void MenuV2::DrawMiscTab()
{
	ImGui::Text(MENU_TRANSLATE("Cuidado", "CAUTION"));
	ImGui::Separator();

	//ImGui::Checkbox(xorstr("Bypass sv_cheats [Nao use cheats no console]"), &cfg.Misc.bypassSvCheats);
	ImGui::Checkbox(MENU_TRANSLATE("Untrusted", "UNTRUSTED"), &cfg.Rage.AntiAim.untrusted);
	ImGui::Checkbox(MENU_TRANSLATE("Airstuck (Key: C)", "AIRSTUCK (KEY: C)"), &cfg.Misc.airStuck);

	if (!cfg.Rage.AntiAim.untrusted)
		cfg.Rage.teleportF9 = false;

	ImGui::Text(xorstr("BUNNYHOP"));
	ImGui::Separator();

	ImGui::Checkbox(xorstr("BUNNYHOP"), &cfg.Misc.bunnyhop);
	ImGui::SameLine();
	ImGui::Checkbox(xorstr("SPOOF AUTOHOP"), &cfg.Misc.spoofAutoBunnyhop);
	ImGui::SameLine();
	ImGui::Checkbox(MENU_TRANSLATE("Parar na parede", "STOP AT WALL"), &cfg.Misc.buunyhopStopAtWall);
	ImGui::SameLine();
	ImGui::Checkbox(xorstr("AUTO STRAFE"), &cfg.Misc.autoStrafe);
	ImGui::SameLine();
	ImGui::Checkbox(xorstr("BUNNY AUTO DUCK"), &cfg.Misc.bunnyAutoDuck);
	ImGui::Checkbox(MENU_TRANSLATE("aceitar partida Automaticamente", "AUTO ACCEPT"), &cfg.Misc.autoaccept);


	if (cfg.Misc.autoStrafe)
	{
		ImGui::SameLine();
		ImGui::Checkbox(xorstr("FAST"), &cfg.Misc.fastAutoStrafe);
		ImGui::SliderFloat(xorstr("RETRACK"), &cfg.Misc.retrackSpeed, 0.5f, 10.0f);
		ImGui::SliderFloat(xorstr("DIVISOR"), &cfg.Misc.speedDivisor, 0.5f, 10.0f);
		ImGui::Checkbox(xorstr("BOX CSTRAFE"), &cfg.Misc.boxCircleStrafe);
	}

	ImGui::Text(xorstr("CLAN TAG"));
	ImGui::Separator();

	static vector<string> anims;
	static char animsStr[1024];

	if (anims.size() == 0)
	{
		anims.push_back(xorstr("NONE"));
		anims.push_back(xorstr("MATRIX 0-1"));
		anims.push_back(xorstr("GETZE.US"));
		anims.push_back(xorstr("BAIM"));
		anims.push_back(xorstr("SKEET.CC"));
		anims.push_back(xorstr("VALVE"));
		anims.push_back(xorstr("TESTE"));
		anims.push_back(xorstr("BLAXED"));
		anims.push_back(xorstr("GB HVH"));
		anims.push_back(xorstr("GAMESENSE"));

		VectorToArray(anims, animsStr);
	}

	ImGui::Text(MENU_TRANSLATE(u8"Animação", "ANIMATION"));
	ImGui::Combo(xorstr("##dummymisc0"), &cfg.Misc.clanTagAnimation, animsStr);

	ImGui::Separator();

	ImGui::Checkbox(MENU_TRANSLATE("Travar os outros", "AUTO BLOCK"), &cfg.Misc.autoBlock);

	ImGui::Separator();
		
	ImGui::Checkbox(MENU_TRANSLATE(u8"Ladrão de nomes", "NAME STEALER"), &cfg.Misc.nameStealer);

	ImGui::Separator();

	static char buf[512];
	static bool colorize;

	ImGui::InputText(xorstr("##dummymisc1"), buf, 512);

	bool changeName = ImGui::Button(MENU_TRANSLATE("Mudar nome", "CHANGE NAME"));

	ImGui::SameLine();
	//ImGui::Checkbox(xorstr("Vermelho"), &colorize);

	bool changeTag = ImGui::Button(MENU_TRANSLATE("Mudar tag", "CHANGE TAG"));

	if (changeName || changeTag)
	{
		string buf2(buf);

		replace(buf2, string("\\n"), string("\n"));
		replace(buf2, string("\\r"), string("\r"));

		if (changeTag)
		{
			SetClanTag(buf2.c_str());
		}
		else if (changeName)
		{
			if (colorize)
			{				
				SetName((" \x01\x0B\x01""\x02""" + buf2 + "\x01").c_str());
			}
			else
			{
				SetName(buf2.c_str());
			}
		}
	}

	ImGui::Separator();

	ImGui::Text(xorstr("OVERLAY"));
	ImGui::Checkbox(MENU_TRANSLATE("Espectadores", "SPECTATORS"), &cfg.Overlay.spectators);

	/*ImGui::Text(xorstr("Mudar Convar [untrusted]"));
	ImGui::Separator();

	if (cfg.Rage.AntiAim.untrusted)
	{
		static char buf[512];

		ImGui::InputText(xorstr("Convar: "), buf, 512);

		if (ImGui::Button(xorstr("Ativar")))
		{
			ConVar *convar = I::pCVar->FindVar(buf);

			if (convar)
			{
				convar->SetValue(1);
			}
		}
		if (ImGui::Button(xorstr("Desativar")))
		{
			ConVar *convar = I::pCVar->FindVar(buf);

			if (convar)
			{
				convar->SetValue(0);
			}
		}
	}*/
}
