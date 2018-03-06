#pragma once

class AutoBlock
{
public:
	void Tick()
	{
		if (!cfg.Misc.autoBlock)
			return;

		if (aimbotV2->IsUsingAimWeapon())
			return;

		CUserCmd *pCmd = G::pCmd;
		CBaseEntity *pLocal = G::pLocal;
		CBaseEntity *pTarget = NULL;

		float bestDistance = 50.0f;

		int max = I::pEngineClient->GetMaxClients();

		for (int i = 0; i < max; i++)
		{
			CBaseEntity *pEntity = I::pClientEntityList->GetClientEntity(i);

			if (pEntity == NULL) continue;
			if (pEntity == pLocal) continue;
			if (pEntity->IsDormant()) continue;
			if (pEntity->GetLifeState() != LIFE_ALIVE) continue;
			if (pEntity->GetHealth() <= 0) continue;

			float distance = (pEntity->GetOrigin() - pLocal->GetOrigin()).Length();

			if (distance < bestDistance)
			{
				bestDistance = distance;
				pTarget = pEntity;
			}
		}

		if (pTarget != NULL)
		{
			QAngle angles = Math::CalcAngle(pTarget->GetOrigin(), pLocal->GetOrigin());
			angles.y -= pLocal->GetEyeAngles().y;
			angles.Clamp();

			pCmd->sidemove = sin(DEG2RAD(angles.y)) * 10000.0f;// 450.0f;

			if (pCmd->sidemove > 450.0f)
				pCmd->sidemove = 450.0f;

			if (pCmd->sidemove < -450.0f)
				pCmd->sidemove = -450.0f;
		}
	}
};

extern AutoBlock *autoBlock;
