#pragma once

// last update 15-10-2016
#define m_AttributeManager				0x2D70
#define m_Item							0x40
#define m_iItemDefinitionIndex			0x1D8
#define m_iItemIDHigh					0x1F0
#define m_iItemIDLow					0x1F4
#define m_iAccountID					0x1F8
#define m_iEntityQuality				0x1DC
#define m_szCustomName					0x26C
#define m_OriginalOwnerXuidLow			0x3168
#define m_OriginalOwnerXuidHigh			0x316C
#define m_nFallbackPaintKit				0x3170
#define m_nFallbackSeed					0x3174
#define m_flFallbackWear				0x3178
#define m_nFallbackStatTrak				0x317C

//#define m_nViewModelIndex				0x31D4
#define m_iViewModelIndex				0x31E0
#define m_iWorldModelIndex				0x31E4
#define m_hWeaponWorldModel_			0x31F4

enum ItemDefinitionIndex : int {
	WEAPON_DEAGLE = 1,
	WEAPON_ELITE = 2,
	WEAPON_FIVESEVEN = 3,
	WEAPON_GLOCK = 4,
	WEAPON_AK47 = 7,
	WEAPON_AUG = 8,
	WEAPON_AWP = 9,
	WEAPON_FAMAS = 10,
	WEAPON_G3SG1 = 11,
	WEAPON_GALILAR = 13,
	WEAPON_M249 = 14,
	WEAPON_M4A1 = 16,
	WEAPON_MAC10 = 17,
	WEAPON_P90 = 19,
	WEAPON_UMP45 = 24,
	WEAPON_XM1014 = 25,
	WEAPON_BIZON = 26,
	WEAPON_MAG7 = 27,
	WEAPON_NEGEV = 28,
	WEAPON_SAWEDOFF = 29,
	WEAPON_TEC9 = 30,
	WEAPON_TASER = 31,
	WEAPON_HKP2000 = 32,
	WEAPON_MP7 = 33,
	WEAPON_MP9 = 34,
	WEAPON_NOVA = 35,
	WEAPON_P250 = 36,
	WEAPON_SCAR20 = 38,
	WEAPON_SG556 = 39,
	WEAPON_SSG08 = 40,
	WEAPON_KNIFE = 42,
	WEAPON_FLASHBANG = 43,
	WEAPON_HEGRENADE = 44,
	WEAPON_SMOKEGRENADE = 45,
	WEAPON_MOLOTOV = 46,
	WEAPON_DECOY = 47,
	WEAPON_INCGRENADE = 48,
	WEAPON_C4 = 49,
	WEAPON_KNIFE_T = 59,
	WEAPON_M4A1_SILENCER = 60,
	WEAPON_USP_SILENCER = 61,
	WEAPON_CZ75A = 63,
	WEAPON_REVOLVER = 64,
	WEAPON_KNIFE_BAYONET = 500,
	WEAPON_KNIFE_FLIP = 505,
	WEAPON_KNIFE_GUT = 506,
	WEAPON_KNIFE_KARAMBIT = 507,
	WEAPON_KNIFE_M9_BAYONET = 508,
	WEAPON_KNIFE_TACTICAL = 509,
	WEAPON_KNIFE_FALCHION = 512,
	WEAPON_KNIFE_SURVIVAL_BOWIE = 514,
	WEAPON_KNIFE_BUTTERFLY = 515,
	WEAPON_KNIFE_PUSH = 516
};

class CBaseAttributableItem : public CBaseEntity {
public:

	inline int* GetItemDefinitionIndex() {
		// DT_BaseAttributableItem -> m_AttributeManager -> m_Item -> m_iItemDefinitionIndex
		return (int*)((DWORD)this + m_AttributeManager + m_Item + m_iItemDefinitionIndex);
	}

	inline int* GetItemIDHigh() {
		// DT_BaseAttributableItem -> m_AttributeManager -> m_Item -> m_iItemIDHigh
		return (int*)((DWORD)this + m_AttributeManager + m_Item + m_iItemIDHigh);
	}

	inline int* GetItemIDLow() {
		// DT_BaseAttributableItem -> m_AttributeManager -> m_Item -> m_iItemIDLow
		return (int*)((DWORD)this + m_AttributeManager + m_Item + m_iItemIDLow);
	}

	inline int* GetAccountID() {
		// DT_BaseAttributableItem -> m_AttributeManager -> m_Item -> m_iAccountID
		return (int*)((DWORD)this + m_AttributeManager + m_Item + m_iAccountID);
	}

	inline int* GetEntityQuality() {
		// DT_BaseAttributableItem -> m_AttributeManager -> m_Item -> m_iEntityQuality
		return (int*)((DWORD)this + m_AttributeManager + m_Item + m_iEntityQuality);
	}

	inline char* GetCustomName() {
		// DT_BaseAttributableItem -> m_AttributeManager -> m_Item -> m_szCustomName
		return (char*)((DWORD)this + m_AttributeManager + m_Item + m_szCustomName);
	}

	inline int* GetOriginalOwnerXuidLow() {
		// DT_BaseAttributableItem -> m_OriginalOwnerXuidLow
		return (int*)((DWORD)this + m_OriginalOwnerXuidLow);
	}

	inline int* GetOriginalOwnerXuidHigh() {
		// DT_BaseAttributableItem -> m_OriginalOwnerXuidHigh
		return (int*)((DWORD)this + m_OriginalOwnerXuidHigh);
	}

	inline int* GetFallbackPaintKit() {
		// DT_BaseAttributableItem -> m_nFallbackPaintKit
		return (int*)((DWORD)this + m_nFallbackPaintKit);
	}

	inline int* GetFallbackSeed() {
		// DT_BaseAttributableItem -> m_nFallbackSeed
		return (int*)((DWORD)this + m_nFallbackSeed);
	}

	inline float* GetFallbackWear() {
		// DT_BaseAttributableItem -> m_flFallbackWear
		return (float*)((DWORD)this + m_flFallbackWear);
	}

	inline int* GetFallbackStatTrak() {
		// DT_BaseAttributableItem -> m_nFallbackStatTrak
		return (int*)((DWORD)this + m_nFallbackStatTrak);
	}

	inline int* GetViewModelIndex() {
		return (int*)((DWORD)this + m_iViewModelIndex);
	}

	inline int* GetWorldModelIndex() {
		return (int*)((DWORD)this + m_iWorldModelIndex);
	}

	inline int GetWorldModel() {
		return *(int*)((DWORD)this + m_hWeaponWorldModel_);
	}
};
