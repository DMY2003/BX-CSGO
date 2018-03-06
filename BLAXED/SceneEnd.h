#pragma once

typedef void(__thiscall* SceneEnd_t)(void*);
extern SceneEnd_t oSceneEnd;

extern void __fastcall SceneEnd(void* _this, void* edx);
