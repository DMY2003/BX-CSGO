#pragma once

#include "SDK.h"

using PlaySoundFn = void(__thiscall*)(void*, const char*);
extern PlaySoundFn oPlaySound;

extern void __fastcall HookedPlaySound(void* ecx, void* edx, const char* filename);