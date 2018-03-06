#include "SDK.h"
#include "Global.h"
#include "GameEvents.h"
#include "HitMarkers.h"
#include "ResolverV2.h"
#include "NightMode.h"
#include "KillSounds.h"
#include "AntiAimV2.h"
#include "AimbotV2.h"
#include "DamageESP.h"

GameEvents *gameEvents = new GameEvents();

GameEvents::GameEvents()
{

}

void GameEvents::Register()
{
	I::pGameEventManager->AddListener(this, xorstr("player_death"), false);
	I::pGameEventManager->AddListener(this, xorstr("player_hurt"), false);
	I::pGameEventManager->AddListener(this, xorstr("round_start"), false);
	I::pGameEventManager->AddListener(this, xorstr("player_connect"), false);
	I::pGameEventManager->AddListener(this, xorstr("player_disconnect"), false);
	I::pGameEventManager->AddListener(this, xorstr("weapon_fire"), false);
	I::pGameEventManager->AddListener(this, xorstr("bullet_impact"), false);
	//I::pGameEventManager->AddListener(this, xorstr("round_start"), false);
	I::pGameEventManager->AddListener(this, xorstr("round_end"), false);
	I::pGameEventManager->AddListener(this, xorstr("client_disconnect"), false);
	I::pGameEventManager->AddListener(this, xorstr("cs_game_disconnected"), false);
	I::pGameEventManager->AddListener(this, xorstr("player_connect_full"), false);	
}

void GameEvents::Unregister()
{
	I::pGameEventManager->RemoveListener(this);
}

void GameEvents::HandleGameEvent(IGameEvent* event)
{
	if (!event)
		return;
		
	hitMarkers->HandleGameEvent(event);
	ResolverV2::HandleGameEvent(event);
	NightMode::HandleGameEvent(event);
	KillSounds::HandleGameEvent(event);
	antiAim->HandleGameEvent(event);
	DamageESP::HandleGameEvent(event);

	const char *name = event->GetName();

	if (strstr(name, xorstr("round_start")))
	{
		for (int i = 0; i < I::pEngineClient->GetMaxClients(); i++)
		{
			G::lastSeenTime[i] = 0;
		}
	}
	else if(strstr(name, xorstr("weapon_fire")))
	{

	}
}

int GameEvents::IndicateEventHandling()
{
	return EVENT_DEBUG_ID_INIT;
}
