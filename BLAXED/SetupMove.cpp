#include "SDK.h"
#include "Global.h"
#include "SetupMove.h"
#include "Hooks.h"

SetupMove_t oSetupMove = NULL;
ProcessMovement_t oProcessMovement = NULL;

void __fastcall SetupMove(IPrediction *this_, void *edx, CBaseEntity* player, CUserCmd* ucmd, IMoveHelper* pHelper, CMoveData* move)
{
	oSetupMove(this_, player, ucmd, pHelper, move);
}

extern void __fastcall ProcessMovement(IGameMovement *this_, void *edx, CBaseEntity *pPlayer, CMoveData *pMove)
{	
	oProcessMovement(this_, pPlayer, pMove);

	pMove->m_outStepHeight = 0.0f;
}

 