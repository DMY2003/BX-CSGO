#include "LagCompensation.h"
#include "Util.h"
#include "Math.h"
#include "RebuildGameMovement.h"
#include "Global.h"
#include "Prediction.h"

void LagRecord::SaveRecord(CBaseEntity *player)
{
	m_flSimulationTime = player->GetSimulationTime();
	m_vecOrigin = player->GetOrigin();//player->GetAbsOrigin();
	m_angAngles = player->GetEyeAngles();//player->GetAbsAngles();
	m_vecAbsOrigin = player->GetAbsOrigin();
	m_angAbsAngles = player->GetAbsAngles();
	m_vecMins = player->GetMins();
	m_vecMax = player->GetMaxs();
	m_arrflPoseParameters = player->m_flPoseParameter();
	m_nFlags = player->GetFlags();
	m_vecVelocity = player->GetVelocity();
	HeadPosition = player->GetBonePos(8);
	int layerCount = player->GetNumAnimOverlays();
	for (int i = 0; i < layerCount; i++)
	{
		CAnimationLayer *currentLayer = player->GetAnimOverlay(i);
		m_LayerRecords[i].m_nOrder = currentLayer->m_nOrder;
		m_LayerRecords[i].m_nSequence = currentLayer->m_nSequence;
		m_LayerRecords[i].m_flWeight = currentLayer->m_flWeight;
		m_LayerRecords[i].m_flCycle = currentLayer->m_flCycle;
	}
}
void CMBacktracking::CacheInfo()
{
	for (int s = 1; s <= I::pGlobals->maxClients; s++)
	{
		CBaseEntity *player = I::pClientEntityList->GetClientEntity(s);

		if (!player ||
			player == G::pLocal ||
			player->IsDormant() ||
			player->GetGunGameImmunity() ||
			!player->IsAlive() ||
			player->GetTeam() == G::pLocal->GetTeam())
			continue;

		float simTime = player->GetSimulationTime();
		auto& m_LagRecords = this->m_LagRecord[player->GetIndex()];
		RemoveBadRecords(player->GetIndex(), m_LagRecords);

		if (m_PlayerSaveRecords[s].m_flSimulationTime == simTime)
		{
			continue;
		}

		m_PlayerSaveRecords[s].m_iPriority = GetPriorityLevel(player, &m_PlayerSaveRecords[s]);
		m_PlayerSaveRecords[s].SaveRecord(player);

		m_LagRecords.emplace_back(m_PlayerSaveRecords[s]);
	}
}
void CMBacktracking::ProcessCMD(int iTargetIdx, CUserCmd *usercmd)
{
	LagRecord recentLR = m_RestoreLagRecord[iTargetIdx].first;
	if (!IsTickValid(TIME_TO_TICKS(recentLR.m_flSimulationTime)))
	{
		usercmd->tick_count = TIME_TO_TICKS(I::pClientEntityList->GetClientEntity(iTargetIdx)->GetSimulationTime() + GetLerpTime());
	}
	else
	{
		usercmd->tick_count = TIME_TO_TICKS(recentLR.m_flSimulationTime + GetLerpTime());
	}
}
bool CMBacktracking::FindViableRecord(CBaseEntity *player, LagRecord* record)
{
	auto& m_LagRecords = this->m_LagRecord[player->GetIndex()];

	// Ran out of records to check. Go back.
	if (backtrack_records.empty())
	{
		return false;
	}

	LagRecord
		recentLR = *backtrack_records.begin(),
		prevLR;

	// Should still use m_LagRecords because we're checking for LC break.
	auto iter = std::find(m_LagRecords.begin(), m_LagRecords.end(), recentLR);
	auto idx = std::distance(m_LagRecords.begin(), iter);
	if (0 != idx) prevLR = *std::prev(iter);

	// Saving first record for processcmd.
	m_RestoreLagRecord[player->GetIndex()].first = recentLR;

	if (!IsTickValid(TIME_TO_TICKS(recentLR.m_flSimulationTime)))
	{
		backtrack_records.pop_front();
		return backtrack_records.size() > 0; // RET_NO_RECORDS true false
	}

	// Remove a record...
	backtrack_records.pop_front();

	if ((0 != idx) && (recentLR.m_vecOrigin - prevLR.m_vecOrigin).LengthSqr() > 4096.f)
	{
		float simulationTimeDelta = recentLR.m_flSimulationTime - prevLR.m_flSimulationTime;

		int simulationTickDelta = clamp(TIME_TO_TICKS(simulationTimeDelta), 1, 15);

		for (; simulationTickDelta > 0; simulationTickDelta--)
			RebuildGameMovement::Get().FullWalkMove(player);

		// Bandage fix so we "restore" to the lagfixed player.
		m_RestoreLagRecord[player->GetIndex()].second.SaveRecord(player);
		*record = m_RestoreLagRecord[player->GetIndex()].second;

		// Clear so we don't try to bt shit we can't
		backtrack_records.clear();

		return true; // Return true so we still try to aimbot.
	}
	else
	{
		player->InvalidateBoneCache();

		player->GetMins() = recentLR.m_vecMins;
		player->GetMaxs() = recentLR.m_vecMax;
		player->m_flPoseParameter() = recentLR.m_arrflPoseParameters;

		int layerCount = player->GetNumAnimOverlays();
		for (int i = 0; i < layerCount; ++i)
		{
			CAnimationLayer *currentLayer = player->GetAnimOverlay(i);
			currentLayer->m_nOrder = recentLR.m_LayerRecords[i].m_nOrder;
			currentLayer->m_nSequence = recentLR.m_LayerRecords[i].m_nSequence;
			currentLayer->m_flWeight = recentLR.m_LayerRecords[i].m_flWeight;
			currentLayer->m_flCycle = recentLR.m_LayerRecords[i].m_flCycle;
		}

		player->GetAngRotation() = QAngle(0, 0, 0);
		player->m_angAbsRotation() = QAngle(0, 0, 0);
		player->SetPoseAngles(recentLR.m_angAngles.y, recentLR.m_angAngles.x);

		player->SetAbsAngles(QAngle(0, recentLR.m_angAngles.y, 0));
		player->SetAbsOrigin(recentLR.m_vecOrigin);

		player->UpdateClientSideAnimation();

		*record = recentLR;
		return true;
	}
}
void CMBacktracking::RemoveBadRecords(int Idx, std::deque<LagRecord>& records)
{
	auto& m_LagRecords = records;
	if (!m_LagRecords.empty())
	{
		for (auto lag_record = m_LagRecords.begin(); lag_record != m_LagRecords.end(); lag_record++)
		{
			if (!IsTickValid(TIME_TO_TICKS(lag_record->m_flSimulationTime)))
			{
				m_LagRecords.erase(lag_record);
				if (!m_LagRecords.empty())
					lag_record = m_LagRecords.begin();
				else break;
			}
		}
	}
}
void CMBacktracking::RecordBubblesort(std::deque<LagRecord> &list)
{
	// .at has added debugging bullshit so use [] later, this can't fail.

	LagRecord tmp;
	for (int i = 0; i < list.size(); i++)
	{
		for (int j = 0; j < list.size() - 1; j++)
		{
			if (list.at(j + 1).m_iPriority > list.at(j).m_iPriority)
			{
				tmp = list.at(j);
				list.at(j) = list.at(j + 1);
				list.at(j + 1) = tmp;
			}
		}
	}
}

