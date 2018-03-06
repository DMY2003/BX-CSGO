#include "AimbotRecode.h"
#include "Global.h"
#include "LagCompensation.h"
#include "Util.h"
#include "AimbotV2.h"
#include "IClientRenderable.h"
#include "Render.h"
#include "ResolverV2.h"
#define PI 3.14159265358979323846f
#define PI_F	((float)(PI)) 
// Redundant because we now have access to g_InputSystem, which sorts everything from wndproc.
bool pressedKey[256] = {};
int realHitboxSpot[] = { 0, 1, 2, 3 };
trace_t AimRage::trace;
trace_t AimRage::trace2;
///////////////////////////////////////////////////////////////////////////////////////////////
bool CBaseCombatWeapon::CanFire()
{
	float nextPrimAttack = G::pWeapon->GetNextPrimaryAttack();
	float serverTime = TICKS_TO_TIME(G::pLocal->GetTickBase());// + I::pGlobals->frametime;absoluteframetime

	if (G::pWeapon->GetClip1() == 0 || G::pWeapon->InReload())
		return false;

	if (cfg.Rage.AutoFire.autoRevolver)
	{
		if (!aimbotV2->IsRevolverAbleToShoot())
			return false;
	}

	if (/*nextPrimAttack <= 0 || */nextPrimAttack < serverTime)
		return true;

}
bool IsWeaponNonAim()
{
	int idx = *((CBaseAttributableItem*)G::pWeapon)->GetItemDefinitionIndex();

	return (idx == WEAPON_C4 || idx == WEAPON_KNIFE || idx == WEAPON_KNIFE_BAYONET || idx == WEAPON_KNIFE_BUTTERFLY || idx == WEAPON_KNIFE_FALCHION
		|| idx == WEAPON_KNIFE_FLIP || idx == WEAPON_KNIFE_GUT || idx == WEAPON_KNIFE_KARAMBIT || idx == WEAPON_KNIFE_M9_BAYONET || idx == WEAPON_KNIFE_PUSH
		|| idx == WEAPON_KNIFE_SURVIVAL_BOWIE || idx == WEAPON_KNIFE_T || idx == WEAPON_KNIFE_TACTICAL || idx == WEAPON_FLASHBANG || idx == WEAPON_HEGRENADE
		|| idx == WEAPON_SMOKEGRENADE || idx == WEAPON_MOLOTOV || idx == WEAPON_DECOY || idx == WEAPON_INCGRENADE);
}

void AimRage::Work(CUserCmd *usercmd, bool& chokePacket)
{

	aimbotV2->usingTick = false;

	if (!cfg.Aimbot.enabled)
		return;

	if (!usercmd || !G::pLocal || !G::pWeapon)
		return;

	this->chokePacket = chokePacket;
	this->usercmd = usercmd;

	Global::bAimbotting = false;
	Global::bVisualAimbotting = false;

	if (IsWeaponNonAim() || G::pWeapon->GetClip1() < 1)
		return;

	TargetEntities();
}
std::string AimRage::GetUsingWeaponNames()
{
	CBaseCombatWeapon *pWeapon = G::pWeapon;
	if (!G::pLocal || !pWeapon || !pWeapon->GetModel())
		return "";

	char name[64];
	pWeapon->GetModel()->GetName(name);
	name[63] = '\0';

	return std::string(name);
}

