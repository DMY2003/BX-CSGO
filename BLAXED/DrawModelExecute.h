#pragma once

typedef void(__thiscall* DrawModelExecute_t)(void* ths, void*, void*, ModelRenderInfo_t&, matrix3x4_t*);
extern DrawModelExecute_t oDrawModelExecute;

void __stdcall DrawModelExecute(void* context, void* state, ModelRenderInfo_t& info, matrix3x4_t* pCustomBoneToWorld);

// fix link bug
#include "DrawModelExecute.cpp"
