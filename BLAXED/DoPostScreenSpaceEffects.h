#pragma once


typedef void(__thiscall *DoPostScreenSpaceEffects_t)(void* _this, CViewSetup* setup);
extern DoPostScreenSpaceEffects_t oDoPostScreenSpaceEffects;

extern void __fastcall DoPostScreenSpaceEffects(void* ecx, void* edx, CViewSetup* setup);

extern void ApplyGlow();

