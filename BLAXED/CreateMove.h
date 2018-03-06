#pragma once

typedef bool(__stdcall *CreateMove_t)(float SampleTime, CUserCmd* pCmd);
extern CreateMove_t oCreateMove;

extern bool __stdcall CreateMove(float SampleTime, CUserCmd* pCmd);
