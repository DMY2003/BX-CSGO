#pragma once

#include "SDK.h"
#include "Global.h"
#include "Prediction.h"

/* Moved to Aimbot
class AimbotRecordData
{
public:
	bool valid;
	CBaseEntity *entity;
	int tickcount;
	Vector point;
	float damage;
	bool canShot;

	AimbotRecordData()
	{
		valid = false;
	}
};

class AimbotRecord
{
	int top;

public:
	static const int MAX = 10;

	AimbotRecordData data[MAX];

	AimbotRecord()
	{
		top = 0;
	}

	void Push(CBaseEntity *entity, int tickcount, float damage, Vector point, bool canShot)
	{
		int i = (top % MAX);

		data[i].entity = entity;
		data[i].tickcount = tickcount;		
		data[i].damage = damage;
		data[i].valid = true;
		data[i].point = point;
		data[i].canShot = canShot;
		
		top++;
	}

	void Clear()
	{
		for (int i = 0; i < MAX; i++)
			data[i].valid = false;

		top = 0;
	}

	AimbotRecordData *Get(int offset)
	{
		int i = ((top - offset - 1) % MAX);

		if (top <= 0 || i < 0)
			return NULL;

		return &data[i];
	}
};

class RageBot
{
public:
	AimbotRecord backtrack[MAX_PLAYERS];

	AimbotRecordData *curRecordData;

	RageBot()
	{
		for (int i = 0; i < MAX_PLAYERS; i++)
			backtrack[i].Clear();

		curRecordData = NULL;
	}
	
	void Begin()
	{
		CreateTickRecords();
		Process();
	}

	void End()
	{

	}

	static bool Filter(CBaseEntity *entity)
	{
		if (!entity) return true;
		if (entity == G::pLocal) return true;
		if (entity->IsDormant()) return true;
		if (entity->GetHealth() <= 0 || entity->GetLifeState() != LIFE_ALIVE) return true;
		if (entity->GetTeam() == G::pLocal->GetTeam() && !cfg.Aimbot.friendlyFire) return true;
		
		return false;
	}

	bool IsValidRecordData(AimbotRecordData *data)
	{
		if (!data)
			return false;

		if (!data->valid)
			return false;

		if (!Prediction::TickIsValid(data->tickcount))
			return false;

		return true;
	}
	
	AimbotRecordData *GetBestRecordData(CBaseEntity *entity)
	{
		int id = entity->GetIndex();

		AimbotRecordData *data = NULL;

		for (int i = 0; i < AimbotRecord::MAX; i++)
		{
			AimbotRecordData *tmp = backtrack[id].Get(i);

			if (!IsValidRecordData(tmp))
			{			
				break;
			}

			data = tmp;

			//break;
		}

		return data;
	}

	void CreateTickRecords()
	{
		CBaseEntity *pLocal = G::pLocal;
		CUserCmd *pCmd = G::pCmd;

		for (int i = 0; i <  I::pEngineClient->GetMaxClients(); i++)
		{
			CBaseEntity *entity = I::pClientEntityList->GetClientEntity(i);

			if (Filter(entity)) continue;

			QAngle angles;
			Vector point;

			bool canShot = false;

			if (CalcAngles(entity, angles, point))
			{
				canShot = true;
			}
			
			backtrack[i].Push(entity, pCmd->tick_count, 100.0f, point, canShot);
		}
	}

	bool CalcAngles(CBaseEntity *entity, QAngle &outAngles, Vector &outPoint)
	{
		CBaseEntity *pLocal = G::pLocal;
		CUserCmd *pCmd = G::pCmd;

		Vector pos;
		entity->GetBonePosition(8, pos);

		outAngles = Math::CalcAngle(pLocal->GetEyePosition(), pos);
		outPoint = pos;

		if (!Aimbot::IsFreePath(pLocal->GetEyePosition(), pos))
			return false;

		return true;
	}

	float GetFOV(CBaseEntity *local, CBaseEntity *entity)
	{
		QAngle angles = Math::CalcAngle(local->GetEyePosition(), entity->GetEyePosition());
		QAngle delta = G::pCmd->viewangles - angles;

		delta.NormalizeAngles();

		return delta.Length();
	}
	
	void Process()
	{	
		CBaseEntity *pLocal = G::pLocal;
		CUserCmd *pCmd = G::pCmd;

		float maxFOV = 180.0f;

		curRecordData = NULL;

		for (int i = 0; i < I::pEngineClient->GetMaxClients(); i++)
		{
			CBaseEntity *entity = I::pClientEntityList->GetClientEntity(i);

			if (Filter(entity)) continue;

			float fov = GetFOV(pLocal, entity);

			if (fov < maxFOV)
			{
				curRecordData = GetBestRecordData(entity);
				maxFOV = fov;
			}
		}

		if (curRecordData)
		{
			if (pCmd->buttons & IN_ATTACK)
			{
				if (curRecordData->canShot)
				{
					QAngle angles = Math::CalcAngle(pLocal->GetEyePosition(), curRecordData->point);
					angles.Clamp();

					pCmd->viewangles = angles - pLocal->GetAimPunchAngles() * 2.0f;
					pCmd->buttons |= IN_ATTACK;
					pCmd->tick_count = curRecordData->tickcount;
				}
			}
		}
	}
};
*/