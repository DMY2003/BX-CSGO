#pragma once

#include "Util.h"
#include "VMatrix.h"
#include "UtlVector.h"
#include "datamap.h"

#define OFFSET_FUNC_GET_MAKE(TYPE,NAME,OFFSET) inline TYPE& NAME() { return *(TYPE*)((uintptr_t)this + OFFSET); }
#define OFFSET_FUNC_SET_MAKE(TYPE,NAME,VAL,OFFSET) inline void NAME(TYPE VAL) { *(TYPE*)((uintptr_t)this + OFFSET) = VAL; }
#define OFFSET_FUNC_MAKE_BUFFER(TYPE,SIZE,NAME,BUFFER,OFFSET) void NAME(TYPE *BUFFER) { byte *ptr = (byte*)this + OFFSET; memcpy(BUFFER, ptr, SIZE);BUFFER[SIZE-1]='\0'; }

#define OFFSET_FUNC_GET_SET_MAKE(TYPE,NAME,OFFSET) OFFSET_FUNC_GET_MAKE(TYPE,Get##NAME,OFFSET) OFFSET_FUNC_SET_MAKE(TYPE,Set##NAME,val,OFFSET)

#define m_lifeState					0x25B
#define m_hMyWeapons				0x2DE8
//#define m_hViewModel				0x32F8
#define m_hViewModel				0x32DC 

#define MAX_BONES 128 // 128 or 87 ?
#define MAX_PLAYERS 64

#define	LIFE_ALIVE				0 // alive
#define	LIFE_DYING				1 // playing death animation or still falling off of a ledge waiting to hit ground
#define	LIFE_DEAD				2 // dead. lying still.
#define LIFE_RESPAWNABLE		3
#define LIFE_DISCARDBODY		4

#define	FL_ONGROUND				(1<<0)	// At rest / on the ground
#define FL_DUCKING				(1<<1)	// Player flag -- Player is fully crouched
#define	FL_WATERJUMP			(1<<3)	// player jumping out of water
#define FL_ONTRAIN				(1<<4) // Player is _controlling_ a train, so movement commands should be ignored on client during prediction.
#define FL_INRAIN				(1<<5)	// Indicates the entity is standing in rain
#define FL_FROZEN				(1<<6) // Player is frozen for 3rd person camera
#define FL_ATCONTROLS			(1<<7) // Player can't move, but keeps key inputs for controlling another entity
#define	FL_CLIENT				(1<<8)	// Is a player
#define FL_FAKECLIENT			(1<<9)	// Fake client, simulated server side; don't send network messages to them
#define	FL_INWATER				(1<<10)	// In water

#define IN_ATTACK				(1 << 0)
#define IN_JUMP					(1 << 1)
#define IN_DUCK					(1 << 2)
#define IN_FORWARD				(1 << 3)
#define IN_BACK					(1 << 4)
#define IN_USE					(1 << 5)
#define IN_CANCEL				(1 << 6)
#define IN_LEFT					(1 << 7)
#define IN_RIGHT				(1 << 8)
#define IN_MOVELEFT				(1 << 9)
#define IN_MOVERIGHT			(1 << 10)
#define IN_ATTACK2				(1 << 11)
#define IN_RUN					(1 << 12)
#define IN_RELOAD				(1 << 13)
#define IN_ALT1					(1 << 14)
#define IN_ALT2					(1 << 15)
#define IN_SCORE				(1 << 16)
#define IN_SPEED				(1 << 17)
#define IN_WALK					(1 << 17)//(1 << 18)
#define IN_ZOOM					(1 << 19)
#define IN_WEAPON1				(1 << 20)
#define IN_WEAPON2				(1 << 21)
#define IN_BULLRUSH				(1 << 22)


