#include "SDK.h"
#include "Global.h"
#include "Interfaces.h"
#include "PredictionSystem.h"

IEngineClient *I::pEngineClient = NULL;
IClientEntityList *I::pClientEntityList = NULL;
CGlobalVarsBase *I::pGlobals = NULL;
IBaseClientDll *I::pClient = NULL;
IVModelInfo *I::pModelInfo = NULL;
IEngineTrace *I::pEngineTrace = NULL;
IPhysicsSurfaceProps *I::pPhysicsSurfaceProps = NULL;
ICvar *I::pCVar = NULL;
IGameMovement *I::pGameMovement = NULL;
IMoveHelper *I::pMoveHelper = NULL;
IPrediction *I::pPrediction = NULL;
IVModelRender *I::pModelRender = NULL;
IVRenderView *I::pRenderView = NULL;
IMaterialSystem *I::pMaterialSystem = NULL;
IGameEventManager2 *I::pGameEventManager = NULL;
CInput *I::pInput = NULL;
CClientState *I::pClientState = NULL;
IPanel *I::pPanel = NULL;
ISurface *I::pSurface = NULL;
C_TEFireBullets *I::pFireBullets = NULL;
IVEffects *I::pEffects = NULL;
CGlowObjectManager *I::pGlowObjectManager = NULL;
DWORD* I::pGameRules = NULL;
IStudioRender *I::pStudioRender = NULL;

void Interfaces::GetInterfaces()
{
	while (!GetModuleHandle(xorstr("client.dll")) || !GetModuleHandle(xorstr("engine.dll")))
		Sleep(7000);

	G::clientDll = GetModuleHandle(xorstr("client.dll"));

	SAFE_WAIT(G::pDevice, 100, G::pDevice = *(LPDIRECT3DDEVICE9*)((DWORD)GetModuleHandle(xorstr("shaderapidx9.dll")) + 0xA1F40););
	SAFE_WAIT(pClient, 100, pClient = (IBaseClientDll*)InterfaceMgr::CreateInterface<uintptr_t*>(xorstr("client.dll"), xorstr("VClient")););
	SAFE_WAIT(pEngineClient, 100, pEngineClient = InterfaceMgr::CreateInterface<IEngineClient*>(xorstr("engine.dll"), xorstr("VEngineClient")););
	SAFE_WAIT(pClientEntityList, 100, pClientEntityList = InterfaceMgr::CreateInterface<IClientEntityList*>(xorstr("client.dll"), xorstr("VClientEntityList")););
	SAFE_WAIT(pModelInfo, 100, pModelInfo = InterfaceMgr::CreateInterface<IVModelInfo*>(xorstr("engine.dll"), xorstr("VModelInfoClient")););
	SAFE_WAIT(pEngineTrace, 100, pEngineTrace = InterfaceMgr::CreateInterface<IEngineTrace*>(XorStr("engine.dll"), XorStr("EngineTraceClient")););
	SAFE_WAIT(pPhysicsSurfaceProps, 100, pPhysicsSurfaceProps = InterfaceMgr::CreateInterface<IPhysicsSurfaceProps*>(XorStr("vphysics.dll"), XorStr("VPhysicsSurfaceProps")););
	SAFE_WAIT(pCVar, 100, pCVar = InterfaceMgr::CreateInterface<ICvar*>(XorStr("vstdlib.dll"), XorStr("VEngineCvar")););
	SAFE_WAIT(pGameMovement, 100, pGameMovement = *(IGameMovement**)(Mem::FindPattern(XorStr("client.dll"), XorStr("A1 ? ? ? ? B9 ? ? ? ? FF 50 04 EB 07")) + 1););
	SAFE_WAIT(pPrediction, 100, pPrediction = InterfaceMgr::CreateInterface<IPrediction*>(XorStr("client.dll"), XorStr("VClientPrediction")););
	SAFE_WAIT(pModelRender, 100, pModelRender = InterfaceMgr::CreateInterface<IVModelRender*>(XorStr("engine.dll"), XorStr("VEngineModel")););
	SAFE_WAIT(pRenderView, 100, pRenderView = InterfaceMgr::CreateInterface<IVRenderView*>(XorStr("engine.dll"), XorStr("VEngineRenderView")););
	SAFE_WAIT(pMaterialSystem, 100, pMaterialSystem = InterfaceMgr::CreateInterface<IMaterialSystem*>(XorStr("materialsystem.dll"), XorStr("VMaterialSystem")););
	SAFE_WAIT(pGameEventManager, 100, pGameEventManager = InterfaceMgr::CreateInterfaceExact<IGameEventManager2*>(XorStr("engine.dll"), XorStr("GAMEEVENTSMANAGER002")););
	SAFE_WAIT(pPanel, 100, pPanel = InterfaceMgr::CreateInterface<IPanel*>(XorStr("vgui2.dll"), XorStr("VGUI_Panel")););
	SAFE_WAIT(pSurface, 100, pSurface = InterfaceMgr::CreateInterface<ISurface*>(XorStr("vguimatsurface.dll"), XorStr("VGUI_Surface")););
	SAFE_WAIT(pMoveHelper, 100, pMoveHelper = **(IMoveHelper***)(Mem::FindPattern(XorStr("client.dll"), XorStr("8B 0D ? ? ? ? 8B 46 08 68")) + 2););
	SAFE_WAIT(pEffects, 100, pEffects = InterfaceMgr::CreateInterface<IVEffects*>(XorStr("engine.dll"), XorStr("VEngineEffects")););

	
	SAFE_WAIT(G::clientMode, 30000,
	{
		G::clientMode = **(uintptr_t***)((*(DWORD**)pClient)[10] + 5);
	});

	SAFE_WAIT(pGlobals, 100, pGlobals = **(CGlobalVarsBase***)((*(uintptr_t**)pClient)[0] + 0x1B););
	SAFE_WAIT(pInput, 100, pInput = *(CInput**)(Mem::FindPattern(xorstr("client.dll"), xorstr("B9 ? ? ? ? F3 0F 11 04 24 FF 50 10")) + 1););
	SAFE_WAIT(pClientState, 100, pClientState = **(CClientState***)((*(DWORD**)pEngineClient)[20] + 1););
	SAFE_WAIT(pGlowObjectManager, 100, pGlowObjectManager = *(CGlowObjectManager**)(Mem::FindPattern(xorstr("client.dll"), xorstr("0F 11 05 ?? ?? ?? ?? 83 C8 01 C7 05 ?? ?? ?? ?? 00 00 00 00")) + 3););
	SAFE_WAIT(pStudioRender, 100, pStudioRender = InterfaceMgr::CreateInterface<IStudioRender*>(XorStr("studiorender.dll"), XorStr("VStudioRender"));); 

	CPredictionSystem::Init();
}
