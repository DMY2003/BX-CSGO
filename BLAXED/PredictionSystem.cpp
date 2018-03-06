#include "SDK.h"
#include "Global.h"
#include "PredictionSystem.h"
#include "Prediction.h"
int* predictionRandomSeed = NULL;
CBaseEntity* predictionPlayer = NULL;
CPredictionSystem *predictionSystem = new CPredictionSystem();
void CPredictionSystem::Init() {
	predictionRandomSeed = *(int**)(Mem::FindPattern(xorstr("client.dll"), xorstr("8B 0D ? ? ? ? BA ? ? ? ? E8 ? ? ? ? 83 C4 04")) + 2);
	//predictionPlayer = *reinterpret_cast<CBaseEntity**>(Mem::FindPattern(xorstr("client.dll"), xorstr("89 3D ? ? ? ? F3 0F 10 48")) + 2);
}
void CPredictionSystem::StartPrediction(CUserCmd *pCmd, CBaseEntity *entity)
{
	*predictionRandomSeed = MD5_PseudoRandom(pCmd->command_number) & 0x7FFFFFFF;
	//predictionPlayer = entity;

	m_flOldCurTime = I::pGlobals->curtime;
	m_flOldFrametime = I::pGlobals->frametime;

	I::pGlobals->curtime = entity->GetTickBase() * I::pGlobals->interval_per_tick;
	I::pGlobals->frametime = I::pGlobals->interval_per_tick;

	Vector oldVelocity = entity->GetVelocity();

	//Here we're doing CBasePlayer::UpdateButtonState
	moveData.m_nButtons = pCmd->buttons;
	int buttonsChanged = pCmd->buttons ^ *reinterpret_cast<int*>(uintptr_t(entity) + 0x31E8);
	*reinterpret_cast<int*>(uintptr_t(entity) + 0x31DC) = (uintptr_t(entity) + 0x31E8);
	*reinterpret_cast<int*>(uintptr_t(entity) + 0x31E8) = pCmd->buttons;
	*reinterpret_cast<int*>(uintptr_t(entity) + 0x31E0) = pCmd->buttons & buttonsChanged;  //m_afButtonPressed ~ The changed ones still down are "pressed"
	*reinterpret_cast<int*>(uintptr_t(entity) + 0x31E4) = buttonsChanged & ~pCmd->buttons; //m_afButtonReleased ~ The ones not down are "released"

	I::pGameMovement->StartTrackPredictionErrors(entity);

	memset(&moveData, 0, sizeof(CMoveData));
	I::pMoveHelper->SetHost(entity);
	I::pPrediction->SetupMove(entity, pCmd, I::pMoveHelper, &moveData);
	I::pGameMovement->ProcessMovement(entity, &moveData);
	I::pPrediction->FinishMove(entity, pCmd, &moveData);

	entity->SetVelocity(oldVelocity);
}

void CPredictionSystem::Predict()
{
	
}

void CPredictionSystem::EndPrediction(CBaseEntity *entity)
{
	I::pGameMovement->FinishTrackPredictionErrors(entity);
	I::pMoveHelper->SetHost(nullptr);

	*predictionRandomSeed = -1;
	//predictionPlayer = nullptr;

	I::pGlobals->curtime = m_flOldCurTime;
	I::pGlobals->frametime = m_flOldFrametime;
}
