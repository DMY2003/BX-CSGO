#pragma once

// last update 18-10-2017
#define m_iClip1						0x3204
#define m_iClip2						m_iClip1 + 0x8
#define m_bInReload						0x3245 // m_bCanReload ?
#define m_flNextPrimaryAttack_			0x31D8
#define m_bReloadVisuallyComplete		0x32CC
#define m_fAccuracyPenalty				0x32B0
#define m_flPostponeFireReadyTime_		0x32C8
#define m_nViewModelIndex				0x31E0
#define m_iState_						0x31F8
#define m_iRecoilIndex_					0x32BC
#define m_iShotsFired_					0xA2B0
#define m_bPinPulled_					0x3342
#define m_fThrowTime_					0x3344

class CHudTexture;
typedef int CSWeaponType;

struct WeaponInfo_t
{
	/*BYTE		m_pad_0x0000[4];			//0x0000
	bool		m_bParsedScript;			//0x0004
	bool		m_bLoadedHudElements;		//0x0005
	char		m_szClassName[80];		//0x0006
	char		m_szPrintName[80];		//0x0056
	char		m_szViewModel[80];		//0x00A6
	char		m_szWorldModel[80];		//0x00F6
	char		m_szBulletType[32];		//0x0146
	char		m_szWorldDroppedModel[80];//0x0166
	char		m_szAnimationPrefix[16];	//0x01B6
	BYTE		m_pad_0x01C6[2];			//0x01C6
	int			m_iSlot;					//0x01C8
	int			m_iPosition;				//0x01CC
	int			m_iMaxClip1;				//0x01D0
	int			m_iMaxClip2;				//0x01D4
	int			m_iDefaultClip1;			//0x01D8
	int			m_iDefaultClip2;			//0x01DC
	int			m_iWeight;				//0x01E0
	int			m_iRumbleEffect;			//0x01E4
	bool		m_bAutoSwitchTo;			//0x01E8
	bool		m_bAutoSwitchFrom;		//0x01E9
	BYTE		m_pad_0x01EA[2];			//0x01EA
	int			m_iFlags;					//0x01EC
	char		m_szAmmo1[16];			//0x01F0
	char		m_szAmmo2[16];			//0x0200
	char		m_szAIAddOn[80];			//0x0210
	char		m_aShootSounds[17][80];	//0x0260
	char PAD_NEW[80];
	int			m_iAmmoType;
	int			m_iAmmo2Type;
	bool		m_bMeleeWeapon;
	bool		m_bBuiltRightHanded;
	bool		m_bAllowFlipping;
	BYTE		m_pad_0x07BB;
	int			m_iSpriteCount;
	CHudTexture* m_iconWeaponS;
	CHudTexture* m_iconWeapon;
	CHudTexture* m_iconAmmo;
	CHudTexture* m_iconAmmo2;
	CHudTexture* m_iconCrosshair;
	CHudTexture* m_iconAutoaim;
	CHudTexture* m_iconZoomedCrosshair;
	CHudTexture* m_iconZoomedAutoaim;
	CHudTexture* m_iconWeaponSmall;
	bool		m_bShowUsageHint;
	BYTE		m_pad_0x07E5[3];
	CSWeaponType m_iWeaponType;
	int			m_iTeam;
	int			m_iWeaponId;
	bool		m_bFullAuto;
	BYTE		m_pad_0x07F5[3];
	float		m_flHeatPerShot;
	int			m_iWeaponPrice;
	float		m_flArmorRatio;
	float		m_flMaxPlayerSpeed;
	float		m_flMaxPlayerSpeedAlt;
	int			m_iCrosshairMinDistance;
	int			m_iCrosshairDeltaDistance;
	float		m_flPenetration;
	int			m_iDamage;
	float		m_flRange;
	float		m_flRangeModifier;
	int			m_iBullets;
	float		m_flCycleTime;
	float		m_flCycleTimeAlt;
	char		m_szHeatEffect[80];
	float		m_aflSmokeColor[3];
	char		m_szMuzzleFlashEffect_1stPerson[80];
	char		m_szMuzzleFlashEffect_3rdPerson[80];
	char		m_szEjectBrassEffect[80];
	char		m_szTracerEffect[80];
	int			m_iTracerFrequency;
	float		m_flSpread;
	float		m_flSpreadAlt;
	float		m_flInaccuracyCrouch;
	float		m_flInaccuracyCrouchAlt;
	float		m_flInaccuracyStand;
	float		m_flInaccuracyStandAlt;
	float		m_flInaccuracyJump;
	float		m_flInaccuracyJumpAlt;
	float		m_flInaccuracyLand;
	float		m_flInaccuracyLandAlt;
	float		m_flInaccuracyLadder;
	float		m_flInaccuracyLadderAlt;
	float		m_flInaccuracyFire;
	float		m_flInaccuracyFireAlt;
	float		m_flInaccuracyMove;
	float		m_flInaccuracyMoveAlt;
	float		m_flRecoveryTimeStand;
	float		m_flRecoveryTimeCrouch;
	float		m_flInaccuracyReload;
	float		m_flInaccuracyAltSwitch;
	float		m_flRecoilAngle;
	float		m_flRecoilAngleAlt;
	float		m_flRecoilAngleVariance;
	float		m_flRecoilAngleVarianceAlt;
	float		m_flRecoilMagnitude;
	float		m_flRecoilMagnitudeAlt;
	float		m_flRecoilMagnitudeVariance;
	float		m_flRecoilMagnitudeVarianceAlt;
	int			m_iRecoilSeed;
	float		m_flFlinchVelocityModifierLarge;
	float		m_flFlinchVelocityModifierSmall;
	float		m_flTimeToIdle;
	float		m_flIdleInterval;
	float		m_recoilTable[2][128];
	int			m_iZoomLevels;
	int			m_iZoomFOV[3];
	float		m_fZoomTime[3];
	bool		m_bHideViewModelZoomed;
	char		m_szZoomInSound[80];
	char		m_szZoomOutSound[80];
	BYTE		m_pad_0x0F11[3];
	float		m_flBotAudibleRange;
	bool		m_bCanUseWithShield;
	char		m_szWrongTeamMsg[32];
	char		m_szAnimPrefix[16];
	char		m_szShieldViewModel[64];
	char		m_szAddonModel[80];
	char		m_szAddonLocation[80];
	char		m_szSilencerModel[80];
	BYTE		m_pad_0x1079[3];
	float		m_flAddonScale;
	float		m_flThrowVelocity;
	int			m_iKillAward;
	BYTE		m_pad_0x1088[8];*/

