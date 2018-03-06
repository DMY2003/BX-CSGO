#include "ResolverV2.h"
#include "AimbotV2.h"
#include "HitMarkers.h"
#include "MenuV2.h"
#include "AutoWall.h"
#include "LagCompensation.h"

namespace ResolverV2
{
	CBaseEntity *lastShotEntity = NULL;
	Vector lastShotEyePosition = Vector(0, 0, 0);
	Vector lastShotPoint = Vector(0, 0, 0);
	int lastShotHitbox = 0;
	bool impactAcked = false;

	ResolverFlags lastShotResolverFlags;
	BruteforceInfo lastShotResolverBruteforce;

	RecvVarProxyFn oRecvLowerBodyYawTarget = NULL;
	RecvVarProxyFn oRecvEyeAnglesX = NULL;
	RecvVarProxyFn oRecvEyeAnglesY = NULL;
	RecvVarProxyFn oRecvEyeAnglesZ = NULL;

	void CResolver::Init(CBaseEntity *entity)
	{
		Clear();
		initialized = true;
		this->entity = entity;
		lastResolverMethod = LBY_UPDATE;
	}

	void CResolver::Clear()
	{
		entity = nullptr;
		initialized = false;
		ClearFlags();
		ClearPerTickInfo();
		//aaRec.Clear();
	}

	void CResolver::ClearBruteforce()
	{
		for (int i = 0; i < BF_STEP_COUNT; i++)
		{
			bruteforce.step[i] = 0;
			bruteforce.changeStep[i] = false;
		}

		bruteforce.missedCount = 0;

		movingLby = 0.0f;
	}

	void CResolver::ClearFlags()
	{
		flags.fakeWalking = false;
		flags.standing = false;
		flags.moving = false;
		flags.crouchPicking = false;
		flags.fakingLby = false;
		flags.inAir = false;
		flags.onGround = false;
		flags.lbyUpdated = false;
		flags.lbyGoingUpdate = false;
		flags.fakingYaw = false;
		flags.unresolved = false;
		flags.legit = true;
		flags.ducking = false;
	}

	void CResolver::ClearPerTickInfo()
	{
		lastSimulationTime = 0;
		nextLbyUpdateTime = INFINITY;
		lastLby = 0;
		lastGroundMoveTime = 0;
		lastBalanceAdjust = INFINITY;

		oLby = INFINITY;
		lastPitch = INFINITY;
		fixingPitch = INFINITY;
		receivedFirstUpdate = false;
	}
	void CResolver::OnFrameRenderStart()
	{

	}
	
