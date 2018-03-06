#include "SDK.h"
#include "FireBullets.h"

FireBullets_PostDataUpdate_t oFireBullets_PostDataUpdate = NULL;
int __fastcall FireBullets_PostDataUpdate(C_TEFireBullets* _this, void *edx, DataUpdateType_t updateType)
{/*
	CBaseEntity *entity = I::pClientEntityList->GetClientEntity(_this->m_iPlayer + 1);

	if (entity)
	{
		entity->SetAbsOrigin(_this->_m_vecOrigin - *(Vector*)((uintptr_t)entity + m_vecViewOffset));
		entity->SetOrigin(_this->_m_vecOrigin - *(Vector*)((uintptr_t)entity + m_vecViewOffset));
		entity->SetEyeAngles(_this->m_vecAngles);

		EntityResolver* resolving = resolver->GetResolver(entity);

		//player_info_t i;
		//I::pEngineClient->GetPlayerInfo(entity->GetIndex(), &i);

		//printf("%s[%d]: ID:%d P:%f Y:%f X:%f Y:%f Z:%f\n", i.m_szPlayerName, entity->GetIndex(), _this->m_iPlayer, _this->m_vecAngles.x, _this->m_vecAngles.y, _this->_m_vecOrigin.x, _this->_m_vecOrigin.y, _this->_m_vecOrigin.z);

		if (resolving)
		{
			resolving->localAngles = _this->m_vecAngles;
		}
	}*/
	
	return oFireBullets_PostDataUpdate(_this, updateType);
}