void AimRage::DoAutoScopeNew()
{
	if (!cfg.Rage.AutoFire.autoScope)
		return;

	if (!G::pLocal->IsScoped())
	{
		std::string name = GetUsingWeaponNames();

		if (name.find(xorstr("awp")) != -1 ||
			name.find(xorstr("g3sg1")) != -1 ||
			name.find(xorstr("scar20")) != -1 ||
			name.find(xorstr("ssg08")) != -1 ||
			name.find(xorstr("scout")) != -1)
		{
			usercmd->buttons |= IN_ATTACK2;
		}
	}
}
void AimRage::MultiPointDraw() {
	//TODO MOVE ME ONLY FOR TESTING!
	if (cfg.Visual.debugMultipoint) {
		int outdated_cnt = 0;
		float fl_cur_time = I::pGlobals->curtime;
		for (auto& scan : Global::hitscan_points)
		{
			auto flt_time = std::get<1>(scan);
			auto clr_color = std::get<2>(scan);

			float time_delta = fl_cur_time - flt_time;
			if (time_delta > 0.255f) {
				outdated_cnt++;
				continue;
			}

			clr_color.SetAlpha(0.255f - time_delta);

			Vector screen_pos;
			if (render->WorldToScreen(std::get<0>(scan), screen_pos))
			{
				I::pSurface->DrawSetColor(std::get<2>(scan));
				I::pSurface->DrawLine(screen_pos.x, screen_pos.y, screen_pos.x + 1, screen_pos.y + 1);
			}
		}
		for (int i = 0; i < outdated_cnt; ++i)
			Global::hitscan_points.pop_front();
	}
}
bool AimRage::TargetSpecificEnt(CBaseEntity* pEnt)
{
	int i = pEnt->GetIndex();

	aimbotV2->targetInfo.entity = pEnt;

	auto CalcAngle = [](Vector vec, CUserCmd* cmd)
	{
		QAngle angle = cmd->viewangles;

		angle = Math::CalcAngle(G::pLocal->GetEyePosition(), vec);
		angle -= G::pLocal->GetAimPunchAngles() * 2.f;

		Math::NormalizeAngles(angle);
		Math::ClampAngles(angle);

		return angle;
	};
	int iHitbox = realHitboxSpot[cfg.Aimbot.position];
	Vector vecTarget;
	bool LagComp_Hitchanced = false;
	if (cfg.Rage.Resolver.backTrack)
	{
		if (CMBacktracking::Get().StartLagCompensation(pEnt))
		{
			LagRecord cur_record;
			auto& m_LagRecords = CMBacktracking::Get().m_LagRecord[pEnt->GetIndex()];

			while (CMBacktracking::Get().FindViableRecord(pEnt, &cur_record))
			{
				auto iter = std::find(m_LagRecords.begin(), m_LagRecords.end(), cur_record);
				if (iter == m_LagRecords.end())
					continue;

				if (!iter->m_bMatrixBuilt)
				{
					if (!pEnt->SetupBones(iter->matrix, 128, 256, I::pEngineClient->GetLastTimeStamp()))
						continue;

					iter->m_bMatrixBuilt = true;
				}

				if (cfg.Rage.AutoFire.AutoBaim && Global::iShotsFired > cfg.Rage.AutoFire.BaimAfterShots)
					vecTarget = CalculateBestPoint(pEnt, HITBOX_PELVIS, cfg.Rage.AutoWall.minDamage, cfg.Rage.AutoFire.prioritize, iter->matrix);
				else
					vecTarget = CalculateBestPoint(pEnt, iHitbox, cfg.Rage.AutoWall.minDamage, cfg.Rage.AutoFire.prioritize, iter->matrix);

				if (!vecTarget.IsValid())
					continue;

				LagComp_Hitchanced = HitChance(CalcAngle(vecTarget, usercmd), pEnt, cfg.Rage.AutoFire.hitchance);
				break;	
			}
			CMBacktracking::Get().FinishLagCompensation(pEnt);
		}
	}
	else
	{
		matrix3x4_t matrix[128];
		if (!pEnt->SetupBones(matrix, 128, 256, I::pEngineClient->GetLastTimeStamp()))
			return false;

		if (cfg.Rage.AutoFire.AutoBaim && Global::iShotsFired > cfg.Rage.AutoFire.BaimAfterShots)
			vecTarget = CalculateBestPoint(pEnt, HITBOX_PELVIS, cfg.Rage.AutoWall.minDamage, cfg.Rage.AutoFire.prioritize, matrix);
		else
			vecTarget = CalculateBestPoint(pEnt, iHitbox, cfg.Rage.AutoWall.minDamage, cfg.Rage.AutoFire.prioritize, matrix);
	}

	if (!vecTarget.IsValid())
	{
		return false;
	}

	AutoStop();
	AutoCrouch();

	this->usercmd->viewangles = Global::vecVisualAimbotAngs = CalcAngle(vecTarget, usercmd);
	//Global::vecVisualAimbotAngs += (cfg.Rage.noRecoil ? G::pLocal->GetAimPunchAngles() * 2.f : QAngle(0, 0, 0));
	Global::bVisualAimbotting = true;

	if (this->can_fire_weapon)
	{
		/*
		We've overlooked some things, in particular that we wont try to shoot at ppl that break lagcomp because
		we return false, dont find a good aimspot and dont try to aim at the fakelag fixed entity. IDK if we have
		to mess with the tickcount when they break lc either.
		*/
		if (cfg.Rage.Resolver.backTrack)
			CMBacktracking::Get().ProcessCMD(i, usercmd);

		// Save more fps by remembering to try the same entity again next time.
		prev_aimtarget = pEnt->GetIndex();

		this->DoAutoScopeNew();

		if ((cfg.Rage.Resolver.backTrack && LagComp_Hitchanced) || (!LagComp_Hitchanced && HitChance(usercmd->viewangles, pEnt, cfg.Rage.AutoFire.hitchance)))
		{
			Global::bAimbotting = true;
			if (cfg.Rage.Resolver.enabled)
			{
				ResolverV2::CResolver& resolver = ResolverV2::GetResolver(pEnt->GetIndex());

				if (resolver.IsValid())
				{
					ResolverV2::OnShoot(G::pLocal->GetEyePosition(), vecTarget, pEnt);
				}
				else
				{
					usercmd->buttons &= ~IN_ATTACK;
				}
			}
			if (cfg.Rage.AutoFire.enabled)
			{
				usercmd->buttons |= IN_ATTACK;

				ResolverV2::CResolver& resolver = ResolverV2::GetResolver(pEnt->GetIndex());

				if (resolver.IsValid())
				{
					ResolverV2::lastShotResolverFlags = resolver.flags;
					ResolverV2::lastShotResolverBruteforce = resolver.bruteforce;
					ResolverV2::OnShoot(G::pLocal->GetEyePosition(), vecTarget, pEnt);
				}
			}
			if ((usercmd->buttons & IN_ATTACK))
			{
				aimbotV2->usingTick = true;
			}
		}
	}

	return true;
}
bool AimRage::IsPistol()
{
	int id = *((CBaseAttributableItem*)G::pWeapon)->GetItemDefinitionIndex();

	return id == WEAPON_DEAGLE || id == WEAPON_ELITE || id == WEAPON_FIVESEVEN || id == WEAPON_P250 || id == WEAPON_GLOCK || id == WEAPON_USP_SILENCER || id == WEAPON_CZ75A || id == WEAPON_TEC9 || id == WEAPON_REVOLVER;
}
bool AimRage::HitChance(QAngle angles, CBaseEntity *ent, float chance)
{
	auto weapon = G::pWeapon;

	if (!weapon)
		return false;

	Vector forward, right, up;
	Vector src = G::pLocal->GetEyePosition();
	Math::AngleVectorr(angles, forward, right, up);

	int cHits = 0;
	int cNeededHits = static_cast<int>(150.f * (chance / 100.f));

	weapon->UpdateAccuracyPenalty();
	float weap_spread = weapon->GetSpread();
	float weap_inaccuracy = weapon->GetInaccuracy();

	for (int i = 0; i < 150; i++)
	{
		float a = Math::RandomFloat(0.f, 1.f);
		float b = Math::RandomFloat(0.f, 2.f * PI_F);
		float c = Math::RandomFloat(0.f, 1.f);
		float d = Math::RandomFloat(0.f, 2.f * PI_F);

		float inaccuracy = a * weap_inaccuracy;
		float spread = c * weap_spread;

		if (IsRevolver())
		{
			a = 1.f - a * a;
			a = 1.f - c * c;
		}

		Vector spreadView((cos(b) * inaccuracy) + (cos(d) * spread), (sin(b) * inaccuracy) + (sin(d) * spread), 0), direction;

		direction.x = forward.x + (spreadView.x * right.x) + (spreadView.y * up.x);
		direction.y = forward.y + (spreadView.x * right.y) + (spreadView.y * up.y);
		direction.z = forward.z + (spreadView.x * right.z) + (spreadView.y * up.z);
		direction.Normalized();

		QAngle viewAnglesSpread;
		Math::VectorAngles(direction, up, viewAnglesSpread);
		Math::NormalizeAngles(viewAnglesSpread);

		Vector viewForward;
		Math::AngleVectors(viewAnglesSpread, viewForward);
		viewForward.NormalizeInPlace();

		viewForward = src + (viewForward * weapon->GetCSWpnData()->m_flRange);

		trace_t tr;
		Ray_t ray;

		ray.Init(src, viewForward);
		I::pEngineTrace->ClipRayToEntity(ray, MASK_SHOT | CONTENTS_GRATE, (int*)ent, &tr);

		if (tr.m_pEnt == ent)
			++cHits;

		if (static_cast<int>((static_cast<float>(cHits) / 150.f) * 100.f) >= chance)
			return true;

		if ((150 - i + cHits) < cNeededHits)
			return false;
	}
	return false;
}