	void CResolver::OnEntityUpdate()
	{
		if (cfg.Rage.Resolver.forceYaw)
		{
			auto records = aaRec.m_sRecords;

			if (aaRec.m_sRecords.size() == 0 || entity->GetSimulationTime() != aaRec.m_sRecords.front()->m_flSimulationTime)
			{
				aaRec.StoreVars(entity);
			}
		}

		oDuckAmmount = entity->GetDuckAmount();
		oVelocity = entity->GetVelocity();
		oAbsVelocity = entity->GetAbsVelocity();
		oOrigin = entity->GetOrigin();
		oAbsOrigin = entity->GetAbsOrigin();

		oLby = entity->GetLowerBodyYawTarget();

		/*
		oEyeAngles = entity->GetEyeAngles();
		oLby = entity->GetLowerBodyYawTarget();
		*/

		ClearFlags();

		float speed = entity->GetVelocity().Length();
		float pitch = Math::NormalizeYawInPlace(oEyeAngles.x);

		flags.inAir = !entity->CheckOnGround();
		flags.onGround = !flags.inAir;
		flags.standing = flags.onGround && speed < 1.0f;
		flags.moving = speed > 34.0f;

		//TODO : CHECKED FOR CHOCKED - POSSIBLE LOGIC ERROR"!!!!!!!!!!!!!!
		CAnimationLayer *layer6 = entity->GetAnimOverlay(6);
		//int chockedticks;
		//Backtrack::RecordData *record = Backtrack::pRecordManager->GetLatestRecord(entity);
		//chockedticks = TIME_TO_TICKS(entity->GetSimulationTime() - record->GetSimulationTime());
		if (layer6 && layer6->m_flPlaybackRate > 0.0f && layer6->m_flPlaybackRate < 0.008f && speed <= 34.0f)
			flags.fakeWalking = true; // chockedticks ? true : false;
		else
			flags.fakeWalking = false;

		//fakeWalking = onGround && speed >= 1.0f && speed <= 34.0f && lastGroundMoveTime > TICKS_TO_TIME(1);
		flags.crouchPicking = entity->GetDuckAmount() > 0.1f && entity->GetDuckAmount() < 0.85f;
		flags.ducking = entity->GetDuckAmount() > 0 && flags.onGround;

		if (pitch != lastPitch)
		{
			fixPitch = lastPitch != INFINITY && lastPitch >= 35.0f && pitch < 35.0f;

			if (fixPitch)
			{
				fixingPitch = lastPitch;
			}
			else
			{
				fixingPitch = INFINITY;
			}
		}

		flags.fakingYaw = flags.inAir || fabsf(Math::NormalizeYawInPlace(oLby - entity->GetEyeAngles().y)) >= 35.0f;
		//fakingLby = inAir || fakeWalking || standing;

		if (cfg.Rage.Resolver.noSpreadMode || cfg.Rage.Resolver.resolveLegits)
			flags.legit = false;
		else
			flags.legit = flags.onGround && !flags.fakingYaw && Math::NormalizeYawInPlace(entity->GetEyeAngles().x) < 35.0f;

		flags.lbyPredicted = false;

		float curtime = entity->GetSimulationTime(); //TICKS_TO_TIME(I::pEngineClient->GetServerTick());

		bool lbyChanged = receivedFirstUpdate && (lastLby != oLby); //(fabsf(Math::NormalizeYawInPlace(lastLby - oLby)) >= 35.0f));

		flags.lbyUpdated = false;
		/*
		if (lastBalanceAdjust == INFINITY)
			lastBalanceAdjust = curtime;

		//static int t = 0;
		int layerCount = entity->GetNumAnimOverlays();
		for (int layerIndex = 0; layerIndex < layerCount; ++layerIndex)
		{
			CAnimationLayer *currentLayer = entity->GetAnimOverlay(layerIndex);

			if (currentLayer)
			{
				const char *seqName = entity->GetSequenceActivityName(currentLayer->m_nSequence);

				if (strstr(seqName, xorstr("ACT_CSGO_IDLE_TURN_BALANCEADJUST")))
				{
					if (currentLayer->m_flCycle == 0.0f)// && currentLayer->m_flWeight == 0.0f)
					{
						//printf("%d %f %f\n", TIME_TO_TICKS(curtime), currentLayer->m_flCycle, currentLayer->m_flWeight);
						lastBalanceAdjust = curtime;
					}

					break;
				}
			}
		}

		flags.fakingLby = !flags.moving && (curtime - lastBalanceAdjust <= (1.1f + TICKS_TO_TIME(1)) && curtime - lastBalanceAdjust >= 0 || bruteforce.missedCount > 1) || flags.fakeWalking || !flags.onGround;
		*/

		if (flags.moving && flags.onGround)
			flags.lbyUpdated = true;

		if (flags.moving && flags.onGround || !flags.fakingYaw)
		{
			lastNonBreakedLby = oLby;
		}

		if (flags.moving && flags.onGround)
		{
			movingLby = oLby;
			lastGroundMoveTime = curtime;
		}

		if (fabsf(entity->GetSimulationTime() - lastGroundMoveTime) > 1000.0f)
			lastGroundMoveTime = entity->GetSimulationTime() - 200.0f;

		if (lbyChanged)
			lastLbyUpdateTime = entity->GetSimulationTime();

		lastLby = oLby;
		lastPitch = pitch;

		receivedFirstUpdate = true;
	}
	
	void CResolver::ProcessAngles()
	{
		if (flags.onGround && (flags.lbyUpdated || flags.legit))
		{
			flags.unresolved = false;
		}
		else
		{
			flags.unresolved = true;
		}

		bool missed = false;

		for (int i = 0; i < BF_STEP_COUNT; i++)
		{
			if (bruteforce.changeStep[i])
			{
				bruteforce.changeStep[i] = false;
				bruteforce.step[i]++;
				missed = true;
			}
		}

		if (missed)
			bruteforce.missedCount++;
	}
	