	char		m_pad_0x0000[4];			// 0x0000
	char*		m_consoleName;			// 0x0004
	char		m_pad_0008[12];			// 0x0008
	int			m_iMaxClip1;				// 0x0014
	int			m_iMaxClip2;				// 0x0018
	int			m_iDefaultClip1;			// 0x001C
	int			m_iDefaultClip2;			// 0x0020
	char		m_pad_0024[8];			// 0x0024
	char*		m_szWorldModel;			// 0x002C
	char*		m_szViewModel;			// 0x0030
	char*		m_szDroppedModel;			// 0x0034
	char		m_pad_0038[4];			// 0x0038
	char*		m_N0000023E;				// 0x003C
	char		m_pad_0040[56];			// 0x0040
	char*		m_szEmptySound;			// 0x0078
	char		m_pad_007C[4];			// 0x007C
	char*		m_szBulletType;			// 0x0080
	char		m_pad_0084[4];			// 0x0084
	char*		m_szHudName;				// 0x0088
	char*		m_szWeaponName;			// 0x008C
	char		m_pad_0090[56];			// 0x0090
	int 		m_WeaponType;				// 0x00C8
	int			m_iWeaponPrice;			// 0x00CC
	int			m_iKillAward;				// 0x00D0
	char*		m_szAnimationPrefix;		// 0x00D4
	float		m_flCycleTime;			// 0x00D8
	float		m_flCycleTimeAlt;			// 0x00DC
	float		m_flTimeToIdle;			// 0x00E0
	float		m_flIdleInterval;			// 0x00E4
	bool		m_bFullAuto;				// 0x00E8
	char		m_pad_0x00E5[3];			// 0x00E9
	int			m_iDamage;				// 0x00EC
	float		m_flArmorRatio;			// 0x00F0
	int			m_iBullets;				// 0x00F4
	float		m_flPenetration;			// 0x00F8
	float		m_flFlinchVelocityModifierLarge;	// 0x00FC
	float		m_flFlinchVelocityModifierSmall;	// 0x0100
	float		m_flRange;				// 0x0104
	float		m_flRangeModifier;		// 0x0108
	float		m_flThrowVelocity;		// 0x010C
	char		m_pad_0x010C[12];			// 0x0110
	bool		m_bHasSilencer;			// 0x011C
	char		m_pad_0x0119[3];			// 0x011D
	char*		m_pSilencerModel;			// 0x0120
	int			m_iCrosshairMinDistance;	// 0x0124
	int			m_iCrosshairDeltaDistance;// 0x0128 - iTeam?
	float		m_flMaxPlayerSpeed;		// 0x012C
	float		m_flMaxPlayerSpeedAlt;	// 0x0130
	float		m_flSpread;				// 0x0134
	float		m_flSpreadAlt;			// 0x0138
	float		m_flInaccuracyCrouch;		// 0x013C
	float		m_flInaccuracyCrouchAlt;	// 0x0140
	float		m_flInaccuracyStand;		// 0x0144
	float		m_flInaccuracyStandAlt;	// 0x0148
	float		m_flInaccuracyJumpInitial;// 0x014C
	float		m_flInaccuracyJump;		// 0x0150
	float		m_flInaccuracyJumpAlt;	// 0x0154
	float		m_flInaccuracyLand;		// 0x0158
	float		m_flInaccuracyLandAlt;	// 0x015C
	float		m_flInaccuracyLadder;		// 0x0160
	float		m_flInaccuracyLadderAlt;	// 0x0164
	float		m_flInaccuracyFire;		// 0x0168
	float		m_flInaccuracyFireAlt;	// 0x016C
	float		m_flInaccuracyMove;		// 0x0170
	float		m_flInaccuracyMoveAlt;	// 0x0174
	float		m_flInaccuracyReload;		// 0x0178
	int			m_iRecoilSeed;			// 0x017C
	float		m_flRecoilAngle;			// 0x0180
	float		m_flRecoilAngleAlt;		// 0x0184
	float		m_flRecoilAngleVariance;	// 0x0188
	float		m_flRecoilAngleVarianceAlt;	// 0x018C
	float		m_flRecoilMagnitude;		// 0x0190
	float		m_flRecoilMagnitudeAlt;	// 0x0194
	float		m_flRecoilMagnitudeVariance;	// 0x0198
	float		m_flRecoilMagnitudeVarianceAlt;	// 0x019C
	float		m_flRecoveryTimeCrouch;	// 0x01A0
	float		m_flRecoveryTimeStand;	// 0x01A4
	float		m_flRecoveryTimeCrouchFinal;	// 0x01A8
	float		m_flRecoveryTimeStandFinal;	// 0x01AC
	int			m_iRecoveryTransitionStartBullet;// 0x01B0 
	int			m_iRecoveryTransitionEndBullet;	// 0x01B4
	bool		m_bUnzoomAfterShot;		// 0x01B8
	bool		m_bHideViewModelZoomed;	// 0x01B9
	//char		m_pad_0x01B5[2];			// 0x01BA
	float		m_recoilTable[2][128];
	short		m_iZoomLevels;
	int			m_iZoomFOV[2];			// 0x01C0
	float		m_fZoomTime[3];			// 0x01C4
	char*		m_szWeaponClass;			// 0x01D4
	float		m_flAddonScale;			// 0x01D8
	char		m_pad_0x01DC[4];			// 0x01DC
	char*		m_szEjectBrassEffect;		// 0x01E0
	char*		m_szTracerEffect;			// 0x01E4
	int			m_iTracerFrequency;		// 0x01E8
	int			m_iTracerFrequencyAlt;	// 0x01EC
	char*		m_szMuzzleFlashEffect_1stPerson; // 0x01F0
	char		m_pad_0x01F4[4];			 // 0x01F4
	char*		m_szMuzzleFlashEffect_3rdPerson; // 0x01F8
	char		m_pad_0x01FC[4];			// 0x01FC
	char*		m_szMuzzleSmokeEffect;	// 0x0200
	float		m_flHeatPerShot;			// 0x0204
	char*		m_szZoomInSound;			// 0x0208
	char*		m_szZoomOutSound;			// 0x020C
	float		m_flInaccuracyPitchShift;	// 0x0210
	float		m_flInaccuracySoundThreshold;	// 0x0214
	float		m_flBotAudibleRange;		// 0x0218
	BYTE		m_pad_0x0218[8];			// 0x0220
	char*		m_pWrongTeamMsg;			// 0x0224
	bool		m_bHasBurstMode;			// 0x0228
	BYTE		m_pad_0x0225[3];			// 0x0229
	bool		m_bIsRevolver;			// 0x022C
	bool		m_bCannotShootUnderwater;	// 0x0230
}
;

