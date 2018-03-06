#pragma once

namespace Interfaces
{
	extern IEngineClient *pEngineClient;
	extern IClientEntityList *pClientEntityList;
	extern CGlobalVarsBase *pGlobals;
	extern IBaseClientDll *pClient;
	extern IVModelInfo *pModelInfo;
	extern IEngineTrace* pEngineTrace;
	extern IPhysicsSurfaceProps *pPhysicsSurfaceProps;
	extern ICvar *pCVar;
	extern IGameMovement* pGameMovement;
	extern IMoveHelper *pMoveHelper;
	extern IPrediction *pPrediction;
	extern IVModelRender *pModelRender;
	extern IVRenderView *pRenderView;
	extern IMaterialSystem *pMaterialSystem;
	extern IGameEventManager2 *pGameEventManager;
	extern CInput *pInput;
	extern CClientState *pClientState;
	extern IPanel *pPanel;
	extern ISurface *pSurface;
	extern C_TEFireBullets *pFireBullets;
	extern IVEffects *pEffects;
	extern CGlowObjectManager *pGlowObjectManager;
	extern DWORD* pGameRules;
	extern IStudioRender *pStudioRender;

	extern void GetInterfaces();
}

namespace I = Interfaces;

