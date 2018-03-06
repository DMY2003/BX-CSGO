#pragma once

#define CHECK_VALID( _v ) 0
#define Assert( _exp ) ((void)0)

typedef __int16					int16;
typedef unsigned __int16		uint16;
typedef __int32					int32;
typedef unsigned __int32		uint32;
typedef __int64					int64;
typedef unsigned __int64		uint64;

#include "main.h"
#include "VMT.h"
#include "Configs.h"
#include "Util.h"
#include "Math.h"
#include "Vector.h"
#include "Misc.h"
#include "VMatrix.h"
#include "CUserCmd.h"
#include "CViewSetup.h"
#include "IPlayerAnimState.h"
#include "CBaseEntity.h"
#include "CBaseCombatWeapon.h"
#include "ICVar.h"
#include "ConVar.h"
#include "IEngineClient.h"
#include "IClientEntityList.h"
#include "CGlobalVarsBase.h"
#include "EngineTrace.h"
#include "ModelInfo.h"
#include "IPhysicsSurfaceProps.h"
#include "CBaseAttributableItem.h"
#include "IBaseClientDll.h"
#include "CBaseViewModel.h"
#include "INetChannel.h"
#include "CMoveData.h"
#include "IMoveHelper.h"
#include "IGameMovement.h"
#include "IPrediction.h"
#include "IMaterial.h"
#include "IMaterialSystem.h"
#include "IVModelRender.h"
#include "IVRenderView.h"
#include "IGameEvent.h"
#include "CInput.h"
#include "CClientState.h"
#include "IPanel.h"
#include "ISurface.h"
#include "C_TEFireBullets.h"
#include "IEffects.h"
#include "CGlowObjectManager.h"
#include "CCSGameRulesProxy.h"
#include "checksum.h"
#include "IStudioRender.h"
#include "IClientRenderable.h"
#include "datamap.h"
#include "Interfaces.h"

#include "NET_SetConvar.h"

#include "SpoofedConvar.hpp"