void AimRage::AutoCrouch()
{
	if (!cfg.Rage.AutoFire.autoStop)
		return;

	usercmd->buttons |= IN_DUCK;
}

void AimRage::AutoStop()
{
	if (!cfg.Rage.AutoFire.autoStop)
		return;

	usercmd->forwardmove = 0;
	usercmd->sidemove = 0;
}
bool AimRage::IsRevolver()
{
	return GetUsingWeaponNames().find(xorstr("revolver")) != -1;
}
void AimRage::TargetEntities()
{
	auto weapon = local_weapon = G::pWeapon;

	static CBaseCombatWeapon *oldWeapon;

	if (weapon != oldWeapon)
	{
		oldWeapon = weapon;
		usercmd->buttons &= ~IN_ATTACK;
		return;
	}

	if (IsPistol() && usercmd->tick_count % 2)
	{
		static int lastshot;
		if (usercmd->buttons & IN_ATTACK)
			lastshot++;

		if (!usercmd->buttons & IN_ATTACK || lastshot > 1)
		{
			usercmd->buttons &= ~IN_ATTACK;
			lastshot = 0;
		}
		return;
	}

	/*
	We should also add those health/fov based memes and only check newest record. Good enough IMO
	*/

	this->can_fire_weapon = local_weapon->CanFire();
	if (this->can_fire_weapon)
		Global::hitscan_points.clear();
	if (prev_aimtarget && CheckTarget(prev_aimtarget))
	{
		if (TargetSpecificEnt(I::pClientEntityList->GetClientEntity(prev_aimtarget)))
		{
			return;
		}
	}

	for (int i = 1; i < I::pEngineClient->GetMaxClients(); i++)
	{
		// Failed to shoot at him again, reset him and try others.
		if (prev_aimtarget && prev_aimtarget == i) {
			prev_aimtarget = NULL;
			continue;
		}

		if (!CheckTarget(i))
			continue;

		CBaseEntity *player = I::pClientEntityList->GetClientEntity(i);

		if (TargetSpecificEnt(player))
		{
			return;
		}
	}

	if (weapon)
	{
		if (!(usercmd->buttons & IN_ATTACK) && weapon->CanFire())
			Global::iShotsFired = 0;
		else
			++Global::iShotsFired;
	}
}
float AimRage::BestHitPoint(CBaseEntity *player, int prioritized, float minDmg, mstudiohitboxset_t *hitset, matrix3x4_t matrix[], Vector &vecOut)
{
	mstudiobbox_t *hitbox = hitset->pHitbox(prioritized);
	if (!hitbox)
		return 0.f;

	std::vector<Vector> vecArray;
	float flHigherDamage = 0.f;

	float mod = hitbox->radius != -1.f ? hitbox->radius : 0.f;

	Vector max;
	Vector min;

	Math::VectorTransform(hitbox->bbmax + mod, matrix[hitbox->bone], max);
	Math::VectorTransform(hitbox->bbmin - mod, matrix[hitbox->bone], min);

	auto center = (min + max) * 0.5f;

	QAngle curAngles = Math::CalcAngle(center, G::pLocal->GetEyePosition());

	Vector forward;
	Math::AngleVectors(curAngles, forward);

	Vector right = forward.Cross(Vector(0, 0, 1));
	Vector left = Vector(-right.x, -right.y, right.z);

	Vector top = Vector(0, 0, 1);
	Vector bot = Vector(0, 0, -1);

	const float POINT_SCALE = cfg.Rage.AutoFire.pointScale;
	if (cfg.Rage.AutoFire.multiPoint) {
		switch (prioritized)
		{
		case HITBOX_HEAD:
			for (auto i = 0; i < 4; ++i)
			{
				vecArray.emplace_back(center);
			}
			vecArray[1] += top * (hitbox->radius * POINT_SCALE);
			vecArray[2] += right * (hitbox->radius * POINT_SCALE);
			vecArray[3] += left * (hitbox->radius * POINT_SCALE);
			break;

		default:

			for (auto i = 0; i < 2; ++i)
			{
				vecArray.emplace_back(center);
			}
			vecArray[0] += right * (hitbox->radius * POINT_SCALE);
			vecArray[1] += left * (hitbox->radius * POINT_SCALE);
			break;
		}
	}
	else
		vecArray.emplace_back(center);

	for (Vector cur : vecArray)
	{
		int iHitbox = realHitboxSpot[cfg.Aimbot.position];
		float flCurDamage = 0;
		flCurDamage = GetDamageVec(cur);
		if (cfg.Visual.debugMultipoint) 
		{
			Color col = (flCurDamage && (flCurDamage > flHigherDamage) && (flCurDamage > minDmg) ? Color(0, 200, 55, 200) : Color(200, 0, 55, 200));
			Global::hitscan_points.emplace_back(std::make_tuple(cur, I::pGlobals->curtime, col));
		}

		if (!flCurDamage)
			continue;

		if ((flCurDamage > flHigherDamage) && (flCurDamage > minDmg))
		{
			flHigherDamage = flCurDamage;
			vecOut = cur;
		}
	}
	return flHigherDamage;
}
Vector AimRage::CalculateBestPoint(CBaseEntity *player, int prioritized, float minDmg, bool onlyPrioritized, matrix3x4_t matrix[])
{
	studiohdr_t *studioHdr = I::pModelInfo->GetStudioModel((model_t*)player->GetModel());
	mstudiohitboxset_t *set = studioHdr->pHitboxSet(player->GetHitboxSet());
	Vector vecOutput(NAN, NAN, NAN);

	if (BestHitPoint(player, prioritized, minDmg, set, matrix, vecOutput) > minDmg && onlyPrioritized)
	{
		return vecOutput;
	}
	else
	{
		float flHigherDamage = 0.f;
		Vector vecCurVec;
		// why not use all the hitboxes then
		//static Hitboxes hitboxesLoop;
		static int hitboxesLoop[] =
		{
			HITBOX_HEAD,
			HITBOX_PELVIS,
			HITBOX_UPPER_CHEST,
			HITBOX_CHEST,
			HITBOX_LOWER_NECK,
			HITBOX_LEFT_FOREARM,
			HITBOX_RIGHT_FOREARM,
			HITBOX_RIGHT_HAND,
			HITBOX_LEFT_THIGH,
			HITBOX_RIGHT_THIGH,
			HITBOX_LEFT_CALF,
			HITBOX_RIGHT_CALF,
			HITBOX_LEFT_FOOT,
			HITBOX_RIGHT_FOOT
		};

		int loopSize = ARRAYSIZE(hitboxesLoop);

		if ((player->GetVelocity().Length() > 0.1f) && !player->GetDucking())
			loopSize = 8;

		for (int i = 0; i < loopSize; ++i)
		{
			if (!cfg.Rage.AutoFire.rage_multiHitboxes[i])
				continue;

			float flCurDamage = BestHitPoint(player, hitboxesLoop[i], minDmg, set, matrix, vecCurVec);

			if (!flCurDamage)
				continue;

			if (flCurDamage > flHigherDamage)
			{
				flHigherDamage = flCurDamage;
				vecOutput = vecCurVec;
			}
		}
		return vecOutput;
	}
}

