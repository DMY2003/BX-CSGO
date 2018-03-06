#include "SDK.h"
#include "Global.h"
#include "AimbotV2.h"
#include "NoRecoil.h"
#include "Prediction.h"
#include "AutoWall.h"
#include "BunnyHop.h"
#include "Hitbox.h"
#include "AntiAimV2.h"

AimbotV2 *aimbotV2 = new AimbotV2();

bool AimbotV2::Filter(CBaseEntity *entity)
{
	if (!entity || !G::pLocal) return true;
	if (entity->IsDormant()) return true;
	if (entity->GetLifeState() != LIFE_ALIVE) return true;
	if (entity->GetHealth() <= 0.0f) return true;
	if (entity->GetTeam() != 2 && entity->GetTeam() != 3) return true;
	if (entity == G::pLocal) return true;
	if (entity->GetTeam() == G::pLocal->GetTeam() && !cfg.Aimbot.friendlyFire) return true;
	if (entity->GetClientClass()->m_ClassID != _CCSPlayer) return true;
	if (entity->GetGunGameImmunity()) return true;

	return false;
}

void AimbotV2::CreateMove(CUserCmd *pCmd, bool& chokePacket)
{

}

bool AimbotV2::CheckVisible(CBaseEntity *entity, const Vector& point)
{
	Ray_t ray;
	trace_t trace;
	CTraceFilter filter;

	ray.Init(pLocal->GetEyePosition(), point);

	filter.pSkip = pLocal;

	I::pEngineTrace->TraceRay(ray, MASK_SHOT, &filter, &trace);

	return trace.m_pEnt == entity || trace.fraction > 0.97f;
}

int AimbotV2::GetMultiPointValue(int hitbox)
{
	if (!cfg.Rage.AutoFire.multiPoint)
		return 0;

	return cfg.Rage.Hitscan.points[hitbox];
}

float AimbotV2::GetPointScaleValue(int hitbox)
{
	return cfg.Rage.Hitscan.scale[hitbox] * cfg.Rage.AutoFire.pointScale;
}

void AimbotV2::HitscanAdd(std::vector<int>& hitscan, int hitbox)
{
	if (!cfg.Rage.Hitscan.ignore[hitbox])
		hitscan.push_back(hitbox);
}

void AimbotV2::DoAutoScope()
{
	if (!cfg.Rage.AutoFire.autoScope)
		return;

	if (!pLocal->IsScoped())
	{
		std::string name = GetUsingWeaponName();

		if (name.find(xorstr("awp")) != -1 ||
			name.find(xorstr("g3sg1")) != -1 ||
			name.find(xorstr("scar20")) != -1 ||
			name.find(xorstr("ssg08")) != -1 ||
			name.find(xorstr("scout")) != -1)
			//name.find(xorstr("sg550")) != -1 ||
			//name.find(xorstr("sg552")) != -1 ||
			//name.find(xorstr("sg556")) != -1 ||
			//name.find(xorstr("aug")) != -1)
		{
			pCmd->buttons |= IN_ATTACK2;
		}
	}
}

QAngle AimbotV2::RecoilCompensation(CBaseEntity *entity)
{
	return CNoRecoil::DecayAimPunchAngle_Reversed(entity) * CNoRecoil::GetRecoilScale();
}