// last update 18-10-2017
#define m_nTickBase_				0x3404
#define m_viewPunchAngle		0x3010
#define m_aimPunchAngle			0x301C
#define m_aimPunchAngleVel		0x3028
#define m_iShotsFired			0xA2B0
#define m_iGlowIndex			0xA310
#define m_iTeamNum				0xF0
#define m_iHealth				0xFC
#define m_fFlags_				0x100
#define m_dwIndex				0x64
#define m_vecViewOffset			0x104
#define m_dwModel				0x6C
#define m_bDormant				0xE9
#define m_vecOrigin_			0x134
#define m_iWeaponID				0x32EC // or 0x30?
#define m_hActiveWeapon			0x2EE8
#define m_iFOV					0x31D4
#define m_bHasHelmet			0xB22C
#define m_bHasHeavyArmor		0xB21D
#define m_ArmorValue			0x33E0
#define m_angEyeAngles			0xB23C 
#define m_flNextAttack			0x2D60
#define m_vecAngles_			0x31C8 // deadflag + 4
#define m_vecVelocity_			0x110
#define m_flLowerBodyYawTarget_	0x39DC
#define m_angRotation_			0x128
#define m_nModelIndex_			0x254
#define m_flC4Blow_				0x297C
#define m_bBombTicking_			0x2970
#define m_nForceBone_			0x267C
#define WritableBones_			m_nForceBone_ + 0x20
#define m_vecLadderNormal_		0x3214
#define m_nMoveType_			0x258
#define m_flPoseParameter_		0x2764
#define m_PlayerAnimState_		0x3870
#define m_nHitboxSet_			0x09FC
#define m_flAnimTime_			0x025C
#define m_flOldAnimTime_		m_flAnimTime_ + 4
#define m_flSimulationTime_		0x0264
#define m_flOldSimulationTime_	m_flSimulationTime_ + 4
#define moveparent_				0x0144
#define m_bIsScoped_			0x387E
#define m_hObserverTarget_		0x3360

#define m_flDuckAmount_			0x2F9C
#define m_bDucked_				0x3034
#define m_bDucking_				0x3035
#define m_flDuckSpeed_			0x2FA0
#define m_vecMins_				0x0320
#define m_vecMaxs_				0x032C

#define m_vecAbsOrigin_			0xA0 
#define m_angAbsRotation_		0xC4 
#define m_vecAbsVelocity_		0x94 

#define m_flFriction_			0x140
#define m_vecNetworkOrigin_		m_flFriction_ - 0xC // -sizeof(Vector)
#define m_hGroundEntity_		0x14C

#define m_flMaxspeed_			0x322C

#define m_vecBaseVelocity_		0x011C
#define m_flFallVelocity_		0x3004
#define m_bGunGameImmunity_		0x3894
#define m_bHasMovedSinceSpawn_	0x3895

#define m_hOwnerEntity_			0x0148
#define m_Collision_			0x0318

#define m_bClientSideAnimation_	0x288C

struct RecvProp;
struct RecvTable;

class DVariant
{
public:
	union
	{
		float	m_Float;
		long	m_Int;
		char	*m_pString;
		void	*m_pData;
		float	m_Vector[3];
	};
};

class CRecvProxyData
{
public:
	const RecvProp	*m_pRecvProp;
	char _pad[4];//csgo ( for l4d keep it commented out :) )
	DVariant		m_Value;
	int				m_iElement;
	int				m_ObjectID;
};

typedef void(*RecvVarProxyFn)(const CRecvProxyData *pData, void *pStruct, void *pOut);


struct RecvProp
{
	char					*m_pVarName;
	int						m_RecvType;
	int						m_Flags;
	int						m_StringBufferSize;
	bool					m_bInsideArray;
	const void				*m_pExtraData;
	RecvProp				*m_pArrayProp;
	void*					m_ArrayLengthProxy;
	void*					m_ProxyFn;
	void*					m_DataTableProxyFn;
	RecvTable				*m_pDataTable;
	int						m_Offset;
	int						m_ElementStride;
	int						m_nElements;
	const char				*m_pParentArrayPropName;
};

struct RecvTable
{
	RecvProp		*m_pProps;
	int				m_nProps;
	void			*m_pDecoder;
	char			*m_pNetTableName;
	bool			m_bInitialized;
	bool			m_bInMainList;
};

struct ClientClass
{
	void*			m_pCreateFn;
	void*			m_pCreateEventFn;
	char			*m_pNetworkName;
	RecvTable		*m_pRecvTable;
	ClientClass		*m_pNext;
	int				m_ClassID;
};
class VarMapEntry_t
{
public:
	unsigned short type;
	unsigned short m_bNeedsToInterpolate;	// Set to false when this var doesn't
											// need Interpolate() called on it anymore.
	void *data;
	void *watcher;
};

struct VarMapping_t
{
	SourceEngine::CUtlVector<VarMapEntry_t> m_Entries;
	int m_nInterpolatedEntries;
	float m_lastInterpolationTime;
};
class CEntityModel
{
public:
	byte padd0[4];
	char modelName[64];
	OFFSET_FUNC_MAKE_BUFFER(char, 64, GetName, buffer, 0x4)
};

