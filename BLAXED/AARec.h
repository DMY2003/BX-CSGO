#pragma once

#include "SDK.h"
#include <deque>

struct CTickRecord
{
	CTickRecord() {}

	CTickRecord(CBaseEntity* ent) 
	{
		SaveRecord(ent);
	}

	void SaveRecord(CBaseEntity *ent)
	{
		m_flLowerBodyYawTarget = ent->GetLowerBodyYawTarget();
		m_angEyeAngless = ent->GetEyeAngles();
		m_flSimulationTime = ent->GetSimulationTime();
		m_angAbsAngles = ent->GetAbsAngles();
		m_vecAbsOrigin = ent->GetAbsOrigin();
		m_iLayerCount = ent->GetNumAnimOverlays();
		for (int i = 0; i < m_iLayerCount; i++)
			animationLayer[i] = ent->GetAnimOverlays()[i];
		tickcount = I::pGlobals->tickcount;
	}

	bool operator==(CTickRecord &other)
	{
		return other.m_flSimulationTime == m_flSimulationTime;
	}

	float m_flLowerBodyYawTarget = 0.f;
	QAngle m_angEyeAngless = QAngle(0, 0, 0);
	float m_flSimulationTime = 0.f;
	Vector m_vecAbsOrigin = Vector(0, 0, 0);
	QAngle m_angAbsAngles = QAngle(0, 0, 0);
	int tickcount = 0;
	int m_iLayerCount = 0;
	CAnimationLayer animationLayer[15];
};

class AARec {
public:
	using RecordContainer = std::deque<CTickRecord*>;

	RecordContainer m_sRecords;

	CTickRecord* curTickRecord;
	CTickRecord* prevTickRecord;

	void Clear();
	void StoreVars(CBaseEntity* Entity);
	//bool HasStaticRealAngle(int index, float tolerance = 15.f);
	bool HasStaticYawDifference(const RecordContainer& l, float tolerance = 15.f);
	bool HasSteadyDifference(const RecordContainer& l, float tolerance = 15.f);
	int GetDifferentDeltas(const RecordContainer& l, float tolerance = 15.f);
	int GetDifferentLBYs(const RecordContainer& l, float tolerance = 15.f);
	float GetLBYByComparingTicks(const RecordContainer& l);
	float GetDeltaByComparingTicks(const RecordContainer& l);
	bool DeltaKeepsChanging(const RecordContainer& cur, float tolerance = 15.f);
	bool LBYKeepsChanging(const RecordContainer& cur, float tolerance = 15.f);
	bool HasStaticRealAngle(const RecordContainer& l, float tolerance = 15.f);
	bool HasStaticRealAngle_Wrapper(int index, float tolerance);
	CTickRecord GetLatestUpdateRecord(CBaseEntity *player);
	bool IsEntityMoving(CBaseEntity *player);
	bool IsAdjustingBalance(CBaseEntity *player, CTickRecord &record, CAnimationLayer *layer);
};
