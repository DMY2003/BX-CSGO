#include "SDK.h"
#include "Hooks.h"
#include "Global.h"
#include "DirectX.h"
#include "Render.h"
#include "CreateMove.h"
#include "FrameStageNotify.h"
#include "TraceRay.h"
#include "DrawModelExecute.h" 
#include "SetupMove.h"
#include "FireEventClientSide.h"
#include "OverrideView.h"
#include "IsPlayingTimeDemo.h"
#include "WriteUsercmdDeltaToBuffer.h"
#include "PaintTraverse.h"
#include "GameEvents.h"
#include "FireBullets.h"
#include "PlaySound.h"
#include "ConvarSpoofer.h"
#include "ResolverV2.h"
#include "DoPostScreenSpaceEffects.h"
#include "DrawModel.h"
#include "SceneEnd.h"
#include "NetChannelHook.h"
#include "InPrediction.h"
#include "VMTManager.h"
#include "SetupBones.h"


void InstallProxyes();
void RemoveProxyes();

extern RecvVarProxyFn oDidSmokeEffect = NULL;
void DidSmokeEffectProxy(const CRecvProxyData *pData, void *pStruct, void *pOut);

VMT *clientmode_hook = NULL;
VMT *client_hook = NULL;
VMT *enginetrace_hook = NULL;
VMT *modelrender_hook = NULL;
VMT *prediction_hook = NULL;
VMT *gameeventmanager_hook = NULL;
VMT *engineclient_hook = NULL;
VMT *input_hook = NULL;
VMT *panel_hook = NULL;
VMT *te_fire_bulltes = NULL;
VMT *gamemovement_hook = NULL;
VMT *renderview_hook = NULL;
VMT *studiorender_hook = NULL;
VMT *surface_hook = NULL;
VMT *net_channelhook = NULL;

DWORD *pCCSPlayerRenderableVMT = NULL;

void Hooks::ApplyHooks()
{
	Sleep(500);

	Interfaces::GetInterfaces();

	DirectX::ApplyHook(G::pDevice);

	input_hook = new VMT(I::pInput);
	engineclient_hook = new VMT(I::pEngineClient);
	prediction_hook = new VMT(I::pPrediction);
	gameeventmanager_hook = new VMT(I::pGameEventManager);
	modelrender_hook = new VMT(I::pModelRender);
	enginetrace_hook = new VMT(I::pEngineTrace);
	client_hook = new VMT(I::pClient);
	clientmode_hook = new VMT(G::clientMode);
	panel_hook = new VMT(I::pPanel);
	//te_fire_bulltes = new VMT(I::pFireBullets);
	gamemovement_hook = new VMT(I::pGameMovement);
	renderview_hook = new VMT(I::pRenderView);
	studiorender_hook = new VMT(I::pStudioRender);
	surface_hook = new VMT(I::pSurface);

	pCCSPlayerRenderableVMT = (DWORD*)(Mem::FindPattern(xorstr("client.dll"), xorstr("55 8B EC 83 E4 F8 83 EC 18 56 57 8B F9 89 7C 24 0C")) + 0x4E);

	//net_channelhook = new VMT(I::pEngineClient->GetNetChannelInfo());
	
	clientmode_hook->HookVM(&CreateMove, 24);
	oCreateMove = clientmode_hook->GetOriginalMethod<CreateMove_t>(24);
	
	clientmode_hook->HookVM(&DoPostScreenSpaceEffects, 44);
	oDoPostScreenSpaceEffects = clientmode_hook->GetOriginalMethod<DoPostScreenSpaceEffects_t>(44);

	clientmode_hook->HookVM(&OverrideView, 18);
	oOverrideView = clientmode_hook->GetOriginalMethod<OverrideView_t>(18);

	client_hook->HookVM(&FrameStageNotify, 36);
	oFrameStageNotify = client_hook->GetOriginalMethod<FrameStageNotify_t>(36);

	modelrender_hook->HookVM(&DrawModelExecute, 21);
	oDrawModelExecute = modelrender_hook->GetOriginalMethod<DrawModelExecute_t>(21);

	panel_hook->HookVM(&PaintTraverse, 41);
	oPaintTraverse = panel_hook->GetOriginalMethod<PaintTraverse_t>(41);

	//USING VAR MAP TO DISABLE INTERPOLATION / FRAMESTAGE

	//engineclient_hook->HookVM(&IsPlayingTimeDemo, 84);
	//oIsPlayingTimeDemo = engineclient_hook->GetOriginalMethod<IsPlayingTimeDemo_t>(84);
	
	surface_hook->HookVM(&HookedPlaySound, 82);
	oPlaySound = surface_hook->GetOriginalMethod<PlaySoundFn>(82);

	renderview_hook->HookVM(&SceneEnd, 9);
	oSceneEnd = renderview_hook->GetOriginalMethod<SceneEnd_t>(9);

	studiorender_hook->HookVM(&DrawModel, 29);
	oDrawModel = studiorender_hook->GetOriginalMethod<DrawModel_t>(29);

	prediction_hook->HookVM(&InPrediction, 14);
	oInPrediction = prediction_hook->GetOriginalMethod<InPrediction_t>(14);
	
	oSetupBones = VFTableHook::HookManual<SetupBones_t>(*(uintptr_t**)pCCSPlayerRenderableVMT, 13, (SetupBones_t)SetupBones);

	input_hook->ApplyVMT();
	engineclient_hook->ApplyVMT();
	prediction_hook->ApplyVMT();
	gameeventmanager_hook->ApplyVMT();
	modelrender_hook->ApplyVMT();
	enginetrace_hook->ApplyVMT();
	client_hook->ApplyVMT();
	clientmode_hook->ApplyVMT();
	panel_hook->ApplyVMT();
	//te_fire_bulltes->ApplyVMT();
	gamemovement_hook->ApplyVMT();
	renderview_hook->ApplyVMT();
	studiorender_hook->ApplyVMT();
	surface_hook->ApplyVMT();
	//net_channelhook->ApplyVMT();

	InstallProxyes();

	gameEvents->Register();
}