class CBaseViewModel;
class CBaseCombatWeapon;
class CStudioHdr;
class CBaseEntity;
enum InvalidatePhysicsBits_t
{
	POSITION_CHANGED = 0x1,
	ANGLES_CHANGED = 0x2,
	VELOCITY_CHANGED = 0x4,
	ANIMATION_CHANGED = 0x8,		// Means cycle has changed, or any other event which would cause render-to-texture shadows to need to be rerendeded
	BOUNDS_CHANGED = 0x10,		// Means render bounds have changed, so shadow decal projection is required, etc.
	SEQUENCE_CHANGED = 0x20,		// Means sequence has changed, only interesting when surrounding bounds depends on sequence																				
};
class CAnimationLayer
{
public:
	char  pad_0000[20];
	uint32_t m_nOrder; //0x0014
	uint32_t m_nSequence; //0x0018
	float_t m_flPrevCycle; //0x001C
	float_t m_flWeight; //0x0020
	float_t m_flWeightDeltaRate; //0x0024
	float_t m_flPlaybackRate; //0x0028
	float_t m_flCycle; //0x002C
	void *m_pOwner; //0x0030 // player's thisptr
	char  pad_0038[4]; //0x0034
}; //Size: 0x0038
class CBasePlayerAnimState;
class CBaseEntity {
public:
	OFFSET_FUNC_GET_MAKE(int, GetTickBase, m_nTickBase_)
	OFFSET_FUNC_SET_MAKE(int, SetTickBase, tick, m_nTickBase_)
	OFFSET_FUNC_GET_MAKE(QAngle, GetViewPunchAngles, m_viewPunchAngle)
	OFFSET_FUNC_SET_MAKE(QAngle, SetViewPunchAngles, angles, m_viewPunchAngle)
	OFFSET_FUNC_GET_MAKE(QAngle, GetAimPunchAngles, m_aimPunchAngle)
	OFFSET_FUNC_SET_MAKE(QAngle, SetAimPunchAngles, angles, m_aimPunchAngle)
	OFFSET_FUNC_GET_MAKE(Vector, GetAimPunchAnglesVel, m_aimPunchAngleVel)
	OFFSET_FUNC_SET_MAKE(Vector, SetAimPunchAnglesVel, angles, m_aimPunchAngleVel)
	OFFSET_FUNC_GET_MAKE(int, GetShotsFired, m_iShotsFired)
	OFFSET_FUNC_GET_MAKE(int, GetGlowIndex, m_iGlowIndex)
	OFFSET_FUNC_GET_MAKE(int, GetTeam, m_iTeamNum)
	OFFSET_FUNC_GET_MAKE(int, GetHealth, m_iHealth)
	OFFSET_FUNC_GET_MAKE(int, GetFlags, m_fFlags_)
	OFFSET_FUNC_SET_MAKE(int, SetFlags, flags, m_fFlags_)
	OFFSET_FUNC_GET_MAKE(DWORD, GetIndex, m_dwIndex)
	OFFSET_FUNC_GET_MAKE(CEntityModel*, GetModel, m_dwModel)
	OFFSET_FUNC_GET_MAKE(int, GetHitboxSet, m_nHitboxSet_)
	OFFSET_FUNC_GET_MAKE(bool, IsDormant, m_bDormant)
	//OFFSET_FUNC_GET_MAKE(QAngle, GetOrigin, m_vecOrigin)
	OFFSET_FUNC_GET_MAKE(int, GetWeaponID, m_iWeaponID)
	OFFSET_FUNC_GET_MAKE(DWORD, GetActiveWeapon, m_hActiveWeapon)
	OFFSET_FUNC_GET_MAKE(int, GetFOV, m_iFOV)
	OFFSET_FUNC_GET_MAKE(bool, HasHelmet, m_bHasHelmet)
	OFFSET_FUNC_GET_MAKE(bool, HasHeavyArmor, m_bHasHeavyArmor)
	OFFSET_FUNC_GET_MAKE(int, GetArmorValue, m_ArmorValue)
	OFFSET_FUNC_GET_MAKE(QAngle, GetEyeAngles, m_angEyeAngles)
	OFFSET_FUNC_SET_MAKE(QAngle, SetEyeAngles, angles, m_angEyeAngles)
	OFFSET_FUNC_GET_MAKE(byte, GetLifeState, m_lifeState)
	OFFSET_FUNC_GET_MAKE(byte, GetNextAttack, m_flNextAttack)
	OFFSET_FUNC_SET_MAKE(byte, SetNextAttack, next, m_flNextAttack)
	OFFSET_FUNC_GET_MAKE(QAngle, GetVecAngles, m_vecAngles_)
	OFFSET_FUNC_SET_MAKE(QAngle, SetVecAngles, angles, m_vecAngles_)
	OFFSET_FUNC_GET_MAKE(Vector, GetVelocity, m_vecVelocity_)
	OFFSET_FUNC_SET_MAKE(Vector, SetVelocity, velocity, m_vecVelocity_)
	OFFSET_FUNC_GET_MAKE(float, GetLowerBodyYawTarget, m_flLowerBodyYawTarget_)
	OFFSET_FUNC_SET_MAKE(float, SetLowerBodyYawTarget, value, m_flLowerBodyYawTarget_)
	OFFSET_FUNC_GET_MAKE(QAngle, GetAngRotation, m_angRotation_)
	OFFSET_FUNC_SET_MAKE(QAngle, SetAngRotation, angles, m_angRotation_)
	OFFSET_FUNC_GET_MAKE(QAngle, GetAbsAngRotation, m_angAbsRotation_)
	OFFSET_FUNC_SET_MAKE(QAngle, SetAbsAngRotation, angles, m_angAbsRotation_)
	OFFSET_FUNC_GET_MAKE(Vector, GetAbsOrigin, m_vecAbsOrigin_)
	OFFSET_FUNC_SET_MAKE(Vector, SetAbsOrigin, origin, m_vecAbsOrigin_)
	OFFSET_FUNC_GET_MAKE(Vector, GetAbsVelocity, m_vecAbsVelocity_)
	OFFSET_FUNC_SET_MAKE(Vector, SetAbsVelocity, velocity, m_vecAbsVelocity_)
	OFFSET_FUNC_GET_MAKE(float, GetC4Blow, m_flC4Blow_);
	OFFSET_FUNC_GET_MAKE(float, GetBombTicking, m_bBombTicking_)
	OFFSET_FUNC_GET_SET_MAKE(matrix3x4_t*, BoneArrayForWrite, WritableBones_ - 8)
	OFFSET_FUNC_SET_MAKE(int, SetWritableBones, writable, WritableBones_)
	OFFSET_FUNC_GET_MAKE(Vector, GetVecLadderNormal, m_vecLadderNormal_)
	OFFSET_FUNC_GET_MAKE(MoveType, GetMoveType, m_nMoveType_)
	OFFSET_FUNC_GET_MAKE(float, GetSimulationTime, m_flSimulationTime_)
	OFFSET_FUNC_GET_MAKE(float, GetAnimTime, m_flAnimTime_)
	OFFSET_FUNC_GET_MAKE(float, GetOldAnimTime, m_flOldAnimTime_)
	OFFSET_FUNC_GET_MAKE(CBaseEntity*, GetMoveParent, moveparent_)
	OFFSET_FUNC_SET_MAKE(CBaseEntity*, SetMoveParent, parent, moveparent_)
	OFFSET_FUNC_GET_MAKE(bool, IsScoped, m_bIsScoped_)
	OFFSET_FUNC_SET_MAKE(bool, SetIsScoped, value, m_bIsScoped_)
	OFFSET_FUNC_SET_MAKE(Vector, SetOrigin, origin, m_vecOrigin_)
	OFFSET_FUNC_GET_MAKE(int, GetObserverTarget, m_hObserverTarget_)
	OFFSET_FUNC_GET_MAKE(int, GetOldSimulationTime, m_flOldSimulationTime_)
	OFFSET_FUNC_GET_SET_MAKE(float, DuckAmount, m_flDuckAmount_)
	OFFSET_FUNC_GET_SET_MAKE(int, Ducked, m_bDucked_)
	OFFSET_FUNC_GET_SET_MAKE(int, Ducking, m_bDucking_)
	OFFSET_FUNC_GET_SET_MAKE(float, DuckSpeed, m_flDuckSpeed_)
	OFFSET_FUNC_GET_SET_MAKE(Vector, NetworkOrigin, m_vecNetworkOrigin_)
	OFFSET_FUNC_GET_MAKE(int, GetGroundEntityHandle, m_hGroundEntity_)
	OFFSET_FUNC_GET_MAKE(int, GetMaxSpeed, m_flMaxspeed_)
	OFFSET_FUNC_GET_MAKE(Vector, GetViewOffset, m_vecViewOffset)

