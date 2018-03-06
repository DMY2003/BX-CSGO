#pragma once


typedef void(__thiscall *OverrideView_t)(void* _this, CViewSetup* setup);
extern OverrideView_t oOverrideView;

extern void __fastcall OverrideView(void* _this, void* _edx, CViewSetup* setup);

