#include "AARec.h"

float inline NormalizeFloat(float result)  // TODO
{
	while (result > 180.f)
		result -= 360.f;

	while (result < -180.f)
		result += 360.f;

	return result;
}
inline float GetDelta(float a, float b)
{
	return fabsf(NormalizeFloat(a - b));
}
inline float LBYDelta(const CTickRecord& v)
{
	return v.m_angEyeAngless.y - v.m_flLowerBodyYawTarget;
}
inline bool IsDifferent(float a, float b, float tolerance = 10.f)
{
	return (GetDelta(a, b) > tolerance);
}

bool AARec::HasStaticRealAngle(const RecordContainer& l, float tolerance)
{
	auto minmax = std::minmax_element(std::begin(l), std::end(l), [](const CTickRecord* t1, const CTickRecord* t2) { return t1->m_flLowerBodyYawTarget < t2->m_flLowerBodyYawTarget; });
	return (fabsf((*minmax.first)->m_flLowerBodyYawTarget - (*minmax.second)->m_flLowerBodyYawTarget) <= tolerance);
}

bool AARec::HasStaticRealAngle_Wrapper(int index, float tolerance)
{
	return HasStaticRealAngle(m_sRecords, tolerance);
}

bool AARec::HasStaticYawDifference(const RecordContainer& l, float tolerance) {
	for (auto i = l.begin(); i < l.end() - 1;) {
		if (GetDelta(LBYDelta(**i), LBYDelta(**++i)) > tolerance)
			return false;
	}
	return true;
}

bool AARec::HasSteadyDifference(const RecordContainer& l, float tolerance)
{
	size_t misses = 0;
	for (size_t i = 0; i < l.size() - 1; i++)
	{
		float tickdif = static_cast<float>(l.at(i)->m_flSimulationTime - l.at(i + 1)->tickcount);
		float lbydif = GetDelta(l.at(i)->m_flLowerBodyYawTarget, l.at(i + 1)->m_flLowerBodyYawTarget);
		float ntickdif = static_cast<float>(I::pGlobals->tickcount- l.at(i)->tickcount);
		if (((lbydif / tickdif) * ntickdif) > tolerance) misses++;
	}
	return (misses <= (l.size() / 3));
}

int AARec::GetDifferentDeltas(const RecordContainer& l, float tolerance)
{
	std::vector<float> vec;
	for (auto var : l)
	{
		float curdelta = LBYDelta(*var);
		bool add = true;
		for (auto fl : vec) {
			if (!IsDifferent(curdelta, fl, tolerance))
				add = false;
		}
		if (add)
			vec.push_back(curdelta);

	}
	return vec.size();
}

int AARec::GetDifferentLBYs(const RecordContainer& l, float tolerance)
{
	std::vector<float> vec;
	for (auto var : l)
	{
		float curyaw = var->m_flLowerBodyYawTarget;
		bool add = true;
		for (auto fl : vec)
		{
			if (!IsDifferent(curyaw, fl, tolerance))
				add = false;
		}
		if (add)
			vec.push_back(curyaw);
	}
	return vec.size();
}

float AARec::GetLBYByComparingTicks(const RecordContainer& l)
{
	int modulo = 1;
	int difangles = GetDifferentLBYs(l);
	int inc = modulo * difangles;
	for (auto var : l)
	{
		for (int lasttick = var->tickcount; lasttick < I::pGlobals->tickcount; lasttick += inc)
		{
			if (lasttick == I::pGlobals->tickcount)
				return var->m_flLowerBodyYawTarget;
		}
	}
	return 0.f;
}

float AARec::GetDeltaByComparingTicks(const RecordContainer& l)
{
	int modulo = 1;
	int difangles = GetDifferentDeltas(l);
	int inc = modulo * difangles;
	for (auto var : l)
	{
		for (int lasttick = var->tickcount; lasttick < I::pGlobals->tickcount; lasttick += inc)
		{
			if (lasttick == I::pGlobals->tickcount)
				return LBYDelta(*var);
		}
	}
	return 0.f;
}

bool AARec::DeltaKeepsChanging(const RecordContainer& cur, float tolerance)
{
	return (GetDifferentDeltas(cur) >(int)cur.size() / 2);
}

bool AARec::LBYKeepsChanging(const RecordContainer& cur, float tolerance)
{
	return (GetDifferentLBYs(cur, tolerance) > (int)cur.size() / 2);
}

void AARec::Clear()
{
	for (auto r : m_sRecords)
	{
		if (r) delete r;
	}

	m_sRecords.clear();
	curTickRecord = nullptr;
	prevTickRecord = nullptr;
}

void AARec::StoreVars(CBaseEntity* ent)
{
	prevTickRecord = curTickRecord;

	CTickRecord *record = new CTickRecord(ent);

	m_sRecords.push_front(record);

	if (m_sRecords.size() >= 16)
	{
		delete m_sRecords.back();
		m_sRecords.pop_back();
	}	
	
	curTickRecord = record;
}

bool AARec::IsAdjustingBalance(CBaseEntity *player, CTickRecord &record, CAnimationLayer *layer)
{
	for (int i = 0; i < record.m_iLayerCount; i++)
	{
		const int activity = player->GetSequenceActivity(record.animationLayer[i].m_nSequence);

		if (activity == 979)
		{
			*layer = record.animationLayer[i];
			return true;
		}
	}
	return false;
}

CTickRecord AARec::GetLatestUpdateRecord(CBaseEntity *player) 
{
	if (m_sRecords.size())
		return *(*m_sRecords.begin());

	return nullptr;
}

bool AARec::IsEntityMoving(CBaseEntity *player)
{
	return (player->GetVelocity().Length2D() > 0.1f && player->GetFlags() & FL_ONGROUND);
}
