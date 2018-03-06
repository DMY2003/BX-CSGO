#include "SDK.h"
#include "Global.h"
#include "SetupBones.h"
#include "Prediction.h"

SetupBones_t oSetupBones = NULL;

bool __fastcall SetupBones(void* ECX, void* EDX, matrix3x4_t *pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime)
{
	if (G::exit)
		return oSetupBones(ECX, pBoneToWorldOut, nMaxBones, boneMask, currentTime);

	if (!cfg.Rage.Resolver.noInterpolation)
		return oSetupBones(ECX, pBoneToWorldOut, nMaxBones, boneMask, currentTime);

	static ConVar* host_timescale = NULL;
	
	if (!host_timescale)
		host_timescale = I::pCVar->FindVar(xorstr("host_timescale"));

	if (ECX && ((IClientRenderable*)ECX)->GetIClientUnknown())
	{
		IClientNetworkable* pNetworkable = ((IClientUnknown*)ECX)->GetClientNetworkable();
		if (pNetworkable && pNetworkable->GetClientClass() && pNetworkable->GetClientClass()->m_ClassID == _CCSPlayer)
		{
			auto player = (CBaseEntity*)ECX;
			float OldCurTime = I::pGlobals->curtime;
			float OldRealTime = I::pGlobals->realtime;
			float OldFrameTime = I::pGlobals->frametime;
			float OldAbsFrameTime = I::pGlobals->absoluteframetime;
			float OldAbsFrameTimeStart = I::pGlobals->absoluteframestarttimestddev;
			float OldInterpAmount = I::pGlobals->interpolation_amount;
			int OldFrameCount = I::pGlobals->framecount;
			int OldTickCount = I::pGlobals->tickcount;

			I::pGlobals->curtime = player->GetSimulationTime();
			I::pGlobals->realtime = player->GetSimulationTime();
			I::pGlobals->frametime = I::pGlobals->interval_per_tick * host_timescale->GetFloat();
			I::pGlobals->absoluteframetime = I::pGlobals->interval_per_tick * host_timescale->GetFloat();
			I::pGlobals->absoluteframestarttimestddev = player->GetSimulationTime() - I::pGlobals->interval_per_tick * host_timescale->GetFloat();
			I::pGlobals->interpolation_amount = 0;
			I::pGlobals->framecount = TIME_TO_TICKS(player->GetSimulationTime());
			I::pGlobals->tickcount = TIME_TO_TICKS(player->GetSimulationTime());

			*(int*)((int)player + 236) |= 8; // IsNoInterpolationFrame = true (inlined check), not sure since offset is different everywhere >_<
			bool ret_value = oSetupBones(player, pBoneToWorldOut, nMaxBones, boneMask, I::pGlobals->curtime);
			*(int*)((int)player + 236) &= ~8; // (1 << 3)

			I::pGlobals->curtime = OldCurTime;
			I::pGlobals->realtime = OldRealTime;
			I::pGlobals->frametime = OldFrameTime;
			I::pGlobals->absoluteframetime = OldAbsFrameTime;
			I::pGlobals->absoluteframestarttimestddev = OldAbsFrameTimeStart;
			I::pGlobals->interpolation_amount = OldInterpAmount;
			I::pGlobals->framecount = OldFrameCount;
			I::pGlobals->tickcount = OldTickCount;
			return ret_value;
		}
	}

	return oSetupBones(ECX, pBoneToWorldOut, nMaxBones, boneMask, currentTime);
}
