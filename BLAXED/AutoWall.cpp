#include "AutoWall.h"
#include "TraceRay.h"

FireBulletData AutoWall::currentData;

float AutoWall::GetHitgroupDamageMultiplier(int iHitGroup)
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

void AutoWall::ScaleDamage(int hitgroup, CBaseEntity *enemy, float weapon_armor_ratio, float &current_damage)
{
	current_damage *= GetHitgroupDamageMultiplier(hitgroup);
}
bool CGameTrace::DidHitWorld() const
{
	return m_pEnt == I::pClientEntityList->GetClientEntity(0);
}
bool AutoWall::DidHitNonWorldEntity(CBaseEntity* m_pEnt)
{
	return m_pEnt != NULL && m_pEnt == I::pClientEntityList->GetClientEntity(0);
}

bool AutoWall::TraceToExit(Vector &end, trace_t& enter_trace, Vector start, Vector dir, trace_t *exit_trace)
{
	float distance = 0.0f;

	while (distance <= 90.f)
	{
		distance += 4.0f;
		end = start + dir * distance;

		auto point_contents = I::pEngineTrace->GetPointContents(end, MASK_SHOT_HULL | CONTENTS_HITBOX, NULL);

		if (point_contents & MASK_SHOT_HULL && (!(point_contents & CONTENTS_HITBOX)))
			continue;

		auto new_end = end - (dir * 4.0f);

		Ray_t ray;
		ray.Init(end, new_end);
		I::pEngineTrace->TraceRay(ray, MASK_SHOT_HULL | CONTENTS_HITBOX, 0, exit_trace);

		if (exit_trace->startsolid && exit_trace->surface.flags & SURF_HITBOX)
		{
			ray.Init(end, start);
			CTraceFilter filter;
			filter.pSkip = exit_trace->m_pEnt;
			I::pEngineTrace->TraceRay(ray, MASK_SHOT_HULL, &filter, exit_trace);

			if (exit_trace->DidHit() && !exit_trace->startsolid)
			{
				end = exit_trace->endpos;
				return true;
			}
			continue;
		}

		if (!exit_trace->DidHit() || exit_trace->startsolid)
		{
			if (enter_trace.m_pEnt)
			{
				if (enter_trace.DidHitNonWorldEntity() && enter_trace.m_pEnt->IsBreakableEntity())
				{
					*exit_trace = enter_trace;
					exit_trace->endpos = start + dir;
					return true;
				}
			}
			continue;
		}
		if ((exit_trace->surface.flags >> 7) & SURF_LIGHT)
		{
			if (exit_trace->m_pEnt->IsBreakableEntity() && enter_trace.m_pEnt->IsBreakableEntity())
			{
				end = exit_trace->endpos;
				return true;
			}

			if (!((enter_trace.surface.flags >> 7) & SURF_LIGHT))
				continue;
		}

		if (exit_trace->plane.normal.Dot(dir) <= 1.f)
		{
			end = end - (dir * (exit_trace->fraction * 4.f));
			return true;
		}

		if (((exit_trace->surface.flags >> 7) & 1) && !((enter_trace.surface.flags >> 7) & 1))
			continue;

		if (exit_trace->plane.normal.Dot(dir) <= 1.0f)
		{
			auto fraction = exit_trace->fraction * 4.0f;
			//auto fraction = exit_trace->fraction * 20.0f;

			end = end - (dir * fraction);

			return true;
		}
	}
	
	return false;	
}

void AutoWall::TraceLine(const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask, CBaseEntity* ignore, trace_t* ptr)
{
	Ray_t ray;
	ray.Init(vecAbsStart, vecAbsEnd);
	CTraceFilter filter;
	filter.pSkip = ignore;

	I::pEngineTrace->TraceRay(ray, mask, &filter, ptr);
}

void AutoWall::TraceHull(const Vector& vecAbsStart, const Vector& vecAbsEnd, const Vector& vecMins, const Vector& vecMaxs, unsigned int mask, CBaseEntity* ignore, trace_t* ptr)
{
	Ray_t ray;
	ray.Init(vecAbsStart, vecAbsEnd, vecMins, vecMaxs);

	CTraceFilter filter;
	filter.pSkip = ignore;

	I::pEngineTrace->TraceRay(ray, mask, &filter, ptr);
}

float DistanceToRay(const Vector& pos, const Vector& rayStart, const Vector& rayEnd, float* along = nullptr, Vector* pointOnRay = nullptr)
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