	void CResolver::ApplyAngles()
	{
		if (!flags.legit)
		{
			rEyeAngles = entity->GetEyeAngles();//oEyeAngles;

			if (cfg.Rage.Resolver.lispCorrect)
			{
				rEyeAngles.y = oLby;
			}

			if (fixPitch)
			{
				rEyeAngles.x = fixingPitch;
			}

			if (cfg.Rage.Resolver.forceYaw)
			{
				float lastMoveLby = movingLby != 0.0f ? movingLby : oLby + 180.0f;
				float atTarget = Math::CalcAngle(entity->GetAbsOrigin(), G::pLocal->GetAbsOrigin()).y + 180.0f;

				CAnimationLayer curBalanceLayer, prevBalanceLayer;
				auto cur = aaRec.m_sRecords;

				if (aaRec.m_sRecords.size() > 1 && aaRec.prevTickRecord && aaRec.curTickRecord)
				{
					///////////////////////////////////////////////////////////// DELTAS!!!!!!!!!  /////////////////////////////////////////////////////////////
					float bodyeyedelta = entity->GetEyeAngles().y - cur.front()->m_flLowerBodyYawTarget;
					float flLowerBodyYaw = entity->GetLowerBodyYawTarget();
					float flOriginalAngle = Math::NormalizeYawResolver(entity->GetEyeAngles().y);
					float lbydelta = flOriginalAngle - flLowerBodyYaw;
					float CurrentLowerbody = entity->GetLowerBodyYawTarget();
					///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

					flags.fakingLby =
						aaRec.IsAdjustingBalance(entity, *aaRec.curTickRecord, &curBalanceLayer) &&
						aaRec.IsAdjustingBalance(entity, *aaRec.prevTickRecord, &prevBalanceLayer) &&
						prevBalanceLayer.m_flCycle != curBalanceLayer.m_flCycle || curBalanceLayer.m_flWeight == 1.f;

					/*float
						flAnimTime = curBalanceLayer.m_flCycle,
						flSimTime = entity->GetSimulationTime();

					if (flAnimTime < 0.01f && prevBalanceLayer.m_flCycle > 0.01f && cfg.Rage.Resolver.backTrack && CMBacktracking::Get().IsTickValid(TIME_TO_TICKS(flSimTime - flAnimTime)))
					{
						CMBacktracking::Get().SetOverwriteTick(entity, QAngle(entity->GetEyeAngles().x, entity->GetLowerBodyYawTarget(), 0), (flSimTime - flAnimTime), 1);
					}*/

					bool doBruteForce = bruteforce.missedCount >= cfg.Rage.Resolver.BruteForceShots;
					
					if (flags.onGround)
					{
						if (aaRec.IsEntityMoving(entity))
						{
							// moving resolver

							if (flags.fakeWalking)
							{
								// fake walk resolver

								if (aaRec.DeltaKeepsChanging(cur, 10.f))
								{
									rEyeAngles.y = entity->GetEyeAngles().y - aaRec.GetDeltaByComparingTicks(cur);
									lastResolverMethod = DELTA_CHANGE;
								}
								else if (aaRec.LBYKeepsChanging(cur, 10.f))
								{
									rEyeAngles.y = aaRec.GetLBYByComparingTicks(cur);
									lastResolverMethod = LBY_CHANGE;
								}
								else
								{
									doBruteForce = true;
								}
							}
							else
							{
								// moving resolver

								lastResolverMethod = MOVING_LBY;
								rEyeAngles.y = entity->GetLowerBodyYawTarget();
							}
						}
						else
						{
							// standing resolver

							if (flags.fakingLby)
							{
								// fake lby resolver

								if (std::abs(lbydelta - aaRec.GetDeltaByComparingTicks(cur)) <= 10)
								{
									rEyeAngles.y = Math::NormalizeYawResolver(flOriginalAngle - lbydelta);
									lastResolverMethod = LBY_TICKS;
								}
								else if (lbydelta < 35 && lbydelta > -35)
								{
									rEyeAngles.y = aaRec.GetDeltaByComparingTicks(cur) + 180;
									lastResolverMethod = LBY_CALCULATE;
								}
								else if (fabsf(bodyeyedelta) < 35.0f && fabsf(bodyeyedelta) > 0.0f)
								{
									rEyeAngles.y = cur.front()->m_flLowerBodyYawTarget + bodyeyedelta;
									lastResolverMethod = BODYEYE_DELTA;
								}
								else
								{
									doBruteForce = true;
								}
							}
							else
							{
								// standing resolver

								if (std::abs(lbydelta - aaRec.GetDeltaByComparingTicks(cur)) <= 10)
								{
									rEyeAngles.y = Math::NormalizeYawInPlace(flOriginalAngle - lbydelta);
									lastResolverMethod = LBY_TICKS;
								}
								else if (lbydelta < 35 && lbydelta > -35)
								{
									rEyeAngles.y = aaRec.GetDeltaByComparingTicks(cur) + 180;
									lastResolverMethod = LBY_CALCULATE;
								}
								else if (fabsf(bodyeyedelta) < 35.0f && fabsf(bodyeyedelta) > 0.0f)
								{
									rEyeAngles.y = cur.front()->m_flLowerBodyYawTarget + bodyeyedelta;
									lastResolverMethod = BODYEYE_DELTA;
								}
								else if (aaRec.HasStaticRealAngle(cur, 15.f))
								{
									rEyeAngles.y = (cur.front()->m_flLowerBodyYawTarget) + (Math::RandomFloat(0.f, 1.f) > 0.5f ? 10 : -10);
									lastResolverMethod = STATIC_YAW;
								}
								else if (aaRec.HasStaticYawDifference(cur))
								{
									rEyeAngles.y = entity->GetEyeAngles().y - (cur.front()->m_angEyeAngless.y - cur.front()->m_flLowerBodyYawTarget);
									lastResolverMethod = STATIC_YAW_FAKE;
								}
								else if (aaRec.DeltaKeepsChanging(cur))
								{
									rEyeAngles.y = entity->GetEyeAngles().y - aaRec.GetDeltaByComparingTicks(cur);
									lastResolverMethod = DELTA_CHANGE;
								}
								else if (aaRec.LBYKeepsChanging(cur))
								{
									rEyeAngles.y = aaRec.GetLBYByComparingTicks(cur);
									lastResolverMethod = LBY_CHANGE;
								}
								else
								{
									doBruteForce = true;
								}
							}
						}
					}				
					else
					{
						//TODO: air resolver
						doBruteForce = true;
					}

					// we can't resolve, do bruteforce
					if (doBruteForce)
					{
						std::vector<float> yaw;

						if (flags.fakeWalking)
						{
							QAngle velAngles;
							Math::VectorAngles(oVelocity, velAngles);
							velAngles.NormalizeAngles();

							yaw.push_back(lastMoveLby);
							yaw.push_back(velAngles.y + 180.0f);
							yaw.push_back(lastMoveLby + 180.0f);
							yaw.push_back(oLby);
	
							lastResolverMethod = FAKE_WALK;
						}
						else
						{
							yaw.push_back(lastMoveLby);
							yaw.push_back(lastMoveLby + 180);
							yaw.push_back(lastMoveLby - 35);
							yaw.push_back(lastMoveLby + 90);
							yaw.push_back(lastMoveLby - 90);
							yaw.push_back(lastMoveLby + 35);

							lastResolverMethod = LBY_OVERRIDE;
						}

						if (flags.fakeWalking && flags.onGround)
							rEyeAngles.y = yaw[bruteforce.step[BF_STEP_YAW_FAKEWALK] % yaw.size()];
						else if (flags.ducking && flags.onGround)
							rEyeAngles.y = yaw[bruteforce.step[BF_STEP_YAW_DUCKED] % yaw.size()];
						else if (flags.onGround)
							rEyeAngles.y = yaw[bruteforce.step[BF_STEP_YAW_STANDING] % yaw.size()];
						else if (!flags.onGround)
							rEyeAngles.y = yaw[bruteforce.step[BF_STEP_YAW_AIR] % yaw.size()];						
					}

					// if lby updatedd we can override the resolver, because we resolved
					if (flags.lbyUpdated)
					{
						rEyeAngles.y = entity->GetLowerBodyYawTarget();
						lastResolverMethod = LBY_UPDATE;
					}
				}
			}
			else
			{
				ClearBruteforce();
			}

			if (cfg.Rage.Resolver.fixPitch)// && missedCount >= cfg.Rage.Resolver.minBruteforceBullets)
			{
				bool fake0 = false;
				bool legit = false;

				/*if (oEyeAngles.x <= 89.0f && oEyeAngles.x >= -89.0f)
				{
				legit = true;
				}

				if (oEyeAngles.x > -35.0f && oEyeAngles.x < 35.0f)
				{
				fake0 = true;
				}

				if (!legit || fake0)*/
				{
					switch (bruteforce.step[BF_STEP_YAW_PITCH] % 8)
					{
					case 0: rEyeAngles.x = 89.0f; break;
					case 1: rEyeAngles.x = 89.0f; break;
					case 2: rEyeAngles.x = 89.0f; break;
					case 3: rEyeAngles.x = 0.0f; break;
					case 4: rEyeAngles.x = 89.0f; break;
					case 5: rEyeAngles.x = 89.0f; break;
					case 6: rEyeAngles.x = -89.0f; break;
					case 7: rEyeAngles.x = 89.0f; break;
					}
				}
			}

			if (cfg.Rage.Resolver.inverseUnresolved)
			{
				if (!flags.lbyUpdated)
				{
					rEyeAngles.y += 180.0f;
				}
			}

			switch (plPitch)
			{
			case PLP_FORCE_DOWN: rEyeAngles.x = 89.0f; break;
			case PLP_FORCE_ZERO: rEyeAngles.x = 0.0f; break;
			case PLP_FORCE_UP: rEyeAngles.x = -89.0f; break;
			}

			switch (plYaw)
			{
			case PLY_INVERSE: rEyeAngles.y += 180.0f; break;
			case PLY_INVERSE_NO_MOVE: if (entity->GetVelocity().Length() < 1.0f) rEyeAngles.y += 180.0f; break;
			case PLY_LEFT: rEyeAngles.y += 90.0f; break;
			case PLY_RIGHT: rEyeAngles.y += -90.0f; break;
			}


			rEyeAngles.NormalizeAngles();
			rEyeAngles.Clamp();

			entity->SetEyeAngles(rEyeAngles);
		}
	}


