#include "SDK.h"
#include "Global.h"
#include "Configs.h"
#include "FrameStageNotify.h"
#include "NoVisualRecoil.h"
#include "ViewAntiAim.h"
#include "Render.h"
#include "Chams.h"
#include "AntiAimV2.h"
#include "NightMode.h"
#include "ResolverV2.h"
#include "MainThread.h"
#include "LagCompensation.h"
FrameStageNotify_t oFrameStageNotify = NULL;

void __fastcall FrameStageNotify(void* ecx, void* edx, ClientFrameStage_t Stage)
{
	//return oFrameStageNotify(ecx, Stage); 

	if (G::exit)
	{
		NightMode::Remove();

		return oFrameStageNotify(ecx, Stage);
	}

	//G::pLocal = I::pClientEntityList->GetClientEntity(I::pEngineClient->GetLocalPlayer()); 
	if (!G::pLocal)
		return oFrameStageNotify(ecx, Stage);

	CBaseEntity *pLocal = G::pLocal;
	CUserCmd *pCmd = G::pCmd;

	NightMode::FrameStageNotify(Stage);

	if (!I::pEngineClient->IsInGame() || !pLocal || !pCmd)
		return oFrameStageNotify(ecx, Stage);

	antiAim->FrameStageNotify(Stage);

	if (Stage == FRAME_NET_UPDATE_START)
	{
		for (int i = 1; i < I::pGlobals->maxClients; i++)
		{
			CBaseEntity *player = I::pClientEntityList->GetClientEntity(i);

			if (!player)
				continue;

			if (player == G::pLocal)
				continue;

			if (player->GetTeam() == G::pLocal->GetTeam())
				continue;

			VarMapping_t *map = player->VarMapping();
			if (map)
			{
				if (cfg.Rage.Resolver.noInterpolation)
				{
					map->m_nInterpolatedEntries = 0;
				}
				else
				{
					if (map->m_nInterpolatedEntries == 0)
						map->m_nInterpolatedEntries = 6;
				}
			}
		}
	}

	ResolverV2::FrameStageNotifyPreReal(Stage);
	//Backtrack::FrameStageNotify(Stage); 

	if (pLocal->GetLifeState() == LIFE_ALIVE && pLocal->GetHealth() > 0)
	{
		if (Stage == FRAME_NET_UPDATE_POSTDATAUPDATE_START)
		{
			CMBacktracking::Get().CacheInfo();

		}
		ViewAntiAim(Stage, 0);
		NoVisualRecoil(Stage, 0);

		oFrameStageNotify(ecx, Stage);

		NoVisualRecoil(Stage, 1);
		ViewAntiAim(Stage, 1);
	}
	else
	{
		oFrameStageNotify(ecx, Stage);
	}

	ResolverV2::FrameStageNotifyPostReal(Stage);
	//Backtrack::FrameStageNotifyPostReal(Stage); 

	return oFrameStageNotify(ecx, Stage);
}