	OFFSET_FUNC_GET_SET_MAKE(Vector, BaseVelocity, m_vecBaseVelocity_)
	OFFSET_FUNC_GET_SET_MAKE(float, FallVelocity, m_flFallVelocity_)
	OFFSET_FUNC_GET_MAKE(bool, GetGunGameImmunity, m_bGunGameImmunity_)
	OFFSET_FUNC_GET_MAKE(bool, GetHasMovedSinceSpawn, m_bHasMovedSinceSpawn_)
	OFFSET_FUNC_GET_MAKE(CBaseEntity*, GetOwnerEntity, m_hOwnerEntity_)
	OFFSET_FUNC_GET_SET_MAKE(Vector, Mins, m_Collision_ + m_vecMins_)
	OFFSET_FUNC_GET_SET_MAKE(Vector, Maxs, m_Collision_ + m_vecMaxs_)
	OFFSET_FUNC_GET_SET_MAKE(bool, ClientSideAnimation, m_bClientSideAnimation_)
	
	bool SetupBones(matrix3x4_t *pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime);
	int LookupPoseParameter(char *szName);
	bool GetPoseParameterRange(int index, float &minValue, float &maxValue);
	float SetPoseParameter(CStudioHdr *pStudioHdr, int iParameter, float flValue);
	CStudioHdr *GetModelPtr();
	bool GetBonePosition(int boneId, Vector &pos);
	Vector GetBonePos(int bone);
	Vector GetEyePosition();
	Vector GetOrigin();
	CBaseCombatWeapon *GetWeapon();
	UINT* GetWeapons();
	DWORD GetViewModel();
	ClientClass* GetClientClass();
	void SetModelIndex(int nModelIndex);
	int GetModelIndex();
	void InvalidateBoneCache();
	std::array<float, 24> &m_flPoseParameter();
	float* GetPoseParameter();
	int AddToInterpolationList(int a2);
	int RemoveFromInterpolationList(int a2);
	bool Interpolate(float curtime);
	CCSPlayerAnimState *GetPlayerAnimState();
	//void SetAbsOrigin(Vector origin);	
	//void SetAbsAngles(Vector angles);
	//void SetAbsVelocity(Vector velocity);
	Vector GetWorldSpaceCenter();
	void PVSFix();
	bool CheckOnGround();
	void UpdateClientSideAnimation();
	VarMapping_t *VarMapping();
	CAnimationLayer *GetAnimOverlay(int index);
	int GetNumAnimOverlays();
	int UpdatePlayerAnimState(float eyeYaw, float eyePitch);
	int GetSequenceActivity(int sequence);
	bool IsBreakableEntity();
	CAnimationLayer *CBaseEntity::GetAnimOverlays();
	QAngle& GetAbsAngles();
	void SetAbsAngles(QAngle angles);
	QAngle& GetRenderAngles();
	void SetRenderAngles(QAngle angles);
	Vector& GetRenderOrigin();
	void SetRenderOrigin(Vector origin);
	void GetRenderBounds(Vector& mins, Vector& maxs);
	float_t m_surfaceFriction();
	float GetStepSize();
	void  SetStepSize(float stepsize);
	datamap_t *GetDataDescMap()
	{
		typedef datamap_t*(__thiscall *o_GetPredDescMap)(void*);
		return Mem::CallVirtualFunction<o_GetPredDescMap>(this, 15)(this);
	}
	datamap_t *GetPredDescMap()
	{
		typedef datamap_t*(__thiscall *o_GetPredDescMap)(void*);
		return Mem::CallVirtualFunction<o_GetPredDescMap>(this, 17)(this);
	}
	bool IsAlive()
	{
		return GetHealth() > 0 && GetLifeState() == LIFE_ALIVE;
	}
	void SetPoseAngles(float_t yaw, float_t pitch) {
		auto& poses = m_flPoseParameter();
		poses[11] = (pitch + 90) / 180;
		poses[2] = (yaw + 180) / 360;
	}
	QAngle& m_angAbsRotation() {
		return *(QAngle*)((DWORD)&GetAngRotation() - 12);
	}
	int getChockedTicks();
	const char *GetSequenceActivityName(int iSequence);

	void StandardBlendingRules(CStudioHdr *pStudioHdr, Vector *pos, Quaternion* q, float currentTime, int boneMask);
	void BuildTransformations(CStudioHdr *pStudioHdr, Vector *pos, Quaternion* q, const matrix3x4_t &cameraTransform, int boneMask, byte *computed);
	void UpdateIKLocks(float currentTime);
	void CalculateIKLocks(float currentTime);
	int getPing();
	void DrawModel(int flags, BYTE alpha);
	bool ShouldDraw();
};

