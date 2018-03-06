#pragma once

namespace Hooks
{
	void ApplyHooks();
	void ReleaseHooks();
};

extern VMT *clientmode_hook;
extern VMT *client_hook;
extern VMT *enginetrace_hook;
extern VMT *modelrender_hook;
extern VMT *prediction_hook;
extern VMT *gameeventmanager_hook;
extern VMT *engineclient_hook;
extern VMT *input_hook;
extern VMT *panel_hook;
extern VMT *te_fire_bulltes;
extern VMT *gamemovement_hook;
extern VMT *renderview_hook;
extern VMT *studiorender_hook;
extern VMT *surface_hook;
extern VMT *net_channelhook;

extern DWORD *pCCSPlayerRenderableVMT;

