#include "SDK.h"
#include "Global.h"
#include "CreateMove.h"
#include "AimbotV2.h"
#include "AntiAimV2.h"
#include "BunnyHop.h"
#include "FakeLag.h"
#include "PredictionSystem.h"
#include "Teleport.h"
#include "MouseDeltaSimulator.h"
#include "AnimatedClanTag.h"
#include "AutoBlock.h"
#include "NameStealer.h"
#include "HUDIndicators.h"
#include "AutoWall.h"
#include "GranadePrediction.h"
#include "ConvarSpoofer.h"
#include "LegitBot.h"
#include "ESP.h"
#include "SpreadCrosshair.h"
#include "AimbotRecode.h"
CreateMove_t oCreateMove = nullptr;

bool *bSendPacket = 0;

void MovementFix(Vector viewAnglesOld, CUserCmd* pCmd, float forwardMoveOld, float sideMoveOld);

BunnyHop *bunnyHop = new BunnyHop();
FakeLag *fakeLag = new FakeLag();
Teleport *teleport = new Teleport();
MouseDeltaSimulator *mouseDeltaSimulator = new MouseDeltaSimulator();
AutoBlock *autoBlock = new AutoBlock();
NameStealer *nameStealer = new NameStealer();

bool __stdcall CreateMove(float SampleTime, CUserCmd* pCmd)
{
	if (G::exit)
		return oCreateMove(SampleTime, pCmd);
	
	bool ret = oCreateMove(SampleTime, pCmd);

	bool validState = false;

	if (I::pEngineClient && I::pClientEntityList && pCmd)
		validState = true;

	if (validState)
		validState = pCmd->command_number && I::pEngineClient->IsInGame() && I::pEngineClient->IsConnected();

	if (!I::pEngineClient->IsInGame() || !I::pEngineClient->IsConnected())
	{
		if (nameStealer->started)
		{
			nameStealer->started = false;
			nameStealer->delay = 0;
			SetName(nameStealer->bkpName);
		}
	}

	if (!validState)
	{
		G::inGameAndConnected = false;
		return ret;
	}

	G::inGameAndConnected = true;

	static PVOID pEBP = 0;
	__asm MOV pEBP, EBP
	bool *pSendPacket = (bool*)(*(PDWORD)pEBP - 0x1C);

	G::pCmd = pCmd;
	G::pLocal = I::pClientEntityList->GetClientEntity(I::pEngineClient->GetLocalPlayer());

	if (!(pEBP != nullptr))
	{
		int max = I::pEngineClient->GetMaxClients();
		for (int i = 0; i < max; i++)
		{
			CBaseEntity *entity = I::pClientEntityList->GetClientEntity(i);

			if (entity && !entity->IsDormant())
			{
				CBaseCombatWeapon *weapon = entity->GetWeapon();

				if (weapon)
				{
					WeaponInfo_t *wpnData = weapon->GetCSWpnData();

					if (wpnData)
					{
						memcpy(&G::weapInfoCache[i], wpnData, sizeof(WeaponInfo_t));
					}
				}
			}
		}
	}

	//printf("%X\n", G::pLocal->GetWeapon()->GetCSWpnData());

	/*max = I::pEngineClient->GetMaxClients();
	for (int i = 0; i < max; i++)
	{
		CBaseEntity *entity = I::pClientEntityList->GetClientEntity(i);

		if (AimbotV2::Filter(entity))
			continue;

		Backtrack::RecordData *record = Backtrack::pRecordManager->GetLatestRecord(i);

		G::boneCache[i].valid = record != NULL;

		if (record)
		{
			memcpy(G::boneCache[i].pBoneToWorldOut, record->boneMatrix, sizeof(matrix3x4_t) * MAX_BONES);
		}

		//G::boneCache[i].valid = entity->SetupBones(G::boneCache[i].pBoneToWorldOut, MAX_BONES, 0x100, I::pEngineClient->GetLastTimeStamp());
	}*/

	static bool spawned = false;
	static int spawnedTickCount = 0;

	if (G::pLocal->GetHealth() > 0)
	{
		if (!spawned)
		{
			spawnedTickCount = GetTickCount();
			spawned = true;
		}
	}
	else
	{
		spawned = false;
	}

	if (spawned)
	{
		if (GetTickCount() - spawnedTickCount < 100) // wait 100ms when respawn
		{
			return ret;
		}
	}

	QAngle oldViewAngles = pCmd->viewangles;
	static QAngle lastViewAngles = QAngle(0, 0, 0);
	int tick_count_bkp = pCmd->tick_count;

	if (lastViewAngles == QAngle(0, 0, 0))
		lastViewAngles = pCmd->viewangles;

	animatedClanTag->Tick();
	nameStealer->Tick();

	if (G::pLocal->GetLifeState() != LIFE_ALIVE || G::pLocal->GetHealth() <= 0)
		return ret;

	CBaseCombatWeapon *weapon = G::pLocal->GetWeapon();

	G::pWeapon = weapon;

	if (!weapon)
		return ret;

	aimbotV2->UpdatePointers(G::pLocal, G::pWeapon, G::pCmd);

	/*if (pCmd->buttons & IN_USE)
	{
	//antiAim->nextLowerBodyUpdate = 0;

	return ret;
	}*/

	if (cfg.Rage.noRecoil || cfg.Aimbot.silent || cfg.LegitBot.silent)
		ret = false;



	static bool chokePacket = false;
	static bool cancelChoke = false;
	static int chokeCount = 0;

	if (cfg.Visual.granadePrediction && aimbotV2->IsUsingGrenade())
	{
		GranadePrediction::pInstance->PredictNade(G::pLocal);
	}
	else
	{
		GranadePrediction::pInstance->SetPredicted(false);
	}


	QAngle viewAngles;
	I::pEngineClient->GetViewAngles(viewAngles);

	spreadCrosshair->Tick(viewAngles);

	INetChannel *net = (INetChannel*)I::pEngineClient->GetNetChannelInfo();

	bool slowMotion = cfg.Rage.AntiAim.enabled && cfg.Rage.AntiAim.slowMotionTicks != 0 && pCmd->buttons & IN_WALK && G::pLocal->CheckOnGround() && G::pLocal->GetVelocity().Length() < 500;

	int bkpOutSequenceNr = net->m_nOutSequenceNr;
	int bkpTickCount = pCmd->tick_count;
	int bkpCommandNumber = pCmd->command_number;

	/*static bool lastBreakLby = false;

	if (lastBreakLby)
	antiAim->lbyUpdate = false;

	if (antiAim->breakLby2)
	antiAim->lbyUpdate2 = false;*/

	if (!cancelChoke)
	{
		fakeLag->Tick(chokePacket);
	}
	else
	{
		cancelChoke = false;
	}

	//if (!net->CanPacket() && !fake)
	//{
	//	fake = false;

	bunnyHop->Tick(chokePacket);

	if (cfg.Rage.Resolver.prediction && G::pLocal->GetLifeState() == LIFE_ALIVE && G::pLocal->GetHealth() > 0)
	{
		predictionSystem->StartPrediction(pCmd, G::pLocal);
		//predictionSystem->Predict();

		if (G::pWeapon)
			G::pWeapon->UpdateAccuracyPenalty();
	}

	autoBlock->Tick();

	bool legitBotUsingTick = LegitBot::CreateMove(pCmd);

	if (!legitBotUsingTick)
		AimRage::Get().Work(pCmd, chokePacket);

	antiAim->choke = chokePacket;
	antiAim->lastTickAngles = lastViewAngles;

	if (cfg.Rage.AutoFire.antiSilent || aimbotV2->usingTick && aimbotV2->IsUsingRevolver() && aimbotV2->IsAbleToShot())
	{
		if (aimbotV2->usingTick)
		{
			chokePacket = false;
		}
	}

	// anti recoil fucking
	/*if (chokePacket && chokeCount >= 7 && aimbot->usingTick)
	{
	chokePacket = false;

	if (cfg.Rage.pSilent)
	{
	pCmd->buttons &= ~IN_ATTACK;
	aimbot->usingTick = false;
	}
	}*/

	if (cfg.Rage.pSilent)
	{
		if (chokeCount >= 2 && aimbotV2->usingTick)
		{
			cancelChoke = true;
		}
	}

	if (!aimbotV2->usingTick && !legitBotUsingTick)
	{
		antiAim->CreateMove(pCmd);

		if (antiAim->usingTick)
			ret = false;
	}

	if (cfg.Rage.AutoFire.autoRevolver)
	{
		if (!aimbotV2->IsAbleToShot() && aimbotV2->IsUsingRevolver() && G::pWeapon->GetClip1() > 0 && !G::pWeapon->InReload())
		{
			pCmd->buttons |= IN_ATTACK;

			/*// anti switch lock
			static int inAttackTicks = 0;
			if (pCmd->buttons & IN_ATTACK)
			{
			inAttackTicks++;
			if (inAttackTicks > 32)
			{
			pCmd->buttons &= ~IN_ATTACK;
			inAttackTicks = 0;
			}
			}
			else
			{
			inAttackTicks = 0;
			}*/
		}
	}

	//if (cfg.Rage.Resolver.noInterpolation)
		//pCmd->tick_count += TIME_TO_TICKS(Prediction::GetLerp());


	mouseDeltaSimulator->Tick(chokePacket);
	teleport->Tick(chokePacket);

	if (cfg.Rage.Resolver.prediction && G::pLocal->GetLifeState() == LIFE_ALIVE && G::pLocal->GetHealth() > 0)
	{
		predictionSystem->EndPrediction(G::pLocal);
	}

	
	
	if(cfg.Rage.AntiAim.untrusted)
	MovementFix(oldViewAngles, pCmd, pCmd->forwardmove, pCmd->sidemove);

	if (ret == false && !antiAim->usingTick && !cfg.Aimbot.silent && !cfg.LegitBot.silent)
		I::pEngineClient->SetViewAngles(pCmd->viewangles + G::pLocal->GetAimPunchAngles() * 2.0f);


	QAngle deltaViewAngles = lastViewAngles - pCmd->viewangles;
	deltaViewAngles.NormalizeAngles();

	if (cfg.Aimbot.Aimstep.enabled)
	{
		if (fabsf(deltaViewAngles.x) > cfg.Aimbot.Aimstep.ammount + 0.2f)
		{
			//printf("[WARNING] %d HIGH DELTA X (%f).\n", pCmd->tick_count, deltaViewAngles.x);
		}

		if (fabsf(deltaViewAngles.y) > cfg.Aimbot.Aimstep.ammount + 0.2f)
		{
			//printf("[WARNING] %d HIGH DELTA Y (%f).\n", pCmd->tick_count, deltaViewAngles.y);
		}
	}

	fakeLag->EndTick(chokePacket);

	if (cfg.Rage.AntiAim.viewLowerBodyYawTarget && !chokePacket)
	{
		QAngle bkp = G::pLocal->GetAbsAngles();
		G::pLocal->SetAbsAngles(Vector(bkp.x, G::pLocal->GetLowerBodyYawTarget(), bkp.z));
		G::pLocal->InvalidateBoneCache();
		G::pLocal->SetupBones(G::localPlayerLbyBones, MAX_BONES, 0x100, I::pEngineClient->GetLastTimeStamp());
		G::pLocal->SetAbsAngles(bkp);
		G::pLocal->InvalidateBoneCache();
	}

	lastViewAngles = pCmd->viewangles;

	if (chokePacket)
		hudIndicators->lowerBodyChoke = fabsf(Math::NormalizeYawInPlace(Math::NormalizeYawInPlace(G::pLocal->GetLowerBodyYawTarget()) - Math::NormalizeYawInPlace(pCmd->viewangles.y))) <= 2 * 35.0f;

	hudIndicators->lagCompensated = fakeLag->isLagCompensated;

	antiAim->breakingLby = !hudIndicators->lowerBodyChoke;
	static bool thirdperson = false;
	static bool setting = cfg.Misc.thirdPerson;
	static bool last_thirdperson_check;

	static bool check = false;
	if (GetAsyncKeyState(VK_MBUTTON))
	{
		if (!check)
			setting = !setting;
		check = true;
	}
	else
		check = false;

	if (cfg.Misc.thirdPerson != last_thirdperson_check)
	{
		setting = cfg.Misc.thirdPerson;
		last_thirdperson_check = cfg.Misc.thirdPerson;
	}

	if (cfg.Misc.thirdPerson)
	{
		if (setting)
		{
			auto svcheats = I::pCVar->FindVar("sv_cheats");
			auto svcheatsspoof = new SpoofedConvar(svcheats);
			svcheatsspoof->SetValue(1);

			I::pEngineClient->ClientCmd_Unrestricted("thirdperson");

			thirdperson = true;
		}
	}

	if (!setting && thirdperson)
	{
		I::pEngineClient->ClientCmd_Unrestricted("firstperson");
		thirdperson = false;
	}
	//lastBreakLby = antiAim->breakLby;

	/*if (chokePacket && aimbot->usingTick)
	{
	pCmd->command_number -= 1;
	//net->m_nOutSequenceNr -= 1;
	I::pClientState->lastoutgoingcommand -= 1;
	//net->m_nOutSequenceNr -= 15;
	}*/

	/*if (chokePacket && aimbot->usingTick)
	{
	//pCmd->command_number -= 16;
	//net->m_nOutSequenceNr -= 1;
	I::pClientState->lastoutgoingcommand -= 15;
	//net->m_nOutSequenceNr -= 15;
	}*/

	if (chokePacket)
		chokeCount++;
	else
		chokeCount = 0;

	//printf("chokeCount: %d\n", chokeCount);

	if (!cfg.Rage.FakeLag.enabled && !cfg.Rage.AntiAim.enabled && !aimbotV2->usingTick && !antiAim->usingTick)
		chokePacket = false;

	if (G::pLocal->IsScoped())
		G::scoped = true;
	else
		G::scoped = false;

	if (!pCmd->hasbeenpredicted)
	{
		if (G::pLocal)
		{
			G::pLocal->SetTickBase(G::pLocal->GetTickBase() + 1);
		}
	}

	antiAim->lastTickChoked = chokePacket;

	G::SendPacket = !chokePacket;
	chokePacket = !chokePacket;

	/*if (cfg.Rage.Resolver.noInterpolation)
	{
	pCmd->tick_count += TIME_TO_TICKS(Prediction::GetLerp());
	}*/

	return ret;
}


