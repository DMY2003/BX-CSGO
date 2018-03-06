#pragma once

#include "SDK.h"
#include "Global.h"
#include "FrameStageNotify.h"

class AntiAimV2
{
public:
	enum AA_Y
	{
		Y_NONE = 0,
		Y_LEGIT,
		Y_BACKWARD,
		Y_SPIN,
		Y_LBY_INVERSE,
		Y_AUTO,
		Y_BACKWARD_AT_TARGETS,
		Y_LBY_SPIN,
		INVADEAA,
		SYNCHRONIZEAA
	};

	enum AA_LBY
	{
		LBY_NONE = 0,
		LBY_INVERSE,
		LBY_AUTO_OUT,
		LBY_AUTO_IN,
		LBY_AIR,
		LBY_CUSTOM,
		LBY_AIR_INVERSE,
	};

	enum AA_X
	{
		X_NONE = 0,
		X_LEGIT,
		X_DOWN,
		X_UP,
		X_UT_FAKE_DOWN,
		X_UT_FAKE_UP,
		X_UT_FAKE_ZERO,
		X_UT_UP_CYCLE,
		X_CUSTOM,
	};

	QAngle angFake;	
	QAngle angReal;
	QAngle angNonLbyBreak;

	//////////////////////
	QAngle lastTickAngles;
	bool choke;
	bool usingTick;
	bool lastTickChoked;
	//////////////////////

	bool breakingLby;
	bool lbyUpdate;
	bool lbyUpdate2;
	float nextLbyUpdate;
	float nextLbyUpdate2;
	bool lbyUpdatedLastTick;
	bool breakLby;
	bool breakLby2;
	float lastMovingYaw;
	
	AntiAimV2();

	bool CreateMove(CUserCmd* pCmd);
	void HandleGameEvent(IGameEvent* pEvent);
	void FrameStageNotify(ClientFrameStage_t stage);

	unsigned int pitchUpCycleStep;
	unsigned int realJitter;
	unsigned int fakeJitter;

	void DoPitch(int style, float& pitch);
	void DoYaw(int style, bool real, float& yaw);
	void DoLby(int style, bool real, float& yaw);

	float lastThrow;
	bool waitingThrow;

	bool roundEnded;

	bool AllDead();
	bool OverrideAA();

	void FindTargetAngle(float& yaw);

	void TraceWorld(const QAngle& rotation, const Vector& start, float range, trace_t& trace);
	void AutoYaw(float &yaw);
};

extern AntiAimV2 *antiAim;