Vector AimbotV2::GetSpreadForward(Vector viewAngles, int seed)
{
	float innacuracyVal = pWeapon->GetInaccuracy();
	float spreadVal = pWeapon->GetSpread();

	int recoilIdx = G::weapInfoCache[pLocal->GetIndex()].m_iRecoilSeed;
	int itemDefinitionIdx = *((CBaseAttributableItem*)pWeapon)->GetItemDefinitionIndex();

	Vector forward, right, up;
	Math::AngleVectors(viewAngles, &forward, &right, &up);

	Math::RandomSeed(seed);

	float fRand1 = Math::RandomFloat(0.f, 1.f);
	float fRandPi1 = Math::RandomFloat(0.f, 2.f * M_PI);
	float fRand2 = Math::RandomFloat(0.f, 1.f);
	float fRandPi2 = Math::RandomFloat(0.f, 2.f * M_PI);

	if (itemDefinitionIdx == WEAPON_REVOLVER)
	{
		if (pCmd->buttons & IN_ATTACK2)
		{
			fRand1 = 1.f - fRand1 * fRand1;
			fRand2 = 1.f - fRand2 * fRand2;
		}
	}
	else if (itemDefinitionIdx == WEAPON_NEGEV && recoilIdx < 3.f)
	{
		for (int i = 3; i > recoilIdx; --i)
		{
			fRand1 *= fRand1;
			fRand2 *= fRand2;
		}

		fRand1 = 1.f - fRand1;
		fRand2 = 1.f - fRand2;
	}

	float fRandInaccuracy = fRand1 * innacuracyVal;
	float fRandSpread = fRand2 * spreadVal;

	float fSpreadX = cos(fRandPi1) * fRandInaccuracy + cos(fRandPi2) * fRandSpread;
	float fSpreadY = sin(fRandPi1) * fRandInaccuracy + sin(fRandPi2) * fRandSpread;

	return forward + (right * fSpreadX) + (up * fSpreadY);
}

bool AimbotV2::IsRevolverAbleToShoot()
{
	if (!pLocal->GetWeapon())
		return true;

	if (!IsUsingRevolver())
		return true;

	if (TICKS_TO_TIME(pLocal->GetTickBase()) + Prediction::GetLatencyOut() < pWeapon->GetPostponeFireReadyTime())
		return false;

	return true;
}

std::string AimbotV2::GetUsingWeaponName()
{
	if (!pLocal || !pWeapon || !pWeapon->GetModel())
		return "";

	char name[64];
	pWeapon->GetModel()->GetName(name);
	name[63] = '\0';

	return std::string(name);
}

bool AimbotV2::IsUsingC4()
{
	return GetUsingWeaponName().find(xorstr("v_ied")) != -1;
}

bool AimbotV2::IsUsingGrenade()
{
	std::string name = GetUsingWeaponName();

	return name.find(xorstr("flash")) != -1 ||
		name.find(xorstr("decoy")) != -1 ||
		name.find(xorstr("molotov")) != -1 ||
		name.find(xorstr("grenade")) != -1;
}

bool AimbotV2::IsUsingKnife()
{
	return GetUsingWeaponName().find(xorstr("knife")) != -1;
}

bool AimbotV2::IsUsingRevolver()
{
	return GetUsingWeaponName().find(xorstr("revolver")) != -1;
}

bool AimbotV2::IsUsingAwp()
{
	return GetUsingWeaponName().find(xorstr("awp")) != -1;
}

bool AimbotV2::IsUsingScout()
{
	return GetUsingWeaponName().find(xorstr("ssg")) != -1;
}

bool AimbotV2::IsUsingDeagle()
{
	return GetUsingWeaponName().find(xorstr("eagle")) != -1;
}

bool AimbotV2::IsUsingZeus()
{
	return GetUsingWeaponName().find(xorstr("taser")) != -1;
}

bool AimbotV2::IsUsingAimWeapon()
{
	return !IsUsingGrenade() && !IsUsingC4() && !IsUsingKnife();
}

bool AimbotV2::IsAbleToShot()
{
	float nextPrimAttack = pWeapon->GetNextPrimaryAttack();
	float serverTime = TICKS_TO_TIME(pLocal->GetTickBase());// + I::pGlobals->frametime;absoluteframetime

	if (pWeapon->GetClip1() == 0 || pWeapon->InReload())
		return false;

	if (cfg.Rage.AutoFire.autoRevolver)
	{
		if (!IsRevolverAbleToShoot())
			return false;
	}

	if (/*nextPrimAttack <= 0 || */nextPrimAttack < serverTime)
		return true;

	return false;
}
