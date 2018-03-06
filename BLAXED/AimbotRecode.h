#pragma once

#include "SDK.h"
#include "Singleton.h"
#include "Autowall.h"
class AimRage : public Singleton<AimRage>
{

public:

	void AimRage::Work(CUserCmd *usercmd, bool& chokePacket);
	void MultiPointDraw();
	float GetDamageVec(const Vector &vecPoint);
	void ScaleDamage(int hitgroup, CBaseEntity *player, float weapon_armor_ratio, float &current_damage);

private:

	float BestHitPoint(CBaseEntity *player, int prioritized, float minDmg, mstudiohitboxset_t *hitset, matrix3x4_t matrix[], Vector &vecOut);
	Vector CalculateBestPoint(CBaseEntity *player, int prioritized, float minDmg, bool onlyPrioritized, matrix3x4_t matrix[]);
	bool CheckTarget(int i);
	void TargetEntities();
	bool TargetSpecificEnt(CBaseEntity* pEnt);
	bool HitChance(QAngle angles, CBaseEntity *ent, float chance);
	void DoAutoScopeNew();
	void AutoStop();
	void AutoCrouch();
	bool IsRevolver();
	bool IsPistol();
	std::string GetUsingWeaponNames();
	bool SimulateFireBullet(CBaseEntity* pLocal, CBaseCombatWeapon *weap, FireBulletData &data);
	bool HandleBulletPenetration(WeaponInfo_t *wpn_data, FireBulletData &data);
	bool TraceToExit(Vector &end, CGameTrace *enter_trace, Vector start, Vector dir, CGameTrace *exit_trace);
	bool TraceToExit(Vector &start, Vector &dir);
	bool IsBreakableEntity(CBaseEntity *ent);
	void ClipTraceToPlayers(const Vector &vecAbsStart, const Vector &vecAbsEnd, unsigned int mask, ITraceFilter *filter, CGameTrace *tr);
	bool DidHitNonWorldEntity(CBaseEntity *ent);
	bool IsArmored(CBaseEntity *player, int armorVal, int hitgroup);
	void traceIt(Vector &vecAbsStart, Vector &vecAbsEnd, unsigned int mask, CBaseEntity *ign, CGameTrace *tr);

	CUserCmd *usercmd = nullptr;
	CBaseCombatWeapon* local_weapon = nullptr;
	int prev_aimtarget = NULL;
	bool can_fire_weapon = false;
	bool chokePacket;
	static trace_t trace;
	static trace_t trace2;
};