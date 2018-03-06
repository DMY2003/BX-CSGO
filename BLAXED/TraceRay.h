#pragma once

typedef void(__thiscall *TraceRayHook_t)(void*, Ray_t const &ray, unsigned int mask, ITraceFilter *filter, trace_t &trace);
extern TraceRayHook_t oTraceRayHook;

extern void __fastcall TraceRayHook(void* ths, void*, Ray_t const &ray, unsigned int mask, ITraceFilter *filter, trace_t &trace);

extern bool useTraceRayHook;
