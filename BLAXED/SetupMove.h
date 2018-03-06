#pragma once

typedef void(__thiscall *SetupMove_t)(IPrediction *this_, CBaseEntity* player, CUserCmd* ucmd, IMoveHelper* pHelper, CMoveData* move);
typedef void(__thiscall *ProcessMovement_t)(IGameMovement *this_, CBaseEntity* pPlayer, CMoveData* pMove);

extern SetupMove_t oSetupMove;
extern ProcessMovement_t oProcessMovement;

extern void __fastcall SetupMove(IPrediction *this_, void *edx, CBaseEntity* player, CUserCmd* ucmd, IMoveHelper* pHelper, CMoveData* move);
extern void __fastcall ProcessMovement(IGameMovement *this_, void *edx, CBaseEntity *pPlayer, CMoveData *pMove);
