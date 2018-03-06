#pragma once

// last update 15-10-2016
#define m_nIndex					0x30//0x64

#define m_nModelIndex				0x254
#define m_hWeapon					0x29B8
#define m_hOwner					0x29BC


class CBaseViewModel : public CBaseEntity {
public:
	inline int GetModelIndex() {
		// DT_BaseViewModel -> m_nModelIndex
		return *(int*)((DWORD)this + m_nModelIndex);
	}

	inline DWORD GetOwner() {
		// DT_BaseViewModel -> m_hOwner
		return *(PDWORD)((DWORD)this + m_hOwner);
	}

	inline DWORD GetWeapon() {
		// DT_BaseViewModel -> m_hWeapon
		return *(PDWORD)((DWORD)this + m_hWeapon);
	}
};