void Hooks::ReleaseHooks()
{
	Sleep(500);

	RemoveProxyes();

	DirectX::ReleaseHook();

	input_hook->ReleaseVMT();
	engineclient_hook->ReleaseVMT();
	prediction_hook->ReleaseVMT();
	gameeventmanager_hook->ReleaseVMT();
	modelrender_hook->ReleaseVMT();
	enginetrace_hook->ReleaseVMT();
	client_hook->ReleaseVMT();
	clientmode_hook->ReleaseVMT();
	panel_hook->ReleaseVMT();
	//te_fire_bulltes->ReleaseVMT();
	gamemovement_hook->ReleaseVMT();
	renderview_hook->ReleaseVMT();
	studiorender_hook->ReleaseVMT();
	surface_hook->ReleaseVMT();
	//net_channelhook->ReleaseVMT();

	VFTableHook::UnHookManual<SetupBones_t>(*(uintptr_t**)pCCSPlayerRenderableVMT, 13, oSetupBones);

	gameEvents->Unregister();

	if (ConvarSpoofer::IsReady())
	{
		ConvarSpoofer::Release();
	}

	if (render)
	{
		render->Shutdown();
		delete render;
	}

	Sleep(500);
}
/*
void DumpRecvTable(RecvTable* recvTable, FILE *file)
{
	if (!recvTable)
		return;

	if (recvTable->m_nProps < 0)
		return;

	fprintf(file, "%s:\n", recvTable->m_pNetTableName);

	for (int i = 0; i < recvTable->m_nProps; i++)
	{
		RecvProp *prop = &recvTable->m_pProps[i];

		if (!prop)
			continue;

		fprintf(file, "    %s:0x%X\n", prop->m_pVarName, prop->m_Offset);

		if (prop->m_pDataTable)
			DumpRecvTable(prop->m_pDataTable, file);
	}
}
*/
void InstallProxyes()
{
	ClientClass *pClass = I::pClient->GetAllClasses();

	//FILE* file = fopen("dmp.txt", "w");

	while (pClass)
	{
		//DumpRecvTable(pClass->m_pRecvTable, file);

		const char *pszName = pClass->m_pRecvTable->m_pNetTableName;

		if (!strcmp(pszName, xorstr("DT_CSPlayer")))
		{
			for (int i = 0; i < pClass->m_pRecvTable->m_nProps; i++)
			{
				RecvProp *pProp = &(pClass->m_pRecvTable->m_pProps[i]);
				const char *name = pProp->m_pVarName;

				if (!strcmp(name, xorstr("m_flLowerBodyYawTarget")))
				{
					ResolverV2::oRecvLowerBodyYawTarget = (RecvVarProxyFn)pProp->m_ProxyFn;
					pProp->m_ProxyFn = ResolverV2::RecvLowerBodyYawTarget;
				}
				else if (!strcmp(name, xorstr("m_angEyeAngles[0]")))
				{
					ResolverV2::oRecvEyeAnglesX = (RecvVarProxyFn)pProp->m_ProxyFn;
					pProp->m_ProxyFn = ResolverV2::RecvEyeAnglesX;
				}
				else if (!strcmp(name, xorstr("m_angEyeAngles[1]")))
				{
					ResolverV2::oRecvEyeAnglesY = (RecvVarProxyFn)pProp->m_ProxyFn;
					pProp->m_ProxyFn = ResolverV2::RecvEyeAnglesY;
				}
				else if (!strcmp(name, xorstr("m_angEyeAngles[2]")))
				{
					ResolverV2::oRecvEyeAnglesZ = (RecvVarProxyFn)pProp->m_ProxyFn;
					pProp->m_ProxyFn = ResolverV2::RecvEyeAnglesZ;
				}
			}
		}


		if (!strcmp(pszName, xorstr("DT_SmokeGrenadeProjectile")))
		{
			for (int i = 0; i < pClass->m_pRecvTable->m_nProps; i++)
			{
				RecvProp *pProp = &(pClass->m_pRecvTable->m_pProps[i]);
				const char *name = pProp->m_pVarName;

				if (!strcmp(name, xorstr("m_bDidSmokeEffect")))
				{
					oDidSmokeEffect = (RecvVarProxyFn)pProp->m_ProxyFn;
					pProp->m_ProxyFn = DidSmokeEffectProxy;
				}
			}
		}

		pClass = pClass->m_pNext;
	}

	//fclose(file);
}



