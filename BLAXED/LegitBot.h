#pragma once

#include "SDK.h"
#include "Global.h"

namespace LegitBot
{	

	bool UsingPistol()
	{
		int id = *((CBaseAttributableItem*)G::pWeapon)->GetItemDefinitionIndex();

		return id == WEAPON_DEAGLE || id == WEAPON_ELITE || id == WEAPON_FIVESEVEN || id == WEAPON_P250 || id == WEAPON_GLOCK || id == WEAPON_USP_SILENCER || id == WEAPON_CZ75A || id == WEAPON_TEC9 || id == WEAPON_REVOLVER;
	}

	bool UsingSniper()
	{
		int id = *((CBaseAttributableItem*)G::pWeapon)->GetItemDefinitionIndex();

		return id == WEAPON_AWP || id == WEAPON_SCAR20 || id == WEAPON_G3SG1 || id == WEAPON_SSG08;
	}

	bool UsingKnife()
	{
		if (G::pWeapon->GetModel())
		{
			char model[64];
			G::pWeapon->GetModel()->GetName(model);

			if (strstr(model, xorstr("knife")))
				return true;
		}

		return false;
	}

	bool UsingGrenade()
	{
		int id = *((CBaseAttributableItem*)G::pWeapon)->GetItemDefinitionIndex();

		return id == WEAPON_HEGRENADE || id == WEAPON_DECOY || id == WEAPON_INCGRENADE || id == WEAPON_MOLOTOV || id == WEAPON_C4 || id == WEAPON_FLASHBANG || id == WEAPON_SMOKEGRENADE;
	}

	bool UsingShotGun()
	{
		int id = *((CBaseAttributableItem*)G::pWeapon)->GetItemDefinitionIndex();

		return id == WEAPON_XM1014 || id == WEAPON_NOVA || id == WEAPON_SAWEDOFF || id == WEAPON_MAG7;
	}

	bool UsingRecoilFreeWepon()
	{
		int id = *((CBaseAttributableItem*)G::pWeapon)->GetItemDefinitionIndex();

		return id == WEAPON_DEAGLE || id == WEAPON_REVOLVER || UsingKnife() || UsingSniper() || UsingGrenade() || UsingShotGun();
	}

	bool CreateMove(CUserCmd* pCmd)
	{	
		return false;
	}
};
