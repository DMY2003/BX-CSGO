#include "SDK.h"
#include "Global.h"
#include "FrameStageNotify.h"
#include "AntiAimV2.h"
#include "AimbotV2.h"
#include "AutoWall.h"
#include "QAngle.h"

AntiAimV2 *antiAim = new AntiAimV2();

AntiAimV2::AntiAimV2()
{
	angFake = QAngle(0, 0, 0);
	angReal = QAngle(0, 0, 0);
	lastTickAngles = QAngle(0, 0, 0);
	angNonLbyBreak = QAngle(0, 0, 0);

	choke = false;

	breakingLby = false;
	lbyUpdate = false;
	lbyUpdate2 = false;
	lbyUpdatedLastTick = false;

	nextLbyUpdate = 0;
	nextLbyUpdate2 = 0;
	
	pitchUpCycleStep = 0;
	realJitter = 0;
	fakeJitter = 0;

	lastThrow = INFINITY;
	waitingThrow = false;

	roundEnded = false;
	lastTickChoked = false;

	lastMovingYaw = 0;
}
void QAngles::SinCos(float a, float* s, float*c)
{
	*s = sin(a);
	*c = cos(a);
}
void QAngles::AngleVectors(const QAngles &angles, Vector& forward)
{
	float	sp, sy, cp, cy;

	SinCos(DEG2RAD(angles[1]), &sy, &cy);
	SinCos(DEG2RAD(angles[0]), &sp, &cp);

	forward.x = cp*cy;
	forward.y = cp*sy;
	forward.z = -sp;
}
float QAngles::RealYawDifference(QAngles dst, QAngles add)
{
	dst += add;

	dst.pitch = 0.f;

	Vector ang, aim;

	AngleVectors(*this, aim);
	AngleVectors(dst, ang);

	return RAD2DEG(acos(aim.Dot(ang) / aim.LengthSqr()));
}

float QAngles::Difference(QAngles dst, QAngles add)
{
	dst += add;

	Vector ang, aim;

	AngleVectors(*this, aim);
	AngleVectors(dst, ang);

	return RAD2DEG(acos(aim.Dot(ang) / aim.LengthSqr()));
}
void AntiAimV2::DoPitch(int style, float& pitch)
{	
	switch (style)
	{
	case AA_X::X_DOWN:
		pitch = 88.93f;
		break;
	case AA_X::X_UP:
		pitch = -88.93f;
		break;
	case AA_X::X_UT_FAKE_DOWN:
		pitch = -180.1f;
		break;
	case AA_X::X_UT_FAKE_UP:
		pitch = 911.1f;
		break;
	case AA_X::X_UT_FAKE_ZERO:
		pitch = 1080.3f;
		break;
	case AA_X::X_UT_UP_CYCLE:
		switch (pitchUpCycleStep % 2)
		{
		case 0: DoPitch(AA_X::X_UT_FAKE_UP, pitch); break;
		case 1: DoPitch(AA_X::X_UT_FAKE_ZERO, pitch); break;
		}
		break;
	case AA_X::X_CUSTOM:
		pitch = cfg.Rage.AntiAim.customPitch;
		break;
	}
}

void AntiAimV2::DoYaw(int style, bool real, float& yaw)
{
	CBaseEntity *pLocal = G::pLocal;

	switch (style)
	{
		case AA_Y::Y_BACKWARD:
			yaw += 180.0f;
			break;
		case AA_Y::Y_SPIN:
			yaw = (float)fmod((double)(pLocal->GetTickBase()) * 1.6180339887498948482 * 3.14 * 5, 360.0f);
		
			if (!real)
				yaw += 180.0f;

			break;
		case AA_Y::Y_LBY_INVERSE:
			yaw = pLocal->GetLowerBodyYawTarget() + (real ? 180.0f : 0 );
			break;
		case AA_Y::Y_AUTO:
			AutoYaw(yaw);
		
			if (!real)
				yaw += 180.0f + (float)Math::Random(-35, 35);
			break;
		case AA_Y::Y_BACKWARD_AT_TARGETS:
			FindTargetAngle(yaw);
			yaw += 180.0f;
			break;

		case AA_Y::Y_LBY_SPIN:
			yaw = pLocal->GetLowerBodyYawTarget() + (real ? +(90 + 35.0f) : 0);
			break;

		case AA_Y::INVADEAA:
		{
			QAngles lby = QAngles(0, pLocal->GetLowerBodyYawTarget(), 0);
			QAngles result = real ? QAngles(0, yaw + 33.3f, 0) : QAngles(0, yaw - 33.3f, 0);
			yaw = result.RealYawDifference(lby) < 27.5f ? yaw : result.yaw;
		}
		break;
		case AA_Y::SYNCHRONIZEAA:
		{
			QAngles lby = QAngles(0, pLocal->GetLowerBodyYawTarget(), 0);
			QAngles result = real ? QAngles(0, yaw + 180.f, 0) : QAngles(0, yaw - 180.f, 0);
			yaw = (result.RealYawDifference(lby) < 75.f || result.RealYawDifference(lby) > 165.f) ? lby.yaw + 90.f : lby.yaw + 180.f;
		}
		break;
	}
	
	yaw = Math::NormalizeYawInPlace(yaw);
}

