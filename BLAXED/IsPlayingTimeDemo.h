#pragma once

typedef bool(__thiscall *IsPlayingTimeDemo_t)(void*);

extern IsPlayingTimeDemo_t oIsPlayingTimeDemo;

bool __fastcall IsPlayingTimeDemo(void *this_, void *edx);
