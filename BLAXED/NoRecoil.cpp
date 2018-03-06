#include "SDK.h"
#include "Global.h"
#include "NoRecoil.h"

CNoRecoil::CNoRecoil(void)
{
}


CNoRecoil::~CNoRecoil(void)
{
}


Vector CNoRecoil::Recoil_Reversed(CBaseCombatWeapon *pThisWeapon, CSWeaponMode weaponMode)
{
	WeaponInfo_t *pWpnData = pThisWeapon->GetCSWpnData();
	CBaseEntity *pOwnerEntity = pThisWeapon->GetOwnerEntity();

	if (!pOwnerEntity)
		return Vector(0, 0, 0);

	UINT iRandomSeed = 0;
	/*if (!pThisWeapon->IsFullAuto())
	{
		iRandomSeed = cl.random_seed;
	}
	else
	{
		iRandomSeed = *reinterpret_cast< UINT* > ((ULONG)(pThisWeapon)+0x1454/*0xD94* /);
	}*/

	float flOffsetX, flOffsetY;

	GetRecoilOffsets(pWpnData, weaponMode, iRandomSeed, flOffsetX, flOffsetY);
	return KickBack_Reversed(pOwnerEntity, flOffsetX, flOffsetY);
}

void CNoRecoil::GetRecoilOffsets(WeaponInfo_t *pWpnData, int iWeaponMode, int iRandomSeed, float& flOffsetX, float& flOffsetY)
{
	UINT iSeed = (iRandomSeed & 0x3F) * 8;
	UINT iMode = (iWeaponMode & 0x01) << 9;

	ULONG pRecoilTable = ((ULONG)pWpnData->m_recoilTable) + iMode;

	flOffsetX = *reinterpret_cast< float* >(pRecoilTable + iSeed);
	flOffsetY = *reinterpret_cast< float* >((pRecoilTable + 4) + iSeed);
}

Vector CNoRecoil::KickBack_Reversed(CBaseEntity *pThisPlayer, float flOffsetX, float flOffsetY)
{
	float flRadX;
	float flCosX, flSinX;

	flRadX = flOffsetX * 0.017453292f; //PI/180

	flCosX = cos(flRadX);
	flSinX = sin(flRadX);

	//flRadX = flSinX;

	float flSinXMulY;
	float flCosXMulY;

	flSinXMulY = -flSinX * flOffsetY;
	flCosXMulY = flCosX * flOffsetY;

	Vector punchAngleVelocity = reinterpret_cast< CBaseEntity* >(pThisPlayer)->GetAimPunchAnglesVel();

	punchAngleVelocity.x -= flCosXMulY;
	punchAngleVelocity.y += flSinXMulY;

	//pThisPlayer->SetAimPunchAngleVelocity(punchAngleVelocity);

	return punchAngleVelocity;
}

ConVar* weapon_recoil_decay2_exp;
ConVar* weapon_recoil_decay2_lin;
ConVar* weapon_recoil_vel_decay;

ConVar* weapon_recoil_scale;

void CNoRecoil::FindRecoilCVars() {
	weapon_recoil_decay2_exp = I::pCVar->FindVar(xorstr("weapon_recoil_decay2_exp"));
	weapon_recoil_decay2_lin = I::pCVar->FindVar(xorstr("weapon_recoil_decay2_lin"));
	weapon_recoil_vel_decay = I::pCVar->FindVar(xorstr("weapon_recoil_vel_decay"));

	weapon_recoil_scale = I::pCVar->FindVar(xorstr("weapon_recoil_scale"));
}

Vector CNoRecoil::DecayAimPunchAngle_Reversed(CBaseEntity *pThisPlayer, Vector& vPunchAngleVelocity)
{
	Vector vPunchAngle, vPunchAngleExpDecay;
	float flExpDecay, flExpDecayResult;
	float flLinDecay, flLinDecayResult;

	float tick_interval;
	float flPunchAngleExpDecayLength;

	if (!weapon_recoil_decay2_exp ||
		!weapon_recoil_decay2_lin ||
		!weapon_recoil_vel_decay) {
		FindRecoilCVars();
	}

	Vector& rPunchAngle = pThisPlayer->GetAimPunchAngles();
	vPunchAngle[0] = rPunchAngle[0];
	vPunchAngle[1] = rPunchAngle[1];
	vPunchAngle[2] = rPunchAngle[2];

	/*Vector& rPunchAngleVelocity =
	vPunchAngleVelocity[0] = rPunchAngleVelocity[0];
	vPunchAngleVelocity[1] = rPunchAngleVelocity[1];
	vPunchAngleVelocity[2] = rPunchAngleVelocity[2];*/


	flExpDecay = weapon_recoil_decay2_exp->GetFloat();
	flLinDecay = weapon_recoil_decay2_lin->GetFloat();

	tick_interval = I::pGlobals->interval_per_tick;

	flExpDecayResult = exp(-flExpDecay * tick_interval);

	vPunchAngleExpDecay = vPunchAngle * flExpDecayResult;
	flPunchAngleExpDecayLength = vPunchAngleExpDecay.Length();

	flLinDecayResult = flLinDecay * tick_interval;

	if ((flPunchAngleExpDecayLength > 0.0) && (flPunchAngleExpDecayLength > flLinDecayResult))
	{
		float flMultiplier;

		flMultiplier = 1.0f - (flLinDecayResult / flPunchAngleExpDecayLength);
		vPunchAngleExpDecay *= flMultiplier;
	}
	else
	{
		vPunchAngleExpDecay.x = vPunchAngleExpDecay.y = vPunchAngleExpDecay.z = 0;
	}

	vPunchAngle = (vPunchAngleVelocity * tick_interval * 0.5) + vPunchAngleExpDecay;

	flExpDecayResult = exp(tick_interval * -weapon_recoil_vel_decay->GetFloat());

	vPunchAngleVelocity *= flExpDecayResult;
	vPunchAngle += (vPunchAngleVelocity * tick_interval * 0.5f);


	Vector result;
	result[0] = (float)vPunchAngle[0];
	result[1] = (float)vPunchAngle[1];
	result[2] = (float)vPunchAngle[2];

	return result;
}

Vector CNoRecoil::DecayAimPunchAngle_Reversed(CBaseEntity *pThisPlayer)
{
	if (!cfg.Rage.Resolver.prediction)
	{
		return DecayAimPunchAngle_Reversed(pThisPlayer, pThisPlayer->GetAimPunchAnglesVel()); // or only: I::pGameMovement->DecayPunchAngle();		
	}
	
	return pThisPlayer->GetAimPunchAngles();
}

float CNoRecoil::GetRecoilScale() {
	if (!weapon_recoil_scale) {
		FindRecoilCVars();
	}
	return weapon_recoil_scale->GetFloat();
}