void AntiAimV2::DoLby(int style, bool real, float& yaw)
{
	CBaseEntity *pLocal = G::pLocal;

	yaw = real ? angNonLbyBreak.y : pLocal->GetLowerBodyYawTarget();
	
	float autoYawLeft = yaw + 90.0f;
	float autoYawRight = yaw - 90.0f;
	
	switch (style)
	{
	case AA_LBY::LBY_INVERSE:
		yaw += 180.0f;
		break;
	case AA_LBY::LBY_AUTO_OUT:
		AutoYaw(autoYawLeft);
		AutoYaw(autoYawRight);
		
		yaw += 90.0f + 35.0f;
		break;
	case AA_LBY::LBY_AUTO_IN:
		AutoYaw(autoYawLeft);
		AutoYaw(autoYawRight);

		yaw -= 90.0f + 35.0f;
		break;
	case AA_LBY::LBY_AIR:
		yaw += 180.0f;
		break;

	case AA_LBY::LBY_AIR_INVERSE:
		yaw = pLocal->GetLowerBodyYawTarget() + 180.0f + (float)Math::Random(-35, 35);
		break;

	case AA_LBY::LBY_CUSTOM:
		yaw += cfg.Rage.AntiAim.fakelLbyOffset;
		break;
	}
	
	yaw = Math::NormalizeYawInPlace(yaw);
}

