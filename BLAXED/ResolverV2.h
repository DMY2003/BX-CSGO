#pragma once

#include "SDK.h"
#include "Global.h"
#include "FrameStageNotify.h"
#include "AARec.h"

namespace ResolverV2
{
	enum BruteforceStep : unsigned int {
		BF_STEP_YAW_STANDING,
		BF_STEP_YAW_FAKEWALK,
		BF_STEP_YAW_AIR,
		BF_STEP_YAW_DUCKED,
		BF_STEP_YAW_PITCH,
		BF_STEP_COUNT,
	};

	//TODO:
	enum ResolverYawModes : int
	{
		BREAKING_LBY,
		FAKE_WALK,
		MOVING_LBY,
		DUCKING_RESOLVE,
		STANDING_RESOLVE,
		STATIC_YAWDIFF,
		DELTA_CHANGE,
		LBY_CHANGE,
		STANDARD_LBY,
		STATIC_YAW,
		STATIC_YAW_FAKE,
		BODYEYE_DELTA,
		LBY_TICKS,
		LBY_CALCULATE,
		LBY_UPDATE,
		LAST_LBYUPDATE,
		LBY_OVERRIDE
	};

	struct ResolverFlags
	{
		bool fakeWalking;
		bool standing;
		bool moving;
		bool crouchPicking;
		bool fakingLby;
		bool inAir;
		bool onGround;
		bool lbyUpdated;
		bool lbyGoingUpdate;
		bool fakingYaw;
		bool lbyPredicted;
		bool unresolved;
		bool legit;
		bool ducking;
	};

	struct BruteforceInfo
	{
		unsigned char step[BF_STEP_COUNT];
		bool changeStep[BF_STEP_COUNT];
		bool missedBySpread;
		int missedCount;
		int spentBullets;
	};

	class CResolver
	{
	public:
		// internal
		CBaseEntity *entity;
		bool initialized;

		// flags
		ResolverFlags flags;

		// resolver bruteforce
		BruteforceInfo bruteforce;

		// aa recognition
		AARec aaRec;

		ResolverYawModes lastResolverMethod;

		float movingLby;

		bool fixPitch;
		float fixingPitch;

		// per tick info
		float lastSimulationTime;
		float nextLbyUpdateTime;
		float lastLby;
		float lastGroundMoveTime;
		float lastBalanceAdjust;
		float lastPitch;
		bool receivedFirstUpdate;
		float lastNonBreakedLby;
		float lastLbyUpdateTime;

		// original info
		float oDuckAmmount;
		QAngle oEyeAngles;
		Vector oVelocity;
		Vector oAbsVelocity;
		Vector oOrigin;
		Vector oAbsOrigin;
		float oLby;

		// resolver info
		QAngle rEyeAngles;		

		// player list
		enum PlayerListPitch : int
		{
			PLP_NONE = 0,
			PLP_FORCE_DOWN,
			PLP_FORCE_ZERO,
			PLP_FORCE_UP,
		};

		enum PlayerListYaw : int
		{
			PLY_NONE = 0,
			PLY_INVERSE,
			PLY_INVERSE_NO_MOVE,
			PLY_LEFT,
			PLY_RIGHT,
		};

		int plPitch;
		int plYaw;

		// functions
		void Init(CBaseEntity *entity);
		void Clear();		
		void ClearBruteforce();

		void ClearFlags();
		void ClearPerTickInfo();
		void OnEntityUpdate();
		void OnFrameRenderStart();
		
		void ProcessAngles();
		void ApplyAngles();		
		bool IsValid();
	};

	CResolver &GetResolver(int index);

	extern CBaseEntity *lastShotEntity;
	extern Vector lastShotEyePosition;
	extern Vector lastShotPoint;
	extern ResolverFlags lastShotResolverFlags;
	extern BruteforceInfo lastShotResolverBruteforce;
	extern int lastShotHitbox;

	bool Filter(CBaseEntity *entity);

	void CreateMove(CUserCmd *pCmd);
	void FrameStageNotifyPreReal(ClientFrameStage_t stage);
	void FrameStageNotifyPostReal(ClientFrameStage_t stage);
	void HandleGameEvent(IGameEvent *pEvent);
	void OnShoot(Vector from, Vector to, CBaseEntity *entity);

	void RecvLowerBodyYawTarget(const CRecvProxyData *pData, CBaseEntity *entity, void *pOut);
	void RecvEyeAnglesX(const CRecvProxyData *pData, CBaseEntity *entity, void *pOut);
	void RecvEyeAnglesY(const CRecvProxyData *pData, CBaseEntity *entity, void *pOut);
	void RecvEyeAnglesZ(const CRecvProxyData *pData, CBaseEntity *entity, void *pOut);

	extern RecvVarProxyFn oRecvLowerBodyYawTarget;
	extern RecvVarProxyFn oRecvEyeAnglesX;
	extern RecvVarProxyFn oRecvEyeAnglesY;
	extern RecvVarProxyFn oRecvEyeAnglesZ;
};
