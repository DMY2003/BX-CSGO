#include "SDK.h"
#include "Global.h"
#include "Configs.h"
#include "NameStealer.h"

NameStealer::NameStealer()
{
	started = false;
	delay = 0;
	idx = 0;
	memset(bkpName, 0, sizeof(bkpName));
}

void NameStealer::Tick()
{
	CBaseEntity *pLocal = G::pLocal;

	if (!cfg.Misc.nameStealer || !pLocal)
	{
		if (started)
		{
			SetName(nameStealer->bkpName);
			delay = 0;
			started = false;
		}

		return;
	}

	static ConVar *cvar_name = NULL;
	
	if (!cvar_name)
		cvar_name = I::pCVar->FindVar(xorstr("name"));

	if (!started)// || !strstr(cvar_name->GetString(), "\n"))
	{	
		strcpy(bkpName, cvar_name->GetString());

		char name[32];
		memset(name, 0, 32);

		strcpy(name, "\n");

		for (int i = 4; i < Math::Random(5, 31); i++)
			name[i] = '\n';

		name[31] = '\0';		

		SetName(name);

		delay++;

		if (delay == 10)
		{
			started = true;
			delay = 0;
		}

		return;
	}


	std::vector<string> names;

	int max = I::pEngineClient->GetMaxClients();
	for (int i = 0; i < max; i++)
	{
		CBaseEntity *entity = I::pClientEntityList->GetClientEntity(i);

		if (!entity) continue;
		if (entity == pLocal) continue;
		if (entity->GetTeam() == pLocal->GetTeam()) continue;
		if (entity->GetClientClass()->m_ClassID != _CCSPlayer) continue;

		player_info_t info;
		if (I::pEngineClient->GetPlayerInfo(i, &info))
		{
			if (strcmp(info.m_szPlayerName, xorstr("GOTV")) == 0)
				continue;

			names.push_back(info.m_szPlayerName);
		}
	}

	if (names.size() > 0)
	{
		idx = ((int)((float)I::pEngineClient->GetServerTick() / 8.0f))  % (int)names.size();

		char name[33];
		memset(name, 0, 33);

		int len = (int)names[idx].length();

		for (int i = 0; i < min(32, len); i++)
			name[i] = names[idx].c_str()[i];

		strcat(name, " ");

		name[31] = '\0';

		if (len > 23)
		{
			name[len] = '\n';			

			for (int i = len + 1; i < 31; i++)
				name[i] = Math::Random('A', 'Z');

			name[31] = '\0';
		}
		else
		{
			name[23] = '\n';

			for (int i = 23 + 1; i < 31; i++)
				name[i] = Math::Random('A', 'Z');

			name[31] = '\0';
		}

		SetName(name);
	}
}