bool AntiAimV2::CreateMove(CUserCmd* pCmd)
{
	usingTick = false;

	if (!cfg.Rage.AntiAim.enabled)
		return false;

	CBaseEntity *pLocal = G::pLocal;
	CBaseCombatWeapon *pWeapon = G::pWeapon;

	if (!pLocal)
		return false;

	if (aimbotV2->IsUsingGrenade())
	{
		if (pCmd->buttons & IN_ATTACK || pCmd->buttons & IN_ATTACK2)
		{
			lastThrow = I::pGlobals->curtime;
			waitingThrow = true;
		}
	}

	if (aimbotV2->IsUsingKnife() && (pCmd->buttons & IN_ATTACK || pCmd->buttons & IN_ATTACK2) && aimbotV2->IsAbleToShot())
	{
		return false;
	}

	int throwTicks = TIME_TO_TICKS(I::pGlobals->curtime - lastThrow);

	if (waitingThrow && throwTicks > 15 && !choke)
	{
		waitingThrow = false;
	}

	if (OverrideAA())
		return false;

	{
		float curtime = TICKS_TO_TIME(pLocal->GetTickBase());// TICKS_TO_TIME(I::pGlobals->tickcount);

		float speed = pLocal->GetVelocity().Length();
		bool moving = speed >= 1;
		static float lastLby = 0;
		float lby = Math::NormalizeYawInPlace(pLocal->GetLowerBodyYawTarget());

		//if (pCmd->buttons & IN_WALK && G::pLocal->CheckOnGround() && G::pLocal->GetVelocity().Length() < 250)
		//	moving = speed >= 35;

		float desync = curtime - antiAim->nextLbyUpdate;

		if (curtime >= antiAim->nextLbyUpdate)
		{
			antiAim->nextLbyUpdate = curtime + 1.1f;
			antiAim->lbyUpdate = true;

			/*if (moving && pLocal->CheckOnGround())
			{
			antiAim->nextLbyUpdate = curtime + 0.22f - TICKS_TO_TIME(1);
			}*/
		}

		if (/*curtime >= antiAim->nextLbyUpdate2 || */lby != lastLby)
		{
			antiAim->nextLbyUpdate2 = curtime + 0.22f;
			antiAim->lbyUpdate2 = true;
		}

		if (moving)// && pLocal->CheckOnGround())
		{
			antiAim->nextLbyUpdate = curtime + 0.22f;
		}

		if (moving)
		{
			if (curtime > antiAim->nextLbyUpdate2)
				antiAim->nextLbyUpdate2 = curtime + 0.22f;
		}

		if (fabsf(curtime - antiAim->nextLbyUpdate) > 2.0f)
			antiAim->nextLbyUpdate = 0;

		if (fabsf(curtime - antiAim->nextLbyUpdate2) > 2.0f)
			antiAim->nextLbyUpdate2 = 0;

		//printf("%f\n", curtime - antiAim->nextLbyUpdate);

		lastLby = lby;
	}

	usingTick = true;

	QAngle angles = pCmd->viewangles;

	angFake = pLocal->GetEyeAngles();

	INetChannel *net = (INetChannel*)I::pEngineClient->GetNetChannelInfo();

	bool moving = pLocal->GetVelocity().Length() > 1.0f;//36.0f;
	bool ducking = pLocal->GetDuckAmount() > 0.8f;

	if (moving)
	{
		lastMovingYaw = angles.y;
	}
	else
	{
		angles.y = lastMovingYaw;
	}

	float speed = pLocal->GetVelocity().Length();

	int styleYaw = cfg.Rage.AntiAim.realStyle;
	int stylePitch = cfg.Rage.AntiAim.pitchStyle;
	int styleLby = cfg.Rage.AntiAim.lowerBodyStyle;

	bool onGround = pLocal->CheckOnGround();

	if (choke)
	{
		if (ducking)
			styleYaw = cfg.Rage.AntiAim.duckingRealStyle;
		else if (moving)
			styleYaw = cfg.Rage.AntiAim.movingRealStyle;
	}
	else
	{
		styleYaw = cfg.Rage.AntiAim.fakeStyle;
	}

	static bool lastOnGround = false;
	static bool needToFlick = false;
	static float lastFlick = 0;
	static bool firstFlick = false;

	float curtime = TICKS_TO_TIME(pLocal->GetTickBase());

	if (fabsf(curtime - lastFlick) > 2)
		lastFlick = curtime;

	if (lastOnGround && !onGround)
		needToFlick = true;

	lastOnGround = onGround;

	//breakingLby = Math::NormalizeYawInPlace(pLocal->GetLowerBodyYawTarget()) != Math::NormalizeYawInPlace(angReal.y);
	breakLby = styleLby != 0 && choke && ((lbyUpdate) || (needToFlick));
	breakLby2 = choke && lbyUpdate2;
	
	if (moving)
	{
		firstFlick = true;
	}

	if (styleLby == AA_LBY::LBY_AIR && !needToFlick)
	{
		breakLby = false;
	}
	
	if (breakLby)
	{
		needToFlick = false;
	}

	if (!(!choke && (stylePitch == AA_X::X_LEGIT || stylePitch == AA_X::X_NONE)))
	{
		DoPitch(stylePitch, angles.x);
	}

	if (!(!choke && (styleYaw == AA_Y::Y_LEGIT || styleYaw == AA_Y::Y_NONE)))
	{
		if (cfg.Rage.AntiAim.lowerBody)
		{
			angles.y = pLocal->GetLowerBodyYawTarget();
		}

		if (cfg.Rage.AntiAim.atTargets)
		{
			FindTargetAngle(angles.y);
		}

		if (cfg.Rage.AntiAim.staticAA)
		{
			angles.y = 0;
		}

		static float tJitter = false;
				
		if (breakLby && (!moving || !onGround) || (!onGround && styleLby == AA_LBY::LBY_AIR_INVERSE))
		{
			if (firstFlick)
			{
				DoLby(styleLby, choke, angles.y);
				firstFlick = false;
			}
			else
			{
				DoLby(styleLby, choke, angles.y);
			}

			lastFlick = curtime;
		}
		else
		{
			DoYaw(styleYaw, choke, angles.y);

			/*if (speed < 0.1f)
			{
				if (choke && styleYaw == AA_Y::Y_AUTO)
				{
					angles.y += 45;
				}
			}*/

			static bool t = false;
			
			//if (TIME_TO_TICKS(curtime) % (TIME_TO_TICKS(1.1f) + 2) == 0)
			//if (TIME_TO_TICKS(curtime) % (TIME_TO_TICKS(1.1f / 4) + 6) == 0)
			//if (TIME_TO_TICKS(curtime) % (TIME_TO_TICKS(1.1f / 8) + 15) == 0)
			//if (TIME_TO_TICKS(curtime) % (TIME_TO_TICKS(1.1f / 5) + 10) == 0)
			if (TIME_TO_TICKS(curtime) % (TIME_TO_TICKS(1.1f / 5) + 1) == 0)
				t = true;

			if (choke)
			{
				angles.y += cfg.Rage.AntiAim.realYawOffset;

				if (cfg.Rage.AntiAim.realJitter)
					angles.y += (realJitter % 2) == 0 ? 35.0f : -35.0f;

				realJitter++;
			}
			else
			{
				angles.y += cfg.Rage.AntiAim.fakeYawOffset;

				if (cfg.Rage.AntiAim.fakeJitter)
					angles.y += (fakeJitter % 2) == 0 ? 35.0f : -35.0f;

				fakeJitter++;
			}
		}

		if (!choke)
		{
			if (cfg.Rage.AntiAim.lbyJitter > 0.1f || cfg.Rage.AntiAim.lbyJitterMoving > 0.1f)
			{
				if (lbyUpdate2 || (!lbyUpdate2 && !breakingLby && speed > 0) || !breakingLby)
				{
					tJitter = !tJitter;
				}

				bool slowmotion = (pCmd->buttons & IN_WALK && G::pLocal->CheckOnGround() && G::pLocal->GetVelocity().Length() < 500);

				if (slowmotion)
				{
					if (cfg.Rage.AntiAim.lbyJitterOnlySlow)
						angles.y += (tJitter ? -1 : 1) * cfg.Rage.AntiAim.lbyJitterMoving;
				}
				else if (speed >= 27)
				{
					if (!cfg.Rage.AntiAim.lbyJitterOnlySlow)
						angles.y += (tJitter ? -1 : 1) * cfg.Rage.AntiAim.lbyJitterMoving;
				}
				else
				{
					angles.y += (tJitter ? -1 : 1) * cfg.Rage.AntiAim.lbyJitter;
				}
			}
		}

		angles += 360.0f;
	}

	pCmd->viewangles = angles;

	
	
	static float lastLby = 0;
	float lby = Math::NormalizeYawInPlace(pLocal->GetLowerBodyYawTarget());

	//if (pCmd->buttons & IN_WALK && G::pLocal->CheckOnGround() && G::pLocal->GetVelocity().Length() < 250)
	//	moving = speed >= 35;

	float desync = curtime - antiAim->nextLbyUpdate;

	if (curtime >= antiAim->nextLbyUpdate)
	{
		antiAim->nextLbyUpdate = curtime + 1.1f + TICKS_TO_TIME(1);
		antiAim->lbyUpdate = true;

		/*if (moving && pLocal->CheckOnGround())
		{
		antiAim->nextLbyUpdate = curtime + 0.22f - TICKS_TO_TIME(1);
		}*/
	}

	if (/*curtime >= antiAim->nextLbyUpdate2 || */lby != lastLby)
	{
		antiAim->nextLbyUpdate2 = curtime + 0.22f + TICKS_TO_TIME(1);
		antiAim->lbyUpdate2 = true;
	}

	if (moving)// && pLocal->CheckOnGround())
	{
		antiAim->nextLbyUpdate = curtime + 0.22f - TICKS_TO_TIME(1);
	}

	if (moving)
	{
		if (curtime > antiAim->nextLbyUpdate2)
			antiAim->nextLbyUpdate2 = curtime + 0.22f - TICKS_TO_TIME(1);
	}

	if (fabsf(curtime - antiAim->nextLbyUpdate) > 2.0f)
		antiAim->nextLbyUpdate = 0;

	if (fabsf(curtime - antiAim->nextLbyUpdate2) > 2.0f)
		antiAim->nextLbyUpdate2 = 0;

	//printf("%f\n", curtime - antiAim->nextLbyUpdate);

	lastLby = lby;

	if (breakLby2)
		lbyUpdate2 = false;

	if (choke)// && !lastTickChoked)
	{
		if (!breakLby || speed > 0)
			angNonLbyBreak = pCmd->viewangles;

		angReal = pCmd->viewangles;		
	}
	else
	{
		pitchUpCycleStep++;
	}
	
	return usingTick;
}