void MovementFix(Vector viewAnglesOld, CUserCmd* pCmd, float forwardMoveOld, float sideMoveOld)
{
	Math::ClampAngles(pCmd->viewangles);

	QAngle viewangles = pCmd->viewangles;

	float f1 = viewAnglesOld.y < 0.0f ? viewAnglesOld.y + 360.0f : viewAnglesOld.y;
	float f2 = viewangles.y < 0.0f ? viewangles.y + 360.0f : viewangles.y;
	float deltaView = f2 < f1 ? abs(f2 - f1) : 360.0f - abs(f1 - f2);
	deltaView = 360.0f - deltaView;

	pCmd->forwardmove = cosf(DEG2RAD(deltaView)) * forwardMoveOld + cosf(DEG2RAD(deltaView + 90.0f)) * sideMoveOld;
	pCmd->sidemove = sinf(DEG2RAD(deltaView)) * forwardMoveOld + sinf(DEG2RAD(deltaView + 90.0f)) * sideMoveOld;


	//if (viewangles.x >= 180 && viewangles.x <= 270) pCmd->forwardmove = -pCmd->forwardmove; 
	//if (viewangles.x <= -180 && viewangles.x >= -270) pCmd->forwardmove = -pCmd->forwardmove; 

	/*while (viewangles.x >= 18000000.0f)
	viewangles.x -= 36000000.0f;
	while (viewangles.x <= -18000000.0f)
	viewangles.x += 36000000.0f;
	while (viewangles.x <= -180.0f)
	viewangles.x += 360.0f;
	while (viewangles.x >= 180.0f)
	viewangles.x -= 360.0f;*/

	//if (viewangles.x >= 90 || viewangles.x <= -90) pCmd->forwardmove = -pCmd->forwardmove; 

	if (pCmd->forwardmove > 450.0f) pCmd->forwardmove = 450.0f;
	if (pCmd->forwardmove < -450.0f) pCmd->forwardmove = -450.0f;
	if (pCmd->sidemove > 450.0f) pCmd->sidemove = 450.0f;
	if (pCmd->sidemove < -450.0f) pCmd->sidemove = -450.0f;
}