bool CMBacktracking::StartLagCompensation(CBaseEntity *player)
{
	backtrack_records.clear();

	enum
	{
		// Only try to awall the "best" records, otherwise fail.
		TYPE_BEST_RECORDS,
		// Only try to awall the newest and the absolute best record.
		TYPE_BEST_AND_NEWEST,
		// Awall everything (fps killer)
		TYPE_ALL_RECORDS,
		// Debug types
		TYPE_DEBUG_SHOTSONLY,
		TYPE_DEBUG_OLDEST
	};

	auto& m_LagRecords = this->m_LagRecord[player->GetIndex()];
	m_RestoreLagRecord[player->GetIndex()].second.SaveRecord(player);

	switch (cfg.Rage.Resolver.backtrackingtype)
	{
	case TYPE_BEST_RECORDS:
	{
		for (auto it : m_LagRecords)
		{
			if (it.m_iPriority >= 1)
				backtrack_records.emplace_back(it);
		}
		break;
	}
	case TYPE_BEST_AND_NEWEST:
	{
		LagRecord newest_record = LagRecord();
		for (auto it : m_LagRecords)
		{
			if (it.m_flSimulationTime > newest_record.m_flSimulationTime)
				newest_record = it;

			if (it.m_iPriority >= 1 /*&& !(it.m_nFlags & FL_ONGROUND) && it.m_vecVelocity.Length2D() > 150*/)
				backtrack_records.emplace_back(it);
		}
		backtrack_records.emplace_back(newest_record);
		break;
	}
	case TYPE_ALL_RECORDS:
		// Ouch, the fps drop will be H U G E.
		backtrack_records = m_LagRecords;
		break;
	case TYPE_DEBUG_SHOTSONLY:
		for (auto it : m_LagRecords)
		{
			if (it.m_iPriority == 3)
				backtrack_records.emplace_back(it);
		}
		break;
	case TYPE_DEBUG_OLDEST:
		for (auto it = m_LagRecords.begin(); it != m_LagRecords.end(); it++)
		{
			backtrack_records.emplace_back(*it);
		}
		break;
	}

	std::sort(backtrack_records.begin(), backtrack_records.end(), [](LagRecord const &a, LagRecord const &b) { return a.m_iPriority > b.m_iPriority; });
	return backtrack_records.size() > 0;
}