void AntiAimV2::HandleGameEvent(IGameEvent *pEvent)
{
	const char *name = pEvent->GetName();

	if (strstr(name, xorstr("round_start")))
	{
		roundEnded = false;
		lastThrow = INFINITY;
	}
	else if (strstr(name, xorstr("round_end")))
	{
		roundEnded = true;
	}
}

void AntiAimV2::FrameStageNotify(ClientFrameStage_t stage)
{
	
}

bool AntiAimV2::AllDead()
{
	for (int i = 0; i < I::pGlobals->maxClients; i++)
	{
		CBaseEntity *entity = I::pClientEntityList->GetClientEntity(i);

		if (!entity) continue;
		if (entity->GetTeam() == G::pLocal->GetTeam() && !cfg.Aimbot.friendlyFire) continue;

		if (entity->IsAlive())
			return false;
	}

	return true;
}

bool AntiAimV2::OverrideAA()
{
	CBaseEntity* pLocal = G::pLocal;

	if (pLocal->GetGunGameImmunity() ||
		
		(AllDead() && roundEnded) ||
		pLocal->GetFlags() & FL_FROZEN ||
		pLocal->GetMoveType() == MOVETYPE_NOCLIP ||
		pLocal->GetMoveType() == MOVETYPE_LADDER ||
		G::pCmd->buttons & IN_USE ||
		cfg.Rage.AntiAim.removeOnKnife && aimbotV2->IsUsingKnife() ||
		waitingThrow)
		return true;

	return false;
}