bool AimRage::CheckTarget(int i)
{
	CBaseEntity *player = I::pClientEntityList->GetClientEntity(i);

	if (!player || player == nullptr)
		return false;

	if (player == G::pLocal)
		return false;

	if (player->GetTeam() == G::pLocal->GetTeam())
		return false;

	if (player->IsDormant())
		return false;

	if (player->GetGunGameImmunity())
		return false;

	if (!player->IsAlive())
		return false;

	return true;
}

float AimRage::GetDamageVec(const Vector &vecPoint)
{
	if (!Global::pLocal)
		return false;

	if (!Global::pLocal->IsAlive())
		return false;

	Vector rem = vecPoint;
	FireBulletData data;
	data.src = G::pLocal->GetEyePosition();
	data.filter.pSkip = G::pLocal;
	QAngle angle = Math::CalcAngle(data.src, rem);
	Math::AngleVectors(angle, data.direction);
	data.direction.Normalized();
	CBaseCombatWeapon* active_weapon = G::pLocal->GetWeapon();

	if (!active_weapon)
		return -1.0f;

	if (!SimulateFireBullet(G::pLocal, active_weapon, data))
		return false;

	return data.current_damage;
}
void UTIL_TraceLine(Vector& vecAbsStart, Vector& vecAbsEnd, unsigned int mask, const CBaseEntity *ignore, int collisionGroup, trace_t *ptr)
{

	Ray_t ray;
	ray.Init(vecAbsStart, vecAbsEnd);
	CTraceFilter traceFilter;
	traceFilter.pSkip = (void*)ignore;
	I::pEngineTrace->TraceRay(ray, mask, &traceFilter, ptr);
}
float DistanceToRays(const Vector &pos, const Vector &rayStart, const Vector &rayEnd, float *along = NULL, Vector *pointOnRay = NULL)
{
	Vector to = pos - rayStart;
	Vector dir = rayEnd - rayStart;
	float length = dir.NormalizeInPlace();

	float rangeAlong = Math::DotProduct(dir, to);
	if (along)
	{
		*along = rangeAlong;
	}

	float range;

	if (rangeAlong < 0.0f)
	{
		// off start point
		range = -(pos - rayStart).Length();

		if (pointOnRay)
		{
			*pointOnRay = rayStart;
		}
	}
	else if (rangeAlong > length)
	{
		// off end point
		range = -(pos - rayEnd).Length();

		if (pointOnRay)
		{
			*pointOnRay = rayEnd;
		}
	}
	else // within ray bounds
	{
		Vector onRay = rayStart + (dir.operator*(rangeAlong));
		range = (pos - onRay).Length();

		if (pointOnRay)
		{
			*pointOnRay = onRay;
		}
	}

	return range;
}