	bool CResolver::IsValid()
	{
		if (!entity) return false;
		if (!initialized) return false;

		return true;
	}


	std::array<CResolver, MAX_PLAYERS> resolverInfo;
	CResolver dummyResolver;

	CResolver &GetResolver(int index)
	{
		if (index < 0 || index >= MAX_PLAYERS)
			return dummyResolver;

		return resolverInfo[index];
	}

	bool Filter(CBaseEntity *entity)
	{
		if (!entity) return true;
		if (entity == G::pLocal) return true;
		if (entity->IsAlive() == false) return true;
		if (entity->GetTeam() != 2 && entity->GetTeam() != 3) return true;
		if (entity->GetClientClass()->m_ClassID != _CCSPlayer) return true; // !CCSPlayer
		if (entity->IsDormant()) return true;
		if (entity->GetIndex() >= MAX_PLAYERS) return true;
		if (entity->GetIndex() < 0) return true;

		if (AimbotV2::Filter(entity)) return true;

		return false;
	}

	void FrameStageNotifyPreReal(ClientFrameStage_t stage)
	{
		if (!cfg.Rage.Resolver.enabled)
			return;

		int max = I::pEngineClient->GetMaxClients();

		for (int i = 0; i < max; i++)
		{
			CBaseEntity *entity = I::pClientEntityList->GetClientEntity(i);

			if (Filter(entity))
			{
				CResolver& resolver = GetResolver(i);
				resolver.ClearPerTickInfo();

				if (entity && !entity->IsAlive())
				{
					resolver.Clear();
				}

				continue;
			}

			CResolver& resolver = GetResolver(i);

			entity->PVSFix();

			if (stage == FRAME_NET_UPDATE_POSTDATAUPDATE_START)
			{
				CResolver& resolver = GetResolver(i);

				// initialize resolver
				if (!resolver.initialized || resolver.entity != entity)
				{
					resolver.Init(entity);
				}

				resolver.entity = entity;

				// entity updated
				resolver.OnEntityUpdate();

				resolver.ProcessAngles();
				resolver.ApplyAngles();

				resolver.lastSimulationTime = entity->GetSimulationTime();
			}
			else if (stage == FRAME_RENDER_START)
			{
				if (resolver.initialized)
				{
					resolver.OnFrameRenderStart();
				}
			}
		}
	}

