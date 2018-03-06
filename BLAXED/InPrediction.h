#pragma once

typedef bool(__thiscall* InPrediction_t)(void*);
extern InPrediction_t oInPrediction;
	
extern bool __fastcall InPrediction(void *ecx, void *edx);
