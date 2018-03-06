#pragma once

typedef int CSWeaponMode;

class CNoRecoil
{
public:
	CNoRecoil(void);
	~CNoRecoil(void);

	static Vector Recoil_Reversed(CBaseCombatWeapon *pThisWeapon, CSWeaponMode weaponMode);
	static void GetRecoilOffsets(WeaponInfo_t *pWpnData, int iWeaponMode, int iRandomSeed, float& flOffsetX, float& flOffsetY);
	static Vector KickBack_Reversed(CBaseEntity *pThisPlayer, float flOffsetX, float flOffsetY);

	static void FindRecoilCVars();
	static Vector DecayAimPunchAngle_Reversed(CBaseEntity *pThisPlayer, Vector& vPunchAngleVelocity);
	static Vector DecayAimPunchAngle_Reversed(CBaseEntity *pThisPlayer);
	static float GetRecoilScale();
};
