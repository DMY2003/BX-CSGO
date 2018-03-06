#pragma once

#include "SDK.h"
#include "Singleton.h"
#include <deque>

class CBaseEntity;
struct LayerRecord
{
	LayerRecord()
	{
		m_nOrder = 0;
		m_nSequence = 0;
		m_flWeight = 0.f;
		m_flCycle = 0.f;
	}

	LayerRecord(const LayerRecord &src)
	{
		m_nOrder = src.m_nOrder;
		m_nSequence = src.m_nSequence;
		m_flWeight = src.m_flWeight;
		m_flCycle = src.m_flCycle;
	}

	uint32_t m_nOrder;
	uint32_t m_nSequence;
	float_t m_flWeight;
	float_t m_flCycle;
};

struct LagRecord
{
	LagRecord()
	{
		m_iPriority = -1;

		m_flSimulationTime = -1.f;
		m_vecOrigin.Init();
		m_angAngles.Init();
		m_vecMins.Init();
		m_vecMax.Init();
	}

	bool operator==(const LagRecord &rec)
	{
		return (m_flSimulationTime == rec.m_flSimulationTime);
	}

	void SaveRecord(CBaseEntity *player);
	bool m_bMatrixBuilt = false;
	// For ESP
	matrix3x4_t matrix[128];
	std::deque<std::array<float_t, 6>> bones;
	Vector	    HeadPosition;
	// For priority/other checks
	int32_t m_iPriority;
	int32_t m_nFlags;
	Vector  m_vecVelocity;
	float_t m_flPrevLowerBodyYaw;

	// For backtracking
	float_t m_flSimulationTime;
	Vector m_vecOrigin;
	QAngle m_angAngles;
	Vector m_vecAbsOrigin;
	QAngle m_angAbsAngles;
	Vector m_vecMins;
	Vector m_vecMax;
	std::array<float_t, 24> m_arrflPoseParameters;
	std::array<LayerRecord, 15> m_LayerRecords;

};

class CMBacktracking : public Singleton<CMBacktracking>
{
public:

	LagRecord m_PlayerSaveRecords[64];
	std::deque<LagRecord> m_LagRecord[64];
	std::pair<LagRecord, LagRecord> m_RestoreLagRecord[64];

	void CacheInfo();
	void ProcessCMD(int iTargetIdx, CUserCmd *usercmd);

	void RemoveBadRecords(int Idx, std::deque<LagRecord>& records);
	bool FindViableRecord(CBaseEntity *player, LagRecord* record);
	bool StartLagCompensation(CBaseEntity *player);
	void FinishLagCompensation(CBaseEntity *player);

	int GetPriorityLevel(CBaseEntity *player, LagRecord* lag_record);

	void SetOverwriteTick(CBaseEntity *player, QAngle angles, float_t correct_time, uint32_t priority);

	bool IsTickValid(int tick);
	float GetLerpTime();

	void RecordBubblesort(std::deque<LagRecord> &list);

	bool m_bStartedLagCompensation = false;

	std::deque<LagRecord> backtrack_records;

	template<class T, class U>
	T clamp(T in, U low, U high);
};