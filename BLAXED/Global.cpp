#include "Global.h"

namespace Global
{
	bool exit = false;
	HMODULE module = nullptr;
	HANDLE mainThread = nullptr;
	LPDIRECT3DDEVICE9 pDevice = nullptr;
	bool pressedKeys[256];
	bool GUIInited = false;
	bool SendPacket = false;
	bool bAimbotting = false;
	bool bVisualAimbotting = false;
	QAngle vecVisualAimbotAngs = QAngle(0.f, 0.f, 0.f);
	uintptr_t *clientMode = nullptr;
	CUserCmd* pCmd = nullptr;
	CBaseEntity *pLocal = nullptr;
	CBaseCombatWeapon *pWeapon = nullptr;
	HMODULE clientDll = nullptr;
	bool inGameAndConnected = false;
	bool flag = false;
	bool scoped = false;
	int iShotsFired = 0;
	Vector dbg_AutoWall[1024] = { Vector(0, 0, 0) };
	SpreadDebug dbg_Spread[256] = { {Vector(0, 0, 0), 0 } };
	std::deque<std::tuple<Vector, float, Color>> hitscan_points;
	unsigned int dbg_AutoWall_idx = 0;

	BoneCache boneCache[MAX_PLAYERS] = { BoneCache() };
	BoneCache boneCacheESP[MAX_PLAYERS] = { BoneCache() };
	Vector absOriginCache[MAX_PLAYERS] = { Vector(0, 0, 0) };
	float lastSeenTime[MAX_PLAYERS] = { 0 };
	matrix3x4_t localPlayerLbyBones[MAX_BONES] = { matrix3x4_t() };
	WeaponInfo_t weapInfoCache[MAX_PLAYERS] = { WeaponInfo_t() };


	float viewMatrix[4][4] = { 0 };

	bool canDrawESP = false;

	HANDLE hWeaponIconFont = NULL;

	bool IsValveDS()
	{
		/*for (int i = 0; i < I::pClientEntityList->GetHighestEntityIndex(); i++)
		{
			CBaseEntity* entity = I::pClientEntityList->GetClientEntity(i);

			if (!entity) continue;

			//m_bIsValveDS 0x0075 
			if (entity->GetClientClass()->m_ClassID == _CCSGameRulesProxy)
			{
				if (*(bool*)(entity + 0x75))
					return true;
			}
		}*/

		CSGameRules* pGameRules = *(CSGameRules**)I::pGameRules;

		if (pGameRules)
			return pGameRules->m_bIsValveDS;

		return false;
	}

	bool FreezePeriod()
	{
		CSGameRules* pGameRules = *(CSGameRules**)I::pGameRules;

		if (pGameRules)
			return pGameRules->m_bFreezePeriod;

		return false;
	}
}