void UTIL_ClipTraceToPlayers(Vector& vecAbsStart, Vector& vecAbsEnd, unsigned int mask, ITraceFilter* filter, CGameTrace* tr)
{
	CGameTrace playerTrace;
	Ray_t ray;
	float smallestFraction = tr->fraction;
	const float maxRange = 60.0f;

	ray.Init(vecAbsStart, vecAbsEnd);

	for (int k = 1; k <= I::pGlobals->maxClients; ++k)
	{
		CBaseEntity *player = I::pClientEntityList->GetClientEntity(k);

		if (!player || !player->IsDormant())
			continue;

		if (filter && filter->ShouldHitEntity(player, mask) == false)
			continue;

		float range = DistanceToRays(player->GetWorldSpaceCenter(), vecAbsStart, vecAbsEnd);
		if (range < 0.0f || range > maxRange)
			continue;

		I::pEngineTrace->ClipRayToEntity(ray, mask | CONTENTS_HITBOX, (int*)player, &playerTrace);
		if (playerTrace.fraction < smallestFraction)
		{
			// we shortened the ray - save off the trace
			*tr = playerTrace;
			smallestFraction = playerTrace.fraction;
		}
	}

}
bool AimRage::SimulateFireBullet(CBaseEntity* pLocal, CBaseCombatWeapon *weap, FireBulletData &data)
{
	data.penetrate_count = 4;
	data.trace_length = 0.0f;
	WeaponInfo_t* weaponData = weap->GetCSWpnData(); // Get Weapon Info
	data.current_damage = (float)weaponData->m_iDamage;

	while ((data.penetrate_count > 0) && (data.current_damage >= 1.0f))
	{
		data.trace_length_remaining = weaponData->m_flRange - data.trace_length;

		Vector end = data.src + data.direction * data.trace_length_remaining;

		traceIt(data.src, end, MASK_SHOT | CONTENTS_GRATE, G::pLocal, &data.enter_trace);
		ClipTraceToPlayers(data.src, end + data.direction * 40.f, MASK_SHOT | CONTENTS_GRATE, &data.filter, &data.enter_trace);

		if (data.enter_trace.fraction == 1.0f)
			break;

		if ((data.enter_trace.hitgroup <= 7) && (data.enter_trace.hitgroup > 0))
		{
			data.trace_length += data.enter_trace.fraction * data.trace_length_remaining;
			data.current_damage *= pow(weaponData->m_flRangeModifier, data.trace_length * 0.002);

			CBaseEntity *player = reinterpret_cast<CBaseEntity*>(data.enter_trace.m_pEnt);
			if (player->GetTeam() == G::pLocal->GetTeam())
				return false;

			ScaleDamage(data.enter_trace.hitgroup, player, weaponData->m_flArmorRatio, data.current_damage);

			return true;
		}

		if (!HandleBulletPenetration(weaponData, data))
			break;
	}

	return false;
}

