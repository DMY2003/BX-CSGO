#include "MainThread.h"
#include "Global.h"
#include "Hooks.h"
#include "Configs.h"
#include "licence.h"
#include "csgo_icons.h"
#include "Protection.h"

ConfigManager *cfgMgr = new ConfigManager();

void Unload()
{
	while (G::exit == false)
	{
		Sleep(500);
	}

	Hooks::ReleaseHooks();

	RemoveFontMemResourceEx(G::hWeaponIconFont);

	if (!::FreeLibrary(G::module))
		::UnmapViewOfFile(G::module);
}

DWORD WINAPI MainThread(LPVOID lpParam)
{

	cfgMgr->SetupConfigs();
	cfgMgr->GetConfigFiles();
	cfgMgr->WriteConfig(xorstr("_default.json"));

	DWORD dwFonts;
	G::hWeaponIconFont = AddFontMemResourceEx(csgo_icons_raw, sizeof(csgo_icons_raw), NULL, &dwFonts);

	Sleep(1000);

	Hooks::ApplyHooks();

	Unload();

	return 0;
}
