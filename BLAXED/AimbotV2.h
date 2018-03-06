#pragma once

#include "SDK.h"
#include "Global.h"
#include "Prediction.h"

class AimbotV2 {

public:
	class TargetInfo {
	public:
		TargetInfo()
			: entity(nullptr), position(Vector(0, 0, 0))
		{ }

		CBaseEntity *entity;
		Vector position;
	};

	bool usingTick;
	bool antiLeak;

	TargetInfo targetInfo;

	void CreateMove(CUserCmd *pCmd, bool& chokePacket);
	bool CheckVisible(CBaseEntity *entity, const Vector& point);
	static bool Filter(CBaseEntity *entity);
	int GetMultiPointValue(int hitbox);
	float GetPointScaleValue(int hitbox);
	void HitscanAdd(std::vector<int>& hitscan, int hitbox);
	void DoAutoScope();
	QAngle RecoilCompensation(CBaseEntity *entity);
	Vector GetSpreadForward(Vector viewAngles, int seed);
	bool IsRevolverAbleToShoot();
	std::string GetUsingWeaponName();
	bool IsUsingC4();
	bool IsUsingGrenade();
	bool IsUsingKnife();
	bool IsUsingRevolver();
	bool IsUsingAwp();
	bool IsUsingScout();
	bool IsUsingDeagle();
	bool IsUsingZeus();

	bool IsUsingAimWeapon();
	bool IsAbleToShot();

	void UpdatePointers(CBaseEntity *pLocal, CBaseCombatWeapon *pWeapon, CUserCmd *pCmd)
	{
		this->pLocal = pLocal;
		this->pWeapon = pWeapon;
		this->pCmd = pCmd;
	}

private:
	
	CBaseEntity *pLocal;
	CBaseCombatWeapon *pWeapon;
	CUserCmd *pCmd;

	bool chokePacket;
};

extern AimbotV2 *aimbotV2;