class CBaseCombatWeapon : public CBaseEntity
{
public:
	OFFSET_FUNC_GET_MAKE(float, GetNextPrimaryAttack, m_flNextPrimaryAttack_)
	OFFSET_FUNC_SET_MAKE(float, SetNextPrimaryAttack, next, m_flNextPrimaryAttack_)
	OFFSET_FUNC_GET_MAKE(byte, GetClip1, m_iClip1)
	OFFSET_FUNC_GET_MAKE(byte, GetClip2, m_iClip2)
	OFFSET_FUNC_GET_MAKE(bool, InReload, m_bInReload)
	OFFSET_FUNC_GET_MAKE(bool, ReloadVisuallyComplete, m_bReloadVisuallyComplete)
	OFFSET_FUNC_GET_MAKE(float, GetAccuracyPenalty, m_fAccuracyPenalty)
	OFFSET_FUNC_GET_MAKE(float, GetPostponeFireReadyTime, m_flPostponeFireReadyTime_)
	OFFSET_FUNC_SET_MAKE(float, SetPostponeFireReadyTime, val, m_flPostponeFireReadyTime_)
	OFFSET_FUNC_GET_MAKE(int, GetViewModelIndex, m_nViewModelIndex)
	OFFSET_FUNC_GET_MAKE(int, GetState, m_iState_)
	OFFSET_FUNC_GET_MAKE(int, GetRecoilIndex, m_iRecoilIndex_)
	OFFSET_FUNC_GET_MAKE(int, GetShotsFired, m_iShotsFired_)
	OFFSET_FUNC_GET_MAKE(bool, GetPinPulled, m_bPinPulled_)
	OFFSET_FUNC_GET_MAKE(float, GetThrowTime, m_fThrowTime_)