void AutoWall::ClipTraceToPlayers(const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask, ITraceFilter* filter, trace_t* tr)
{
	trace_t playerTrace;
	Ray_t ray;
	float smallestFraction = tr->fraction;
	const float maxRange = 60.0f;

	ray.Init(vecAbsStart, vecAbsEnd);

	for (int k = 1; k <= I::pGlobals->maxClients; ++k)
	{
		CBaseEntity* player = I::pClientEntityList->GetClientEntity(k);

		if (!player || player->GetLifeState() != LIFE_ALIVE || player->GetHealth() <= 0)
			continue;

		if (player->IsDormant())
			continue;

		if (filter && filter->ShouldHitEntity(player, mask) == false)
			continue;

		float range = DistanceToRay(player->GetWorldSpaceCenter(), vecAbsStart, vecAbsEnd);
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

bool AutoWall::HandleBulletPenetration(WeaponInfo_t *wpn_data, FireBulletData &data)
{
	surfacedata_t *enter_surface_data = I::pPhysicsSurfaceProps->GetSurfaceData(data.enter_trace.surface.surfaceProps);
	int enter_material = enter_surface_data->game.material;
	float enter_surf_penetration_mod = enter_surface_data->game.flPenetrationModifier;

	Vector dummy(0,0,0);
	trace_t trace_exit;

	if (!TraceToExit(dummy, data.enter_trace, data.enter_trace.endpos, data.direction, &trace_exit))
		return false;

	surfacedata_t *exit_surface_data = I::pPhysicsSurfaceProps->GetSurfaceData(trace_exit.surface.surfaceProps);
	int exit_material = exit_surface_data->game.material;

	float exit_surf_penetration_mod = exit_surface_data->game.flPenetrationModifier;
	float final_damage_modifier = 0.16f;
	float combined_penetration_modifier = 0.0f;

	if ((data.enter_trace.contents & CONTENTS_GRATE) || (enter_material == 89) || (enter_material == 71))
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
	
	float lost_damage = fmaxf(0.0f, thickness + v35);
	
	float traceLenght = (trace_exit.endpos - data.enter_trace.endpos).Length();
	
	data.trace_length += traceLenght;
	data.trace_length_remaining -= traceLenght;
	data.current_damage *= powf(wpn_data->m_flRangeModifier, data.trace_length * 0.002f);

	if (lost_damage >= 0.0f)
		data.current_damage -= lost_damage;

	if (data.current_damage < 1.0f)
		return false;
	 
	data.src = trace_exit.endpos;
	data.penetrate_count--;

	return true;
}

bool AutoWall::SimulateFireBullet(CBaseCombatWeapon* pWeapon, FireBulletData &data)
{
	CBaseEntity* localplayer = (CBaseEntity*)data.filter.pSkip;
	CBaseEntity* target = data.target;

	WeaponInfo_t *weaponData = &G::weapInfoCache[localplayer->GetIndex()];//pWeapon->GetCSWpnData();

	if (!weaponData)
		return false;
	
	data.penetrate_count = 3;
	data.trace_length = 0.0f;
	data.current_damage = (float)weaponData->m_iDamage;
	
	trace_t traceEnd;

	Vector endPos;
	int endHitgroup;
	Ray_t ray;
	ray.Init(data.src, data.dst);
	CTraceFilter filter;
	filter.pSkip = localplayer;
	
	float range = weaponData->m_flRange;

	data.trace_length_remaining = min(range, (endPos - data.src).Length() + 0.02f);

	while ((data.penetrate_count > 0) && (data.current_damage >= 1.0f) && data.trace_length_remaining > 0)
	{
		Vector end = data.src + data.direction * data.trace_length_remaining;

		TraceLine(data.src, end, MASK_SHOT, localplayer, &data.enter_trace);

		float traceLenght = data.enter_trace.fraction * data.trace_length_remaining;
		data.trace_length += traceLenght;
		data.trace_length_remaining -= traceLenght;
		data.current_damage *= powf(weaponData->m_flRangeModifier, data.trace_length * 0.002f);

		G::dbg_AutoWall[G::dbg_AutoWall_idx++ % 1024] = data.enter_trace.endpos;

		if ((data.enter_trace.hitgroup <= 7) && (data.enter_trace.hitgroup > 0) && (data.enter_trace.m_pEnt == data.target))
		{
			if (cfg.Rage.AutoFire.onlyHS && data.enter_trace.hitbox != HITBOX_HEAD)
				return false;
			
			ScaleDamage(data.enter_trace.hitgroup, data.enter_trace.m_pEnt, weaponData->m_flArmorRatio, data.current_damage);

			return true;
		}
			
		if (!HandleBulletPenetration(weaponData, data))
			return false;
	}

	return false;
}

float AutoWall::GetDamage(const Vector& aimPoint, CBaseEntity *target, int hitbox)
{
	float damage = 0.0f;
	Vector dst = aimPoint;
	FireBulletData data;
	CBaseEntity* localplayer = G::pLocal;
	data.src = localplayer->GetEyePosition();
	data.filter.pSkip = localplayer;

	data.target = target;
	data.dst = aimPoint;

	data.hitbox = hitbox;

	QAngle angles = Math::CalcAngle(data.src, dst);
	Math::AngleVectors(angles, data.direction);

	data.direction.NormalizeInPlace();

	currentData = data;

	CBaseCombatWeapon* active_weapon = localplayer->GetWeapon();
	if (!active_weapon)
		return -1.0f;

	if (SimulateFireBullet(active_weapon, data))
		damage = data.current_damage;

	return damage;
} 
float AutoWall::GetDamageVec(const Vector &vecPoint)
{
	float damage = 0.f;

	Vector rem = vecPoint;

	FireBulletData data;

	data.src = G::pLocal->GetEyePosition();
	data.filter.pSkip = G::pLocal;

	QAngle angle = Math::CalcAngle(data.src, rem);
	Math::AngleVectors(angle, data.direction);

	data.direction.Normalized();

	auto weap = G::pLocal->GetWeapon();
	if (SimulateFireBullet(weap, data))
		damage = data.current_damage;

	return damage;
}
float AutoWall::GetDamageEx(const Vector& aimPoint, const Vector& origin, CBaseEntity *target, CBaseEntity *filterEnt, int hitbox, FireBulletData *fbd)
{
	float damage = 0.0f;
	Vector dst = aimPoint;
	FireBulletData data;
	CBaseEntity* localplayer = filterEnt;
	data.src = origin;//localplayer->GetEyePosition();
	data.filter.pSkip = localplayer;

	data.target = target;
	data.dst = aimPoint;

	data.hitbox = hitbox;

	QAngle angles = Math::CalcAngle(data.src, dst);
	Math::AngleVectors(angles, data.direction);

	data.direction.NormalizeInPlace();

	currentData = data;

	CBaseCombatWeapon* active_weapon = localplayer->GetWeapon();
	if (!active_weapon)
		return -1.0f;

	if (SimulateFireBullet(active_weapon, data))
		damage = data.current_damage;

	if (fbd)
		*fbd = data;

	return damage;
}