	void CreateMove(CUserCmd *pCmd)
	{
		dummyResolver.initialized = false;

		if (!cfg.Rage.Resolver.enabled)
			return;
	}

	void FrameStageNotifyPostReal(ClientFrameStage_t stage)
	{
		dummyResolver.initialized = false;

		if (!cfg.Rage.Resolver.enabled)
			return;

	}

	void OnShoot(Vector from, Vector to, CBaseEntity *entity)
	{
		static float last = 0;
		CBaseCombatWeapon *pWeapon = G::pLocal->GetWeapon();

		if (pWeapon && pWeapon->GetNextPrimaryAttack() != last)
		{
			CResolver& resolver = GetResolver(entity->GetIndex());

			if (resolver.IsValid())
			{
				lastShotEntity = entity;
				lastShotEyePosition = from;
				lastShotPoint = to;
			}
		}

		last = pWeapon->GetNextPrimaryAttack();
	}

	void HandleGameEvent(IGameEvent *pEvent)
	{
		if (!cfg.Rage.Resolver.enabled)
			return;

		const char *name = pEvent->GetName();

		static uint8_t colorRed[3] = { 255, 100, 100 };
		static uint8_t colorGreen[3] = { 100, 255, 100 };

		if (strstr(name, xorstr("round_start")))
		{
			lastShotEntity = NULL;
			lastShotEyePosition = Vector(0, 0, 0);
			lastShotPoint = Vector(0, 0, 0);
			lastShotResolverFlags = ResolverFlags();
			lastShotResolverBruteforce = BruteforceInfo();

			for (int i = 0; i < I::pGlobals->maxClients; i++)
			{
				CBaseEntity *entity = I::pClientEntityList->GetClientEntity(i);

				if (!cfg.Rage.Resolver.noSpreadMode)
				{
					CResolver& resolver = GetResolver(i);
					resolver.Clear();
					resolver.ClearBruteforce();
				}
			}
		}
		else if (!strcmp(name, xorstr("player_hurt")))
		{
			int userid = pEvent->GetInt(xorstr("userid"));
			int attackerid = pEvent->GetInt(xorstr("attacker"));
			int dmg_health = pEvent->GetInt(xorstr("dmg_health"));
			int hitgroup = pEvent->GetInt(xorstr("hitgroup"));

			CBaseEntity *entity = I::pClientEntityList->GetClientEntity(I::pEngineClient->GetPlayerForUserID(userid));
			CBaseEntity *attacker = I::pClientEntityList->GetClientEntity(I::pEngineClient->GetPlayerForUserID(attackerid));


			if (!entity || !attacker || attacker != G::pLocal || Filter(entity)) return;

			CResolver& resolver = GetResolver(entity->GetIndex());

			if (!resolver.IsValid())
				return;

			if (lastShotResolverFlags.fakeWalking)
				resolver.bruteforce.changeStep[ResolverV2::BF_STEP_YAW_FAKEWALK] = false;
			else if (lastShotResolverFlags.ducking)
				resolver.bruteforce.changeStep[ResolverV2::BF_STEP_YAW_DUCKED] = false;
			else if (lastShotResolverFlags.onGround)
				resolver.bruteforce.changeStep[ResolverV2::BF_STEP_YAW_STANDING] = false;
			else
				resolver.bruteforce.changeStep[ResolverV2::BF_STEP_YAW_AIR] = false;

			resolver.bruteforce.changeStep[ResolverV2::BF_STEP_YAW_PITCH] = false;

			player_info_t pinfo;
			if (I::pEngineClient->GetPlayerInfo(resolver.entity->GetIndex(), &pinfo))
			{
				std::string hitboxName = "";

				switch (hitgroup)
				{
				case 1: hitboxName = std::string(xorstr("HS")); break;
				case 2: hitboxName = std::string(xorstr("UPPER TORSO")); break;
				case 3: hitboxName = std::string(xorstr("LOWER TORSO")); break;
				case 4: hitboxName = std::string(xorstr("LEFT ARM")); break;
				case 5: hitboxName = std::string(xorstr("RIGHT ARM")); break;
				case 6: hitboxName = std::string(xorstr("LEFT LEG")); break;
				case 7: hitboxName = std::string(xorstr("RIGHT LEG")); break;
				}

				I::pCVar->ConsolePrintf("%s", xorstr("[BLAXED] "));
				I::pCVar->ConsolePrintf("%s %s %s %d%s%s%s", xorstr("  HIT"), MenuV2::SanitizeName(pinfo.m_szPlayerName).c_str(), xorstr("damage given:"), dmg_health, xorstr(". ["), hitboxName.c_str(), xorstr("].\n"));
			}
		}
		else if (!strcmp(name, xorstr("bullet_impact")))
		{
			int userid = pEvent->GetInt(xorstr("userid"));

			CBaseEntity *entity = I::pClientEntityList->GetClientEntity(I::pEngineClient->GetPlayerForUserID(userid));

			if (!entity || entity != G::pLocal || Filter(lastShotEntity))
				return;

			CResolver& resolver = GetResolver(lastShotEntity->GetIndex());

			if (!resolver.IsValid())
				return;

			if (impactAcked)
				return;

			impactAcked = true;

			float x = pEvent->GetFloat(xorstr("x"));
			float y = pEvent->GetFloat(xorstr("y"));
			float z = pEvent->GetFloat(xorstr("z"));

			Vector point = Vector(x, y, z);

			bool fail = false;

			if (!cfg.Rage.Resolver.noSpreadMode)
			{
				QAngle anglesReal = Math::CalcAngle(lastShotEyePosition, point);

				float distance = (lastShotPoint - lastShotEyePosition).Length();

				Vector simulatedPoint;
				Math::AngleVectors(anglesReal, simulatedPoint);
				simulatedPoint.NormalizeInPlace();
				simulatedPoint = lastShotEyePosition + simulatedPoint * distance;

				float delta = (simulatedPoint - lastShotPoint).Length();
				float impactDist = (lastShotPoint - point).Length();

				float damage = AutoWall::GetDamageEx(simulatedPoint, lastShotEyePosition, lastShotEntity, G::pLocal, lastShotHitbox);

				FireBulletData data;
				AutoWall::GetDamageEx(simulatedPoint, lastShotEyePosition, lastShotEntity, G::pLocal, lastShotHitbox, &data);

				float delta2 = (data.enter_trace.endpos - lastShotPoint).Length();


				if (damage <= 0.0f || (delta >= 5.0f || delta2 > 32.0f))
				{
					lastShotResolverBruteforce.missedBySpread = true;

					player_info_t pinfo;
					if (I::pEngineClient->GetPlayerInfo(resolver.entity->GetIndex(), &pinfo))
					{
						I::pCVar->ConsolePrintf("%s", xorstr("[BLAXED] "));
						I::pCVar->ConsolePrintf("%s %s %s [#%0.f#%.0f#%.0f]\n", xorstr("Missed"), MenuV2::SanitizeName(pinfo.m_szPlayerName).c_str(), xorstr("due to spread."), delta, delta2, damage);
					}
				}
			}
			else
			{
				lastShotResolverBruteforce.missedBySpread = false;
			}

			if (!lastShotResolverBruteforce.missedBySpread)
			{
				if (!lastShotResolverFlags.lbyUpdated)
				{
					if (lastShotResolverFlags.fakeWalking)
						resolver.bruteforce.changeStep[ResolverV2::BF_STEP_YAW_FAKEWALK] = true;
					else if (lastShotResolverFlags.ducking)
						resolver.bruteforce.changeStep[ResolverV2::BF_STEP_YAW_DUCKED] = true;
					else if (lastShotResolverFlags.onGround)
						resolver.bruteforce.changeStep[ResolverV2::BF_STEP_YAW_STANDING] = true;
					else
						resolver.bruteforce.changeStep[ResolverV2::BF_STEP_YAW_AIR] = true;

					resolver.bruteforce.changeStep[ResolverV2::BF_STEP_YAW_PITCH] = true;
				}
			}
		}
		else if (!strcmp(name, xorstr("weapon_fire")))
		{
			int userid = pEvent->GetInt(xorstr("userid"));

			CBaseEntity *entity = I::pClientEntityList->GetClientEntity(I::pEngineClient->GetPlayerForUserID(userid));

			if (!entity || entity != G::pLocal || Filter(lastShotEntity))
				return;

			CResolver& resolver = GetResolver(lastShotEntity->GetIndex());

			if (!resolver.IsValid())
				return;

			impactAcked = false;

			lastShotResolverBruteforce.missedBySpread = false;

			Vector from = lastShotEyePosition;
			Vector to = lastShotPoint;

			player_info_t pinfo;
			if (I::pEngineClient->GetPlayerInfo(resolver.entity->GetIndex(), &pinfo))
			{
				std::string rflag = "";
				static uint8_t color[3] = { 100, 100, 255 };

				pinfo.m_szPlayerName[127] = '\0';

				/*rflag += xorstr("|M") + std::to_string((int)lastShotResolver.moving);
				rflag += xorstr("|W") + std::to_string((int)lastShotResolver.fakeWalking);
				rflag += xorstr("|D") + std::to_string((int)lastShotResolver.ducking);
				rflag += xorstr("|U") + std::to_string((int)lastShotResolver.lbyUpdated);
				rflag += xorstr("|P") + std::to_string((int)lastShotResolver.lbyPredicted);
				rflag += xorstr("|ST") + std::to_string((int)lastShotResolver.step[ResolverV2::CResolver::BF_STEP_YAW_STANDING]);
				rflag += xorstr("|FW") + std::to_string((int)lastShotResolver.step[ResolverV2::CResolver::BF_STEP_YAW_FAKEWALK]);
				rflag += xorstr("|AI") + std::to_string((int)lastShotResolver.step[ResolverV2::CResolver::BF_STEP_YAW_AIR]);
				rflag += xorstr("|DU") + std::to_string((int)lastShotResolver.step[ResolverV2::CResolver::BF_STEP_YAW_DUCKED]);
				rflag += xorstr("|D") + std::to_string((int)(G::pLocal->GetEyePosition() - lastShotPoint).Length());
				rflag += xorstr("|MI") + std::to_string((int)(lastShotResolver.missedCount));*/

				I::pCVar->ConsolePrintf("%s", xorstr("[BLAXED] "));
				I::pCVar->ConsolePrintf("%s %s%s %s%s", xorstr("Shoot"), MenuV2::SanitizeName(pinfo.m_szPlayerName).c_str(), ".", rflag.c_str(), "\n");

				Vector fw;
				Math::AngleVectors(Math::CalcAngle(from, to), fw);
				fw.NormalizeInPlace();
				hitMarkers->Add(from, to, fw, 1);
			}
		}
	}

