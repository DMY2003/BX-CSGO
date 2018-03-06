#pragma once

#include "SDK.h"

typedef int(__thiscall *FireBullets_PostDataUpdate_t)(C_TEFireBullets* _this, DataUpdateType_t updateType);
extern FireBullets_PostDataUpdate_t oFireBullets_PostDataUpdate;
extern int __fastcall FireBullets_PostDataUpdate(C_TEFireBullets* _this, void *edx, DataUpdateType_t updateType);

