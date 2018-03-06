#include "SDK.h"
#include "Global.h"
#include "Configs.h"
#include "FakeLag.h"
#include "AntiAimV2.h"
#include "Prediction.h"
#include "AimbotV2.h"

FakeLag::FakeLag()
{
	isLagCompensated = true;
	choked = 0;
}

struct SendInfo
{
	SendInfo(int _tick, Vector _origin)
	{
		tick = _tick;
		origin = _origin;
	}

	int tick;
	Vector origin;
};

std::vector<SendInfo> lastSendInfo;

void FakeLag::Tick(bool &chokePacket)
{
	CUserCmd *pCmd = G::pCmd;
	CBaseEntity *pLocal = G::pLocal;

	INetChannel *net = (INetChannel*)I::pEngineClient->GetNetChannelInfo();

	float speed = G::pLocal->GetVelocity().Length();

	int cfgMin = cfg.Rage.FakeLag.min;
	int cfgMax = cfg.Rage.FakeLag.max;
	int cfgEdge = cfg.Rage.FakeLag.edge;
	bool cfgRandom = cfg.Rage.FakeLag.random;
	int cfgAir = cfg.Rage.FakeLag.air;
	bool cfgEnabled = cfg.Rage.FakeLag.enabled;
	bool cfgAdaptive = cfg.Rage.FakeLag.adaptive;

	bool slowMotion = cfg.Rage.AntiAim.enabled && cfg.Rage.AntiAim.slowMotionTicks != 0 && pCmd->buttons & IN_WALK && G::pLocal->CheckOnGround() && G::pLocal->GetVelocity().Length() < 500;
	bool airStuck = cfg.Misc.airStuck &&  pLocal->GetMoveType() != MOVETYPE_LADDER && GetAsyncKeyState('C') != 0;

	if (!cfgEnabled || aimbotV2->IsUsingRevolver())
	{
		cfgMin = 1;
		cfgMax = 1;
		cfgAir = 0;
		cfgEdge = 0;
	}
	
	// slowmotion slow motion
	if (slowMotion)
	{
		cfgRandom = false;
		cfgMin = 1;
		cfgMax = 1;
		cfgEdge = 0;
		cfgRandom = false;
		cfgAir = 0;
		cfgEnabled = true;
		cfgAdaptive = false;

		cfgMin = cfg.Rage.AntiAim.slowMotionTicks;
		cfgMax = cfg.Rage.AntiAim.slowMotionTicks;
	}

	if (airStuck)
	{
		cfgRandom = false;
		cfgMin = 1;
		cfgMax = 1;
		cfgEdge = 0;
		cfgRandom = false;
		cfgAir = 0;
		cfgEnabled = true;
		cfgAdaptive = false;
	}

	/*if (G::pLocal->CheckOnGround() && choked < 14 && speed < 1 && cfg.Rage.AntiAim.lowerBodyStyle != 0)
	{
		if (antiAim->lbyUpdatedLastTick)
		{
			cfgMin = 1;
			cfgMax = 1;
		}
		else if (antiAim->lbyUpdate)
		{
			cfgMin = 14;
			cfgMax = 14;
		}
	}*/

	static bool lastInAir = false;

	bool onGround = pLocal->CheckOnGround();

	if (lastInAir && onGround)
	{
		cfgRandom = false;
		cfgMin = 14;
		cfgMax = 14;
		cfgEdge = 0;
		cfgRandom = false;
		cfgAir = 0;
		cfgEnabled = true;
		cfgAdaptive = false;
	}

	lastInAir = !onGround;

	static bool flip = false;
	bool edge = false;
	bool air = false;

	if (cfgAir > 0)
	{
		if (!onGround)
		{
			air = true;
		}
	}

	/*if (cfg.Rage.FakeLag.edge > 0)
	{
		trace_t trace;

		for (float i = -180.0f; i <= 180.0f; i += 30.0f)
		{
			antiAim->TraceWorld(Vector(0, i, 0), G::pLocal->GetEyePosition() - Vector(0, 0, 32), 40.0f, trace);

			if (trace.fraction != 1.0f)
			{
				edge = true;
				break;
			}
		}
	}*/

	if (cfgAdaptive && cfgEnabled)
	{
		int min = cfgMin;
		int max = cfgMax;

		float distPerTick = speed * I::pGlobals->interval_per_tick;
		max = min((int)std::ceilf(TELEPORT_DISTANCE / distPerTick) + 1, MAX_CHOKE - 1);
				
		max = min(cfgMax, max);
		
		if (cfgRandom)
			max = flip ? max : Math::Random(max / 2, MAX_CHOKE - 1);

		if (speed < 1.0f)
		{
			max = 1;
			min = 1;
		}

		if (choked >= min && choked >= max)
		{
			//printf("choked: %d\n", choked);
			chokePacket = false;
			choked = 0;
			flip = !flip;
		}
		else
		{
			chokePacket = true;
			choked++;
		}
	}
	else
	{
		int min = cfgMin;
		int max = cfgMax;

		if (edge && speed > 0) min = cfgEdge;
		if (air) min = cfgAir;

		if (choked >= min)
		{
			chokePacket = false;
			choked = 0;
		}
		else
		{
			chokePacket = true;
			choked++;
		}
	}
	
	if (slowMotion)
	{
		if (chokePacket)
		{

		}
		else
		{
			pCmd->forwardmove = 0;
			pCmd->sidemove = 0;
		}
	}

	if (airStuck)
	{
		//if (!pLocal->CheckOnGround())
		{
			//if (!chokePacket)
			{
				pCmd->tick_count = INT_MAX;				
			}
		}
	}
}

void FakeLag::EndTick(bool chokePacket)
{
	CUserCmd *pCmd = G::pCmd;
	CBaseEntity *pLocal = G::pLocal;

	if (!chokePacket)
	{
		lastSendInfo.insert(lastSendInfo.begin(), SendInfo(I::pGlobals->tickcount, pLocal->GetOrigin()));

		if (lastSendInfo.size() > 11)
			lastSendInfo.pop_back();
	}
	
	int lagCompensatedTicks = 0;
	
	if (lastSendInfo.size() != 0)
	{
		Vector curOrigin = lastSendInfo.begin()->origin;

		for (auto it = ++lastSendInfo.begin(); it != lastSendInfo.end(); ++it)
		{
			Vector diff = curOrigin - it->origin;
			curOrigin = it->origin;

			if (diff.Length2D() <= TELEPORT_DISTANCE)
			{
				lagCompensatedTicks++;
			}
			else
			{
				break;
			}
		}
	}

	isLagCompensated = lagCompensatedTicks > 0;

	
	/*if (!chokePacket)
	{
		isLagCompensated = lagCompensatedTicks > 0;

		lastSendInfo.insert(lastSendInfo.begin(), SendInfo(I::pGlobals->tickcount, pLocal->GetOrigin()));

		if (lastSendInfo.size() > 11)
			lastSendInfo.pop_back();
	}*/

	if (antiAim->lbyUpdatedLastTick)
	{
		antiAim->lbyUpdatedLastTick = false;
	}
	else if (antiAim->lbyUpdate)
	{
		antiAim->lbyUpdatedLastTick = true;
	}
}