bool AimRage::HandleBulletPenetration(WeaponInfo_t *wpn_data, FireBulletData &data)
{
	surfacedata_t *enter_surface_data = I::pPhysicsSurfaceProps->GetSurfaceData(data.enter_trace.surface.surfaceProps);
	int enter_material = enter_surface_data->game.material;
	float enter_surf_penetration_mod = enter_surface_data->game.flPenetrationModifier;

	data.trace_length += data.enter_trace.fraction * data.trace_length_remaining;
	data.current_damage *= pow((wpn_data->m_flRangeModifier), (data.trace_length * 0.002));

	if ((data.trace_length > 3000.f) || (enter_surf_penetration_mod < 0.1f))
		data.penetrate_count = 0;

	if (data.penetrate_count <= 0)
		return false;

	Vector dummy;
	trace_t trace_exit;

	if (!TraceToExit(dummy, &data.enter_trace, data.enter_trace.endpos, data.direction, &trace_exit))
		return false;

	surfacedata_t *exit_surface_data = I::pPhysicsSurfaceProps->GetSurfaceData(trace_exit.surface.surfaceProps);
	int exit_material = exit_surface_data->game.material;

	float exit_surf_penetration_mod = enter_surface_data->game.flPenetrationModifier;
	float final_damage_modifier = 0.16f;
	float combined_penetration_modifier = 0.0f;

	if (((data.enter_trace.contents & CONTENTS_GRATE) != 0) || (enter_material == 89) || (enter_material == 71))
	{
		combined_penetration_modifier = 3.0f;
		final_damage_modifier = 0.05f;
	}
	else
	{
		combined_penetration_modifier = (enter_surf_penetration_mod + exit_surf_penetration_mod) * 0.5f;
	}

	if (enter_material == exit_material)
	{
		if (exit_material == 87 || exit_material == 85)
			combined_penetration_modifier = 3.0f;
		else if (exit_material == 76)
			combined_penetration_modifier = 2.0f;
	}

	float v34 = fmaxf(0.f, 1.0f / combined_penetration_modifier);
	float v35 = (data.current_damage * final_damage_modifier) + v34 * 3.0f * fmaxf(0.0f, (3.0f / wpn_data->m_flPenetration) * 1.25f);
	float thickness = (trace_exit.endpos - data.enter_trace.endpos).Length();

	thickness *= thickness;
	thickness *= v34;
	thickness /= 24.0f;

	float lost_damage = fmaxf(0.0f, v35 + thickness);

	if (lost_damage > data.current_damage)
		return false;

	if (lost_damage >= 0.0f)
		data.current_damage -= lost_damage;

	if (data.current_damage < 1.0f)
		return false;

	data.src = trace_exit.endpos;
	data.penetrate_count--;

	return true;
}