	void RecvLowerBodyYawTarget(const CRecvProxyData *pData, CBaseEntity *entity, void *pOut)
	{
		if (entity)
		{
			float value = pData->m_Value.m_Float;

			if (entity == G::pLocal)
			{
				static float last = 0;

				//if (I::pGlobals->curtime - last >= 0.22f)
				{
					/*antiAim->lowerbodyChangedCount++;
					antiAim->lastLowerBodyReceived = value;
					antiAim->lastLowerBodyReceivedTime = I::pGlobals->curtime;*/
				}

				last = I::pGlobals->curtime;
			}
			else
			{
				if (cfg.Rage.Resolver.enabled)
				{
					if (!Filter(entity))
					{
						CResolver& resolver = GetResolver(entity->GetIndex());

						if (resolver.IsValid())
						{
							resolver.oLby = Math::NormalizeYawInPlace(value);
						}
					}
				}
			}

			*(float*)pOut = value;
		}

		oRecvLowerBodyYawTarget(pData, entity, pOut);
	}

	void RecvEyeAnglesX(const CRecvProxyData *pData, CBaseEntity *entity, void *pOut)
	{
		if (cfg.Rage.Resolver.enabled)
		{
			if (entity)
			{
				CResolver& resolver = GetResolver(entity->GetIndex());

				if (resolver.IsValid())
				{
					float value = pData->m_Value.m_Float;

					resolver.oEyeAngles.x = value;

					*(float*)pOut = value;
				}
			}
		}

		oRecvEyeAnglesX(pData, entity, pOut);
	}

	void RecvEyeAnglesY(const CRecvProxyData *pData, CBaseEntity *entity, void *pOut)
	{
		if (cfg.Rage.Resolver.enabled)
		{
			if (entity && !Filter(entity))
			{
				CResolver& resolver = GetResolver(entity->GetIndex());

				if (resolver.IsValid())
				{
					float value = pData->m_Value.m_Float;

					resolver.oEyeAngles.y = value;

					*(float*)pOut = value;
				}
			}
		}

		oRecvEyeAnglesY(pData, entity, pOut);
	}

	void RecvEyeAnglesZ(const CRecvProxyData *pData, CBaseEntity *entity, void *pOut)
	{
		if (cfg.Rage.Resolver.enabled)
		{
			if (entity && !Filter(entity))
			{
				CResolver& resolver = GetResolver(entity->GetIndex());

				if (resolver.IsValid())
				{
					float value = pData->m_Value.m_Float;

					resolver.oEyeAngles.z = value;

					*(float*)pOut = value;
				}
			}
		}

		oRecvEyeAnglesZ(pData, entity, pOut);
	}

};