void RemoveProxyes()
{
	ClientClass *pClass = I::pClient->GetAllClasses();

	while (pClass)
	{
		const char *pszName = pClass->m_pRecvTable->m_pNetTableName;

		if (!strcmp(pszName, xorstr("DT_CSPlayer")))
		{
			for (int i = 0; i < pClass->m_pRecvTable->m_nProps; i++)
			{
				RecvProp *pProp = &(pClass->m_pRecvTable->m_pProps[i]);
				const char *name = pProp->m_pVarName;

				if (!strcmp(name, xorstr("m_flLowerBodyYawTarget")))
				{
					pProp->m_ProxyFn = ResolverV2::oRecvLowerBodyYawTarget;
				}
				else if (!strcmp(name, xorstr("m_angEyeAngles[0]")))
				{
					pProp->m_ProxyFn = ResolverV2::oRecvEyeAnglesX;
				}
				else if (!strcmp(name, xorstr("m_angEyeAngles[1]")))
				{
					pProp->m_ProxyFn = ResolverV2::oRecvEyeAnglesY;
				}
				else if (!strcmp(name, xorstr("m_angEyeAngles[2]")))
				{
					pProp->m_ProxyFn = ResolverV2::oRecvEyeAnglesZ;
				}
			}
		}

		if (!strcmp(pClass->m_pNetworkName, xorstr("CBaseViewModel")))
		{
			for (int i = 0; i < pClass->m_pRecvTable->m_nProps; i++)
			{
				RecvProp *pProp = &(pClass->m_pRecvTable->m_pProps[i]);
				const char *name = pProp->m_pVarName;

			}
		}

		if (!strcmp(pszName, xorstr("DT_SmokeGrenadeProjectile")))
		{
			for (int i = 0; i < pClass->m_pRecvTable->m_nProps; i++)
			{
				RecvProp *pProp = &(pClass->m_pRecvTable->m_pProps[i]);
				const char *name = pProp->m_pVarName;

				if (!strcmp(name, xorstr("m_bDidSmokeEffect")))
				{
					pProp->m_ProxyFn = oDidSmokeEffect;
				}
			}
		}

		pClass = pClass->m_pNext;
	}
}

void DidSmokeEffectProxy(const CRecvProxyData *pData, void *pStruct, void *pOut)
{
	/*if (cfg.Misc.noSmoke)
	{
		//*(bool*)((DWORD)pOut + 0x1) = true;
		*(float*)pOut = 257.f;
	}*/

	oDidSmokeEffect(pData, pStruct, pOut);
}