bool AimRage::TraceToExit(Vector &end, CGameTrace *enter_trace, Vector start, Vector dir, CGameTrace *exit_trace)
{
	auto distance = 0.0f;

	while (distance <= 90.0f)
	{
		distance += 4.0f;
		end = start + dir * distance;

		auto point_contents = I::pEngineTrace->GetPointContents(end, MASK_SHOT_HULL | CONTENTS_HITBOX, nullptr);
		if (point_contents & MASK_SHOT_HULL && (!(point_contents & CONTENTS_HITBOX)))
			continue;

		auto new_end = end - (dir * 4.0f);

		traceIt(end, new_end, MASK_SHOT | CONTENTS_GRATE, nullptr, exit_trace);

		if (exit_trace->m_pEnt == nullptr)
			return false;

		if (exit_trace->startsolid && exit_trace->surface.flags & SURF_HITBOX)
		{
			traceIt(end, start, 0x600400B, reinterpret_cast<CBaseEntity*>(exit_trace->m_pEnt), exit_trace);

			if ((exit_trace->fraction < 1.0f || exit_trace->allsolid) && !exit_trace->startsolid)
			{
				end = exit_trace->endpos;
				return true;
			}
			continue;
		}

		if (!(exit_trace->fraction < 1.0 || exit_trace->allsolid || exit_trace->startsolid) || exit_trace->startsolid)
		{
			if (exit_trace->m_pEnt)
			{
				if ((enter_trace->m_pEnt != nullptr && enter_trace->m_pEnt != I::pClientEntityList->GetClientEntity(0)) && IsBreakableEntity(reinterpret_cast<CBaseEntity*>(enter_trace->m_pEnt)))
					return true;
			}
			continue;
		}

		if (((exit_trace->surface.flags >> 7) & 1) && !((enter_trace->surface.flags >> 7) & 1))
			continue;

		if (exit_trace->plane.normal.Dot(dir) <= 1.0f)
		{
			float fraction = exit_trace->fraction * 4.0f;
			end = end - (dir * fraction);

			return true;
		}
	}
	return false;
}

bool AimRage::TraceToExit(Vector &start, Vector &dir)
{
	auto distance = 0.f;

	while (distance <= 90.0f)
	{
		distance += 4.0f;

		Vector vStart = (dir * distance) + start;
		Vector vEnd = vStart - (dir * 4.0f);

		auto point_contents = I::pEngineTrace->GetPointContents(vStart, MASK_SHOT_HULL | CONTENTS_HITBOX, nullptr);
		if (point_contents & MASK_SHOT_HULL && (!(point_contents & CONTENTS_HITBOX)))
			continue;

		traceIt(vStart, vEnd, MASK_SHOT | CONTENTS_GRATE, nullptr, &trace2);

		if (trace2.m_pEnt == nullptr)
			return false;

		if (trace2.startsolid && trace2.surface.flags & SURF_HITBOX)
		{
			traceIt(vStart, start, 0x600400B, reinterpret_cast<CBaseEntity*>(trace2.m_pEnt), &trace2);

			if ((trace2.fraction < 1.0f || trace2.allsolid || trace2.startsolid) && !trace2.startsolid)
			{
				vStart = trace2.endpos;
				return true;
			}
			continue;
		}

		if (!(trace2.fraction < 1.0 || trace2.allsolid || trace2.startsolid) || trace2.startsolid)
		{
			if (trace2.m_pEnt)
			{
				if ((trace.m_pEnt != nullptr && trace.m_pEnt != I::pClientEntityList->GetClientEntity(0)) && IsBreakableEntity(reinterpret_cast<CBaseEntity*>(trace.m_pEnt)))
					return true;
			}
			continue;
		}

		if (((trace2.surface.flags >> 7) & 1) && !((trace.surface.flags >> 7) & 1))
			continue;

		if (trace2.plane.normal.Dot(dir) <= 1.0f)
		{
			float fraction = trace2.fraction * 4.0f;
			vStart = vStart - (dir * fraction);

			return true;
		}
	}
	return false;
}