void AntiAimV2::FindTargetAngle(float& yaw)
{
	CBaseEntity *pLocal = G::pLocal;
	CBaseEntity *target = NULL;

	if (!AimbotV2::Filter(aimbotV2->targetInfo.entity))
	{
		target = aimbotV2->targetInfo.entity;
	}
	else
	{
		int max = I::pEngineClient->GetMaxClients();
		float minDist = FLT_MAX;

		for (int i = 0; i < max; i++)
		{
			CBaseEntity *entity = I::pClientEntityList->GetClientEntity(i);

			if (AimbotV2::Filter(entity)) continue;

			float dist = (pLocal->GetOrigin() - entity->GetOrigin()).Length();

			if (dist < minDist)
			{
				target = entity;
				minDist = dist;
			}
		}
	}

	if (target)
	{
		Vector lOrigin = pLocal->GetOrigin();
		Vector tOrigin = target->GetOrigin();

		yaw = Math::NormalizeYawInPlace(Math::CalcAngle(lOrigin, tOrigin).y);
	}
}

void AntiAimV2::AutoYaw(float &yaw)
{
	CBaseEntity *pLocal = G::pLocal;

	Vector position;	
	QAngle atTarget = Vector(0, yaw, 0);

	FindTargetAngle(atTarget.y);

	QAngle baseAngles = QAngle(0, yaw, 0);

	yaw += 180.0f;

	position = pLocal->GetEyePosition() - Vector(0, 0, 8.0f);

	bool moving = pLocal->GetVelocity().Length() > 1.0f;

	float offset = 90.0f;
	float maxDist = 4000.0f;

	float wallDistance = maxDist;

	for (float i = 0; i < 45.0f; i += 10.0f)
	{
		trace_t trace;
		TraceWorld(QAngle(0, yaw + i, 0), position, maxDist, trace);

		if (trace.fraction < 0.96f)
		{
			QAngle ang;
			Math::VectorAngles(trace.plane.normal, ang);
			ang.NormalizeAngles();

			yaw = ang.y + 180.0f;
		}

		TraceWorld(QAngle(0, yaw - i, 0), position, maxDist, trace);

		if (trace.fraction < 0.96f)
		{
			QAngle ang;
			Math::VectorAngles(trace.plane.normal, ang);
			ang.NormalizeAngles();

			yaw = ang.y + 180.0f;
		}
	}
}

void AntiAimV2::TraceWorld(const QAngle& rotation, const Vector& start, float range, trace_t& trace)
{
	Ray_t ray;
	CTraceFilterWorldOnly filter;
	Vector forward;

	Math::AngleVectors(rotation, forward);
	forward.NormalizeInPlace();

	//filter.pLocal = G::pLocal;
	ray.Init(start, (start + forward * range));

	I::pEngineTrace->TraceRay(ray, 0x46004003, &filter, &trace);
}
