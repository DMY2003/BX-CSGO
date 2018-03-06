#pragma once

#include "SDK.h"
#include "Global.h"

#define	HITGROUP_GENERIC	0
#define	HITGROUP_HEAD		1
#define	HITGROUP_CHEST		2
#define	HITGROUP_STOMACH	3
#define HITGROUP_LEFTARM	4	
#define HITGROUP_RIGHTARM	5
#define HITGROUP_LEFTLEG	6
#define HITGROUP_RIGHTLEG	7
#define HITGROUP_GEAR		10			// alerts NPC, but doesn't do damage or bleed (1/100th damage)

struct FireBulletData
{
	Vector          src;
	trace_t         enter_trace;
	Vector          direction;
	CTraceFilter    filter;
	float           trace_length;
	float           trace_length_remaining;
	float           current_damage;
	int             penetrate_count;
	CBaseEntity		*target;
	Vector			dst;
	int				hitbox;
};

class CAutowallFilter : public ITraceFilter
{
public:
	bool ShouldHitEntity(CBaseEntity *ent, unsigned int mask)
	{
		return ent != me && ent != mywep;
	}

public:
	CBaseEntity *me, *mywep;
};

class AutoWall
{
public:
	static FireBulletData currentData;

	static float GetHitgroupDamageMultiplier(int iHitGroup);
	static void ScaleDamage(int hitgroup, CBaseEntity *enemy, float weapon_armor_ratio, float &current_damage);
	static bool DidHitNonWorldEntity(CBaseEntity* m_pEnt);
	static bool TraceToExit(Vector &end, trace_t& enter_trace, Vector start, Vector dir, trace_t *exit_trace);
	static void TraceLine(const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask, CBaseEntity* ignore, trace_t* ptr);
	static void TraceHull(const Vector& vecAbsStart, const Vector& vecAbsEnd, const Vector& vecMins, const Vector& vecMaxs, unsigned int mask, CBaseEntity* ignore, trace_t* ptr);
	static bool HandleBulletPenetration(WeaponInfo_t *wpn_data, FireBulletData &data);
	static bool SimulateFireBullet(CBaseCombatWeapon* pWeapon, FireBulletData &data);
	static float GetDamage(const Vector& aimPoint, CBaseEntity *target, int hitbox);
	static float FastAutoWall(const Vector& aimPoint, CBaseEntity *target);
	static bool FastAutoWall2(int hitbox, CBaseEntity *target);
	static void ClipTraceToPlayers(const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask, ITraceFilter* filter, trace_t* tr);
	static float GetDamageEx(const Vector& aimPoint, const Vector& origin, CBaseEntity *target, CBaseEntity *filterEnt, int hitbox, FireBulletData *fbd = NULL);
	static float GetDamageVec(const Vector &vecPoint);
};
