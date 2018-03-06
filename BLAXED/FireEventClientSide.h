#pragma once

typedef bool(__thiscall *FireEvent_t)(void* ths, void** evnt, bool bDontBroadcast);
extern FireEvent_t oFireEvent;

typedef bool(__thiscall *FireEventClientSide_t)(void* ths, void** evnt);
extern FireEventClientSide_t oFireEventClientSide;

extern bool __fastcall FireEvent(void* ths, void* edx, void** evnt, bool bDontBroadcast);
extern bool __fastcall FireEventClientSide(void* ths, void* edx, void** evnt);