	PCHAR GetName()
	{
		typedef PCHAR(__thiscall* _GetName)(PVOID);
		return Mem::CallVirtualFunction<_GetName>(this, 436)(this);
	}

	PCHAR GetPrintName()
	{
		typedef PCHAR(__thiscall* _GetPrintName)(PVOID);
		return Mem::CallVirtualFunction<_GetPrintName>(this, 437)(this);
	}

	WeaponInfo_t* GetCSWpnData()
	{
		if (!this || this == NULL)
			return NULL;

		typedef WeaponInfo_t *(__thiscall *o_getWeapInfo)(void*);
		return Mem::CallVirtualFunction<o_getWeapInfo>(this, 446)(this); // 456
	}

	float GetInaccuracy()
	{
		typedef float(__thiscall* oGetSpread)(PVOID);
		return Mem::CallVirtualFunction<oGetSpread>(this, 469)(this);
	}

	float GetSpread()
	{
		typedef float(__thiscall* oGetInac)(PVOID);
		return Mem::CallVirtualFunction<oGetInac>(this, 439)(this);
	}
	bool CanFire();
	void UpdateAccuracyPenalty()
	{
		if (!this) return;

		typedef void(__thiscall* oUpdateAccuracyPenalty)(PVOID);
		return Mem::CallVirtualFunction<oUpdateAccuracyPenalty>(this, 470)(this);
	}
};


