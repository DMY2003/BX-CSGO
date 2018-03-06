#pragma once

#include "SDK.h"

#include <d3d9.h>
#include <d3dx9.h>
#include <deque>
namespace Global
{
	extern bool exit;
	extern HMODULE module;
	extern HANDLE mainThread;
	extern bool SendPacket;
	extern LPDIRECT3DDEVICE9 pDevice;
	extern bool pressedKeys[256];
	extern bool GUIInited;
	extern bool scoped;
	extern bool bAimbotting;
	extern bool bVisualAimbotting;
	extern QAngle vecVisualAimbotAngs;
	extern uintptr_t *clientMode;
	extern int iShotsFired;
	extern CUserCmd* pCmd;
	extern CBaseEntity *pLocal;
	extern CBaseCombatWeapon *pWeapon;
	extern HMODULE clientDll;
	extern bool inGameAndConnected;
	extern bool flag;
	extern Vector dbg_AutoWall[1024];
	extern unsigned int dbg_AutoWall_idx;
	extern bool canDrawESP;
	extern std::deque<std::tuple<Vector, float, Color>> hitscan_points;
	class BoneCache
	{
	public:
		bool valid;
		matrix3x4_t pBoneToWorldOut[MAX_BONES];

		BoneCache() 
		{ 
			valid = false;
		}
	};

	extern BoneCache boneCache[MAX_PLAYERS];
	extern BoneCache boneCacheESP[MAX_PLAYERS];
	extern Vector absOriginCache[MAX_PLAYERS];
	extern float lastSeenTime[MAX_PLAYERS];
	extern matrix3x4_t localPlayerLbyBones[MAX_BONES];
	
	extern float viewMatrix[4][4];

	class SpreadDebug
	{
	public:
		Vector point;
		int hit;
	};

	extern SpreadDebug dbg_Spread[256];

	bool IsValveDS();
	bool FreezePeriod();

	extern HANDLE hWeaponIconFont;

	extern WeaponInfo_t weapInfoCache[MAX_PLAYERS];
};

namespace G = Global;