void CMBacktracking::FinishLagCompensation(CBaseEntity *player)
{
	int idx = player->GetIndex();

	player->InvalidateBoneCache();

	player->GetMins() = m_RestoreLagRecord[idx].second.m_vecMins;
	player->GetMaxs() = m_RestoreLagRecord[idx].second.m_vecMax;
	player->m_flPoseParameter() = m_RestoreLagRecord[idx].second.m_arrflPoseParameters;

	int layerCount = player->GetNumAnimOverlays();
	for (int i = 0; i < layerCount; ++i)
	{
		CAnimationLayer *currentLayer = player->GetAnimOverlay(i);
		currentLayer->m_nOrder = m_RestoreLagRecord[idx].second.m_LayerRecords[i].m_nOrder;
		currentLayer->m_nSequence = m_RestoreLagRecord[idx].second.m_LayerRecords[i].m_nSequence;
		currentLayer->m_flWeight = m_RestoreLagRecord[idx].second.m_LayerRecords[i].m_flWeight;
		currentLayer->m_flCycle = m_RestoreLagRecord[idx].second.m_LayerRecords[i].m_flCycle;
	}

	player->SetAbsAngles(QAngle(0, m_RestoreLagRecord[idx].second.m_angAngles.y, 0));
	player->SetAbsOrigin(m_RestoreLagRecord[idx].second.m_vecOrigin);

	player->UpdateClientSideAnimation();

	// Resetup matrix to get cached and used
	matrix3x4_t matrix[128];
	player->SetupBones(matrix, 128, 256, I::pEngineClient->GetLastTimeStamp());
}

int CMBacktracking::GetPriorityLevel(CBaseEntity *player, LagRecord* lag_record)
{
	int priority = 0;

	if (lag_record->m_flPrevLowerBodyYaw != player->GetLowerBodyYawTarget())
	{
		lag_record->m_angAngles.y = player->GetLowerBodyYawTarget();
		priority = 2;
	}

	// Do some crouch checks, prev ones were a meme, also do anim checks for taps here.

	lag_record->m_flPrevLowerBodyYaw = player->GetLowerBodyYawTarget();

	return priority;
}

void CMBacktracking::SetOverwriteTick(CBaseEntity *player, QAngle angles, float_t correct_time, uint32_t priority)
{
	int idx = player->GetIndex();
	LagRecord overwrite_record;

	if (IsTickValid(TIME_TO_TICKS(correct_time)))
	{
		overwrite_record.SaveRecord(player);

		overwrite_record.m_angAngles = angles;
		overwrite_record.m_iPriority = priority;
		overwrite_record.m_flSimulationTime = correct_time;

		auto& m_LagRecords = this->m_LagRecord[player->GetIndex()];
		m_LagRecords.emplace_back(overwrite_record);
	}
}

bool CMBacktracking::IsTickValid(int tick)
{
	INetChannelInfo *nci = I::pEngineClient->GetNetChannelInfo();

	if (!nci)
		return false;

	auto LerpTicks = TIME_TO_TICKS(GetLerpTime());

	int predCmdArrivTick = I::pGlobals->tickcount + 1 + TIME_TO_TICKS(nci->GetLatency(FLOW_INCOMING) + nci->GetLatency(FLOW_OUTGOING));

	float flCorrect = clamp(GetLerpTime() + nci->GetLatency(FLOW_OUTGOING), 0.f, 1.f) - TICKS_TO_TIME(predCmdArrivTick + LerpTicks - (tick + TIME_TO_TICKS(GetLerpTime())));

	return fabs(flCorrect) < 0.2f;
}

float CMBacktracking::GetLerpTime()
{
	int ud_rate = I::pCVar->FindVar("cl_updaterate")->GetInt();
	ConVar *min_ud_rate = I::pCVar->FindVar("sv_minupdaterate");
	ConVar *max_ud_rate = I::pCVar->FindVar("sv_maxupdaterate");

	if (min_ud_rate && max_ud_rate)
		ud_rate = max_ud_rate->GetInt();

	float ratio = I::pCVar->FindVar("cl_interp_ratio")->GetFloat();

	if (ratio == 0)
		ratio = 1.0f;

	float lerp = I::pCVar->FindVar("cl_interp")->GetFloat();
	ConVar *c_min_ratio = I::pCVar->FindVar("sv_client_min_interp_ratio");
	ConVar *c_max_ratio = I::pCVar->FindVar("sv_client_max_interp_ratio");

	if (c_min_ratio && c_max_ratio && c_min_ratio->GetFloat() != 1)
		ratio = clamp(ratio, c_min_ratio->GetFloat(), c_max_ratio->GetFloat());

	return max(lerp, (ratio / ud_rate));
}

template<class T, class U>
T CMBacktracking::clamp(T in, U low, U high)
{
	if (in <= low)
		return low;

	if (in >= high)
		return high;

	return in;
}