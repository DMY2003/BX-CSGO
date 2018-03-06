#pragma once

typedef bool(__thiscall *SetupBones_t)(void *_this, matrix3x4_t *pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime);

extern SetupBones_t oSetupBones;

bool __fastcall SetupBones(void *_this, void *edx, matrix3x4_t *pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime);