bool AimRage::IsBreakableEntity(CBaseEntity *ent)
{
	typedef bool(__thiscall *isBreakbaleEntityFn)(CBaseEntity*);
	static isBreakbaleEntityFn IsBreakableEntityFn;

	if (!IsBreakableEntityFn)
		IsBreakableEntityFn = (isBreakbaleEntityFn)Mem::FindPatternAimbot("\x55\x8B\xEC\x51\x56\x8B\xF1\x85\xF6\x74\x68", "xxxxxxxxxxx", 0, "client.dll");

	if (IsBreakableEntityFn)
	{
		// 0x27C = m_takedamage

		auto backupval = *reinterpret_cast<int*>((uint32_t)ent + 0x27C);
		auto className = ent->GetClientClass()->m_pNetworkName;

		if (ent != I::pClientEntityList->GetClientEntity(0))
		{
			if ((className[1] != 'F' || className[4] != 'c' || className[5] != 'B' || className[9] != 'h') // CFuncBrush
				&& (className[1] != 'B' || className[5] != 'D')) // CBaseDoor because fuck doors
			{
				*reinterpret_cast<int*>((uint32_t)ent + 0x27C) = 2;
			}
		}

		bool retn = IsBreakableEntityFn(ent);

		*reinterpret_cast<int*>((uint32_t)ent + 0x27C) = backupval;

		return retn;
	}
	else
		return false;
}
float GetHitgroupDamageMultipliers(int iHitGroup)
{
	switch (iHitGroup)
	{
	case HITGROUP_HEAD:
		return 4.0f;
	case HITGROUP_STOMACH:
		return 1.25f;
	case HITGROUP_LEFTLEG:
	case HITGROUP_RIGHTLEG:
		return 0.75f;
	}

	return 1.0f;
}

void AimRage::ClipTraceToPlayers(const Vector &vecAbsStart, const Vector &vecAbsEnd, unsigned int mask, ITraceFilter *filter, CGameTrace *tr)
{
	static DWORD ClipTraceToPlayersAdd = Mem::FindPatternAimbot("\x53\x8B\xDC\x83\xEC\x08\x83\xE4\xF0\x83\xC4\x04\x55\x8B\x6B\x04\x89\x6C\x24\x04\x8B\xEC\x81\xEC\x00\x00\x00\x00\x8B\x43\x10", "xxxxxxxxxxxxxxxxxxxxxxxx????xxx", 0, "client.dll");

	if (!ClipTraceToPlayersAdd)
		return;

	_asm
	{
		mov eax, filter
		lea ecx, tr
		push ecx
		push eax
		push mask
		lea edx, vecAbsEnd
		lea ecx, vecAbsStart
		call ClipTraceToPlayersAdd
		add esp, 0xC
	}
}

bool AimRage::DidHitNonWorldEntity(CBaseEntity *ent)
{
	return ent != NULL && ent == I::pClientEntityList->GetClientEntity(0);
}

void AimRage::ScaleDamage(int hitgroup, CBaseEntity *player, float weapon_armor_ratio, float &current_damage)
{
	current_damage *= GetHitgroupDamageMultipliers(hitgroup);
	float m_flArmor = (float)player->GetArmorValue();
	if (m_flArmor > 0.0f && hitgroup < HITGROUP_LEFTLEG)  //don't do gear, or legs
	{
		if (hitgroup == HITGROUP_HEAD && !player->HasHelmet())
			return;

		float armorscaled = (weapon_armor_ratio * 0.5f) * current_damage;
		if ((current_damage - armorscaled) * 0.5f > m_flArmor)
			armorscaled = current_damage - (m_flArmor * 2.0f);
		current_damage = armorscaled;
	}
}

bool AimRage::IsArmored(CBaseEntity *player, int armorVal, int hitgroup)
{
	bool res = false;

	if (armorVal > 0)
	{
		switch (hitgroup)
		{
		case HITGROUP_GENERIC:
		case HITGROUP_CHEST:
		case HITGROUP_STOMACH:
		case HITGROUP_LEFTARM:
		case HITGROUP_RIGHTARM:

			res = true;
			break;

		case HITGROUP_HEAD:

			res = player->HasHelmet();
			break;

		}
	}

	return res;
}

void AimRage::traceIt(Vector &vecAbsStart, Vector &vecAbsEnd, unsigned int mask, CBaseEntity *ign, CGameTrace *tr)
{
	Ray_t ray;

	CTraceFilter filter;
	filter.pSkip = ign;

	ray.Init(vecAbsStart, vecAbsEnd);

	I::pEngineTrace->TraceRay(ray, mask, &filter, tr);
}