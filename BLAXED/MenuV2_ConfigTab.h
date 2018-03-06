#pragma once

void MenuV2::DrawConfigTab()
{
	static char cfgsStr[4096];
	static int index;
	static bool safeDelete = false, safeSave = false;
	static char name[128];

	static std::vector<string> vecLanguages;
	static char strLanguages[20];

	if (vecLanguages.size() == 0)
	{
		vecLanguages.push_back(xorstr("ENGLISH"));
		vecLanguages.push_back(xorstr("ROMANIAN"));

		memset(strLanguages, 0, 20);
		VectorToArray(vecLanguages, strLanguages);
	}

	memset(cfgsStr, 0, 4096);
	VectorToArray(cfgMgr->files, cfgsStr);
	
	ImGui::Text(MENU_TRANSLATE("Linguagem", "LANGUAGE"));
	ImGui::Combo(xorstr("##dummyconfigs0"), &cfg.language, strLanguages);

	ImGui::Separator();

	ImGui::Text("%s: %s [%s]", MENU_TRANSLATE(u8"Configuração atual", "CURRENT CONFIGURATION"), cfgMgr->actualConfig.c_str(), cfgMgr->GetConfigPath().c_str());

	if (ImGui::Combo(xorstr("##dummyconfigs1"), &index, cfgsStr))
	{
		cfgMgr->GetConfigFiles();
		strcpy_s(name, 128, cfgMgr->files[index].c_str());
	}

	ImGui::SameLine();

	if (ImGui::Button(MENU_TRANSLATE("Carregar", "LOAD")))
	{
		cfgMgr->ReadConfig(cfgMgr->files[index]);
		cfg.Rage.AntiAim.untrusted = false;
	}

	ImGui::Separator();

	ImGui::Checkbox(std::string(MENU_TRANSLATE("Deletar ", "DELETE ") + cfgMgr->files[index] + "?").c_str(), &safeDelete);
	ImGui::SameLine();
	ImGui::Checkbox(std::string(MENU_TRANSLATE("Salvar ", "SAVE ") + cfgMgr->files[index] + "?").c_str(), &safeSave);

	if (safeSave)
	{
		ImGui::Text(MENU_TRANSLATE(u8"Salvar configuração atual", "SAVE CURRENT CONFIGURATION"));
		ImGui::Separator();

		ImGui::Text(MENU_TRANSLATE("Nome: ", "NAME: "));
		ImGui::InputText(xorstr("##dummyconfigs2"), name, 128);

		if (ImGui::Button(MENU_TRANSLATE("Salvar", "SAVE")))
		{
			std::string n = std::string(name);

			if (n.find(".json") == -1)
				n += ".json";

			cfgMgr->WriteConfig(n);
			cfgMgr->GetConfigFiles();
		}
	}

	if (safeDelete)
	{
		ImGui::Text(MENU_TRANSLATE(u8"DELETAR configuração atual", "DELETE CURRENT CONFIGURATION"));
		ImGui::Separator();

		if (ImGui::Button(MENU_TRANSLATE("Deletar", "DELETE")))
		{
			cfgMgr->RemoveConfig(cfgMgr->files[index]);
			cfgMgr->GetConfigFiles();
			safeDelete = false;
		}
	}
}
