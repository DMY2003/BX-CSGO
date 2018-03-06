#include "RebuildGameMovement.h"
#include "SDK.h"

void RebuildGameMovement::SetAbsOrigin(CBaseEntity *player, const Vector &vec)
{
	player->SetAbsOrigin(vec);
}

int RebuildGameMovement::ClipVelocity(Vector &in, Vector &normal, Vector &out, float overbounce)
{
	float	backoff;
	float	change;
	float angle;
	int		i, blocked;

	angle = normal[2];

	blocked = 0x00;         // Assume unblocked.
	if (angle > 0)			// If the plane that is blocking us has a positive z component, then assume it's a floor.
		blocked |= 0x01;	// 
	if (!angle)				// If the plane has no Z, it is vertical (wall/step)
		blocked |= 0x02;	// 

							// Determine how far along plane to slide based on incoming direction.
	backoff = in.Dot(normal) * overbounce;

	for (i = 0; i<3; i++)
	{
		change = normal[i] * backoff;
		out[i] = in[i] - change;
	}

	// iterate once to make sure we aren't still moving through the plane
	float adjust = out.Dot(normal);
	if (adjust < 0.0f)
	{
		out -= (normal * adjust);
		//		Msg( "Adjustment = %lf\n", adjust );
	}

	// Return blocking flags.
	return blocked;
}

inline void VectorMA(const Vector &start, float scale, const Vector &direction, Vector &dest)
{
	CHECK_VALID(start);
	CHECK_VALID(direction);

	dest.x = start.x + scale * direction.x;
	dest.y = start.y + scale * direction.y;
	dest.z = start.z + scale * direction.z;
}
inline void VectorMultiply(const Vector &a, float b, Vector &c)
{
	CHECK_VALID(a);
	Assert(IsFinite(b));
	c.x = a.x * b;
	c.y = a.y * b;
	c.z = a.z * b;
}
inline void VectorAdd(const Vector &a, const Vector &b, Vector &c)
{
	CHECK_VALID(a);
	CHECK_VALID(b);
	c.x = a.x + b.x;
	c.y = a.y + b.y;
	c.z = a.z + b.z;
}

inline void VectorSubtract(const Vector &a, const Vector &b, Vector &c)
{
	CHECK_VALID(a);
	CHECK_VALID(b);
	c.x = a.x - b.x;
	c.y = a.y - b.y;
	c.z = a.z - b.z;
}
int RebuildGameMovement::TryPlayerMove(CBaseEntity *player, Vector *pFirstDest, trace_t *pFirstTrace)
{
	Vector  planes[5];
	numbumps[player->GetIndex()] = 4;           // Bump up to four times

	blocked[player->GetIndex()] = 0;           // Assume not blocked
	numplanes[player->GetIndex()] = 0;           //  and not sliding along any planes

	original_velocity[player->GetIndex()] = player->GetVelocity(); // Store original velocity
	primal_velocity[player->GetIndex()] = player->GetVelocity();

	allFraction[player->GetIndex()] = 0;
	time_left[player->GetIndex()] = Interfaces::pGlobals->frametime;   // Total time for this movement operation.

	new_velocity[player->GetIndex()].Zero();

	for (bumpcount[player->GetIndex()] = 0; bumpcount[player->GetIndex()] < numbumps[player->GetIndex()]; bumpcount[player->GetIndex()]++)
	{
		if (player->GetVelocity().Length() == 0.0)
			break;

		// Assume we can move all the way from the current origin to the
		//  end point.
		VectorMA(player->GetAbsOrigin(), time_left[player->GetIndex()], player->GetVelocity(), end[player->GetIndex()]);

		// See if we can make it from origin to end point.
		if (true)
		{
			// If their velocity Z is 0, then we can avoid an extra trace here during WalkMove.
			if (pFirstDest && end[player->GetIndex()] == *pFirstDest)
				pm[player->GetIndex()] = *pFirstTrace;
			else
			{
				TracePlayerBBox(player->GetAbsOrigin(), end[player->GetIndex()], MASK_PLAYERSOLID, 8, pm[player->GetIndex()], player);
			}
		}
		else
		{
			TracePlayerBBox(player->GetAbsOrigin(), end[player->GetIndex()], MASK_PLAYERSOLID, 8, pm[player->GetIndex()], player);
		}

		allFraction[player->GetIndex()] += pm[player->GetIndex()].fraction;

		// If we started in a solid object, or we were in solid space
		//  the whole way, zero out our velocity and return that we
		//  are blocked by floor and wall.
		if (pm[player->GetIndex()].allsolid)
		{
			// CBaseEntity is trapped in another solid
			player->GetVelocity() = vec3_origin[player->GetIndex()];
			return 4;
		}

		// If we moved some portion of the total distance, then
		//  copy the end position into the pmove.origin and 
		//  zero the plane counter.
		if (pm[player->GetIndex()].fraction > 0)
		{
			if (numbumps[player->GetIndex()] > 0 && pm[player->GetIndex()].fraction == 1)
			{
				// There's a precision issue with terrain tracing that can cause a swept box to successfully trace
				// when the end position is stuck in the triangle.  Re-run the test with an uswept box to catch that
				// case until the bug is fixed.
				// If we detect getting stuck, don't allow the movement
				trace_t stuck;
				TracePlayerBBox(pm[player->GetIndex()].endpos, pm[player->GetIndex()].endpos, MASK_PLAYERSOLID, 8, stuck, player);
				if (stuck.startsolid || stuck.fraction != 1.0f)
				{
					//Msg( "Player will become stuck!!!\n" );
					player->GetVelocity() = vec3_origin[player->GetIndex()];
					break;
				}
			}

			// actually covered some distance
			SetAbsOrigin(player, pm[player->GetIndex()].endpos);
			original_velocity[player->GetIndex()] = player->GetVelocity();
			numplanes[player->GetIndex()] = 0;
		}

		// If we covered the entire distance, we are done
		//  and can return.
		if (pm[player->GetIndex()].fraction == 1)
		{
			break;		// moved the entire distance
		}

		// If the plane we hit has a high z component in the normal, then
		//  it's probably a floor
		if (pm[player->GetIndex()].plane.normal[2] > 0.7)
		{
			blocked[player->GetIndex()] |= 1;		// floor
		}
		// If the plane has a zero z component in the normal, then it's a 
		//  step or wall
		if (!pm[player->GetIndex()].plane.normal[2])
		{
			blocked[player->GetIndex()] |= 2;		// step / wall
		}

		// Reduce amount of m_flFrameTime left by total time left * fraction
		//  that we covered.
		time_left[player->GetIndex()] -= time_left[player->GetIndex()] * pm[player->GetIndex()].fraction;

		// Did we run out of planes to clip against?
		if (numplanes[player->GetIndex()] >= 5)
		{
			// this shouldn't really happen
			//  Stop our movement if so.
			player->GetVelocity() = vec3_origin[player->GetIndex()];
			//Con_DPrintf("Too many planes 4\n");

			break;
		}

		// Set up next clipping plane
		planes[numplanes[player->GetIndex()]] = pm[player->GetIndex()].plane.normal;
		numplanes[player->GetIndex()]++;

		// modify original_velocity so it parallels all of the clip planes
		//

		// reflect player velocity 
		// Only give this a try for first impact plane because you can get yourself stuck in an acute corner by jumping in place
		//  and pressing forward and nobody was really using this bounce/reflection feature anyway...
		if (numplanes[player->GetIndex()] == 1 &&
			player->GetFlags() & FL_ONGROUND)
		{
			for (i[player->GetIndex()] = 0; i[player->GetIndex()] < numplanes[player->GetIndex()]; i[player->GetIndex()]++)
			{
				if (planes[i[player->GetIndex()]][2] > 0.7)
				{
					// floor or slope
					ClipVelocity(original_velocity[player->GetIndex()], planes[i[player->GetIndex()]], new_velocity[player->GetIndex()], 1);
					original_velocity[player->GetIndex()] = new_velocity[player->GetIndex()];
				}
				else
				{
					ClipVelocity(original_velocity[player->GetIndex()], planes[i[player->GetIndex()]], new_velocity[player->GetIndex()], 1.0 + Interfaces::pCVar->FindVar("sv_bounce")->GetFloat() * (1 - player->m_surfaceFriction()));
				}
			}

			player->GetVelocity() = new_velocity[player->GetIndex()];
			original_velocity[player->GetIndex()] = new_velocity[player->GetIndex()];
		}
		else
		{
			for (i[player->GetIndex()] = 0; i[player->GetIndex()] < numplanes[player->GetIndex()]; i[player->GetIndex()]++)
			{


				for (j[player->GetIndex()] = 0; j[player->GetIndex()]<numplanes[player->GetIndex()]; j[player->GetIndex()]++)
					if (j[player->GetIndex()] != i[player->GetIndex()])
					{
						// Are we now moving against this plane?
						if (player->GetVelocity().Dot(planes[j[player->GetIndex()]]) < 0)
							break;	// not ok
					}
				if (j[player->GetIndex()] == numplanes[player->GetIndex()])  // Didn't have to clip, so we're ok
					break;
			}

			// Did we go all the way through plane set
			if (i[player->GetIndex()] != numplanes[player->GetIndex()])
			{	// go along this plane
				// pmove.velocity is set in clipping call, no need to set again.
				;
			}
			else
			{	// go along the crease
				if (numplanes[player->GetIndex()] != 2)
				{
					player->GetVelocity() = vec3_origin[player->GetIndex()];
					break;
				}

				dir[player->GetIndex()] = planes[0].Cross(planes[1]);
				dir[player->GetIndex()].NormalizeInPlace();
				d[player->GetIndex()] = dir[player->GetIndex()].Dot(player->GetVelocity());
				VectorMultiply(dir[player->GetIndex()], d[player->GetIndex()], player->GetVelocity());
			}

			//
			// if original velocity is against the original velocity, stop dead
			// to avoid tiny occilations in sloping corners
			//
			d[player->GetIndex()] = player->GetVelocity().Dot(primal_velocity[player->GetIndex()]);
			if (d[player->GetIndex()] <= 0)
			{
				//Con_DPrintf("Back\n");
				player->GetVelocity() = vec3_origin[player->GetIndex()];
				break;
			}
		}
	}

	if (allFraction == 0)
	{
		player->GetVelocity() = vec3_origin[player->GetIndex()];
	}

	// Check if they slammed into a wall
	float fSlamVol = 0.0f;

	float fLateralStoppingAmount = primal_velocity[player->GetIndex()].Length2D() - player->GetVelocity().Length2D();
	if (fLateralStoppingAmount > 580.f * 2.0f)
	{
		fSlamVol = 1.0f;
	}
	else if (fLateralStoppingAmount > 580.f)
	{
		fSlamVol = 0.85f;
	}

	return blocked[player->GetIndex()];
}

void RebuildGameMovement::Accelerate(CBaseEntity *player, Vector &wishdir, float wishspeed, float accel)
{
	// See if we are changing direction a bit
	currentspeed[player->GetIndex()] = player->GetVelocity().Dot(wishdir);

	// Reduce wishspeed by the amount of veer.
	addspeed[player->GetIndex()] = wishspeed - currentspeed[player->GetIndex()];

	// If not going to add any speed, done.
	if (addspeed[player->GetIndex()] <= 0)
		return;

	// Determine amount of accleration.
	accelspeed[player->GetIndex()] = accel * Interfaces::pGlobals->frametime * wishspeed * player->m_surfaceFriction();

	// Cap at addspeed
	if (accelspeed[player->GetIndex()] > addspeed[player->GetIndex()])
		accelspeed[player->GetIndex()] = addspeed[player->GetIndex()];

	// Adjust velocity.
	for (i[player->GetIndex()] = 0; i[player->GetIndex()]<3; i[player->GetIndex()]++)
	{
		player->GetVelocity()[i[player->GetIndex()]] += accelspeed[player->GetIndex()] * wishdir[i[player->GetIndex()]];
	}
}

void RebuildGameMovement::AirAccelerate(CBaseEntity *player, Vector &wishdir, float wishspeed, float accel)
{

	wishspd[player->GetIndex()] = wishspeed;

	// Cap speed
	if (wishspd[player->GetIndex()] > 30.f)
		wishspd[player->GetIndex()] = 30.f;

	// Determine veer amount
	currentspeed[player->GetIndex()] = player->GetVelocity().Dot(wishdir);

	// See how much to add
	addspeed[player->GetIndex()] = wishspd[player->GetIndex()] - currentspeed[player->GetIndex()];

	// If not adding any, done.
	if (addspeed <= 0)
		return;

	// Determine acceleration speed after acceleration
	accelspeed[player->GetIndex()] = accel * wishspeed * Interfaces::pGlobals->frametime * player->m_surfaceFriction();

	// Cap it
	if (accelspeed[player->GetIndex()] > addspeed[player->GetIndex()])
		accelspeed[player->GetIndex()] = addspeed[player->GetIndex()];

	// Adjust pmove vel.
	for (i[player->GetIndex()] = 0; i[player->GetIndex()]<3; i[player->GetIndex()]++)
	{
		player->GetVelocity()[i[player->GetIndex()]] += accelspeed[player->GetIndex()] * wishdir[i[player->GetIndex()]];
		Interfaces::pMoveHelper->SetHost(player);
		Interfaces::pMoveHelper->m_outWishVel[i[player->GetIndex()]] += accelspeed[player->GetIndex()] * wishdir[i[player->GetIndex()]];

	}
}

void RebuildGameMovement::AirMove(CBaseEntity *player)
{
	Math::AngleVectorsNew(player->GetEyeAngles(), forward[player->GetIndex()], right[player->GetIndex()], up[player->GetIndex()]);  // Determine movement angles

																																   // Copy movement amounts
	Interfaces::pMoveHelper->SetHost(player);
	fmove[player->GetIndex()] = Interfaces::pMoveHelper->m_flForwardMove;
	smove[player->GetIndex()] = Interfaces::pMoveHelper->m_flSideMove;

	// Zero out z components of movement vectors
	forward[player->GetIndex()][2] = 0;
	right[player->GetIndex()][2] = 0;
	Math::NormalizeVector(forward[player->GetIndex()]);  // Normalize remainder of vectors
	Math::NormalizeVector(right[player->GetIndex()]);    // 

	for (i[player->GetIndex()] = 0; i[player->GetIndex()]<2; i[player->GetIndex()]++)       // Determine x and y parts of velocity
		wishvel[player->GetIndex()][i[player->GetIndex()]] = forward[player->GetIndex()][i[player->GetIndex()]] * fmove[player->GetIndex()] + right[player->GetIndex()][i[player->GetIndex()]] * smove[player->GetIndex()];

	wishvel[player->GetIndex()][2] = 0;             // Zero out z part of velocity

	wishdir[player->GetIndex()] = wishvel[player->GetIndex()]; // Determine maginitude of speed of move
	wishspeed[player->GetIndex()] = wishdir[player->GetIndex()].NormalizeMovement();

	//
	// clamp to server defined max speed
	//
	if (wishspeed != 0 && (wishspeed[player->GetIndex()] > player->GetMaxSpeed()))
	{
		VectorMultiply(wishvel[player->GetIndex()], player->GetMaxSpeed() / wishspeed[player->GetIndex()], wishvel[player->GetIndex()]);
		wishspeed[player->GetIndex()] = player->GetMaxSpeed();
	}

	AirAccelerate(player, wishdir[player->GetIndex()], wishspeed[player->GetIndex()], Interfaces::pCVar->FindVar("sv_airaccelerate")->GetFloat());

	// Add in any base velocity to the current velocity.
	VectorAdd(player->GetVelocity(), player->GetVelocity(), player->GetVelocity());
	trace_t trace;
	TryPlayerMove(player, &dest[player->GetIndex()], &trace);

	// Now pull the base velocity back out.   Base velocity is set if you are on a moving object, like a conveyor (or maybe another monster?)
	VectorSubtract(player->GetVelocity(), player->GetVelocity(), player->GetVelocity());
}

void RebuildGameMovement::StepMove(CBaseEntity *player, Vector &vecDestination, trace_t &trace)
{
	Vector vecEndPos;
	vecEndPos = vecDestination;

	// Try sliding forward both on ground and up 16 pixels
	//  take the move that goes farthest
	Vector vecPos, vecVel;
	vecPos = player->GetAbsOrigin();
	vecVel = player->GetVelocity();

	// Slide move down.
	TryPlayerMove(player, &vecEndPos, &trace);

	// Down results.
	Vector vecDownPos, vecDownVel;
	vecDownPos = player->GetAbsOrigin();
	vecDownVel = player->GetVelocity();

	// Reset original values.
	SetAbsOrigin(player, vecPos);
	player->GetVelocity() = vecVel;

	// Move up a stair height.
	vecEndPos = player->GetAbsOrigin();

	vecEndPos.z += player->GetStepSize() + 0.03125;


	TracePlayerBBox(player->GetAbsOrigin(), vecEndPos, MASK_PLAYERSOLID, 8, trace, player);
	if (!trace.startsolid && !trace.allsolid)
	{
		SetAbsOrigin(player, trace.endpos);
	}

	TryPlayerMove(player, &dest[player->GetIndex()], &trace);

	// Move down a stair (attempt to).
	vecEndPos = player->GetAbsOrigin();

	vecEndPos.z -= player->GetStepSize() + 0.03125;


	TracePlayerBBox(player->GetAbsOrigin(), vecEndPos, MASK_PLAYERSOLID, 8, trace, player);

	// If we are not on the ground any more then use the original movement attempt.
	if (trace.plane.normal[2] < 0.7)
	{
		SetAbsOrigin(player, vecDownPos);
		player->GetVelocity() = vecDownVel;

		float flStepDist = player->GetAbsOrigin().z - vecPos.z;
		if (flStepDist > 0.0f)
		{
			Interfaces::pMoveHelper->SetHost(player);
			Interfaces::pMoveHelper->m_outStepHeight += flStepDist;
			Interfaces::pMoveHelper->SetHost(nullptr);
		}
		return;
	}

	// If the trace ended up in empty space, copy the end over to the origin.
	if (!trace.startsolid && !trace.allsolid)
	{
		player->SetAbsOrigin(trace.endpos);
	}

	// Copy this origin to up.
	Vector vecUpPos;
	vecUpPos = player->GetAbsOrigin();

	// decide which one went farther
	float flDownDist = (vecDownPos.x - vecPos.x) * (vecDownPos.x - vecPos.x) + (vecDownPos.y - vecPos.y) * (vecDownPos.y - vecPos.y);
	float flUpDist = (vecUpPos.x - vecPos.x) * (vecUpPos.x - vecPos.x) + (vecUpPos.y - vecPos.y) * (vecUpPos.y - vecPos.y);
	if (flDownDist > flUpDist)
	{
		SetAbsOrigin(player, vecDownPos);
		player->GetVelocity() = vecDownVel;
	}
	else
	{
		// copy z value from slide move
		player->GetVelocity() = vecDownVel;
	}

	float flStepDist = player->GetAbsOrigin().z - vecPos.z;
	if (flStepDist > 0)
	{
		Interfaces::pMoveHelper->SetHost(player);
		Interfaces::pMoveHelper->m_outStepHeight += flStepDist;
		Interfaces::pMoveHelper->SetHost(nullptr);
	}
}

void RebuildGameMovement::TracePlayerBBox(const Vector &start, const Vector &end, unsigned int fMask, int collisionGroup, trace_t& pm, CBaseEntity *player)
{
	Ray_t ray;
	CTraceFilter filter;
	filter.pSkip = reinterpret_cast<void*>(player);

	ray.Init(start, end, player->GetMins(), player->GetMaxs());
	Interfaces::pEngineTrace->TraceRay(ray, fMask, &filter, &pm);
}

void RebuildGameMovement::WalkMove(CBaseEntity *player)
{
	Math::AngleVectorsNew(player->GetEyeAngles(), forward[player->GetIndex()], right[player->GetIndex()], up[player->GetIndex()]);  // Determine movement angles
																																   // Copy movement amounts
	Interfaces::pMoveHelper->SetHost(player);
	fmove[player->GetIndex()] = Interfaces::pMoveHelper->m_flForwardMove;
	smove[player->GetIndex()] = Interfaces::pMoveHelper->m_flSideMove;
	Interfaces::pMoveHelper->SetHost(nullptr);


	if (forward[player->GetIndex()][2] != 0)
	{
		forward[player->GetIndex()][2] = 0;
		Math::NormalizeVector(forward[player->GetIndex()]);
	}

	if (right[player->GetIndex()][2] != 0)
	{
		right[player->GetIndex()][2] = 0;
		Math::NormalizeVector(right[player->GetIndex()]);
	}


	for (i[player->GetIndex()] = 0; i[player->GetIndex()]<2; i[player->GetIndex()]++)       // Determine x and y parts of velocity
		wishvel[player->GetIndex()][i[player->GetIndex()]] = forward[player->GetIndex()][i[player->GetIndex()]] * fmove[player->GetIndex()] + right[player->GetIndex()][i[player->GetIndex()]] * smove[player->GetIndex()];

	wishvel[player->GetIndex()][2] = 0;             // Zero out z part of velocity

	wishdir[player->GetIndex()] = wishvel[player->GetIndex()]; // Determine maginitude of speed of move
	wishspeed[player->GetIndex()] = wishdir[player->GetIndex()].NormalizeMovement();

	//
	// Clamp to server defined max speed
	//
	Interfaces::pMoveHelper->SetHost(player);
	if ((wishspeed[player->GetIndex()] != 0.0f) && (wishspeed[player->GetIndex()] > Interfaces::pMoveHelper->m_flMaxSpeed))
	{
		VectorMultiply(wishvel[player->GetIndex()], player->GetMaxSpeed() / wishspeed[player->GetIndex()], wishvel[player->GetIndex()]);
		wishspeed[player->GetIndex()] = player->GetMaxSpeed();
	}
	Interfaces::pMoveHelper->SetHost(nullptr);
	// Set pmove velocity
	player->GetVelocity()[2] = 0;
	Accelerate(player, wishdir[player->GetIndex()], wishspeed[player->GetIndex()], Interfaces::pCVar->FindVar("sv_accelerate")->GetFloat());
	player->GetVelocity()[2] = 0;

	// Add in any base velocity to the current velocity.
	VectorAdd(player->GetVelocity(), player->GetVelocity(), player->GetVelocity());

	spd[player->GetIndex()] = player->GetVelocity().Length();

	if (spd[player->GetIndex()] < 1.0f)
	{
		player->GetVelocity().Zero();
		// Now pull the base velocity back out.   Base velocity is set if you are on a moving object, like a conveyor (or maybe another monster?)
		VectorSubtract(player->GetVelocity(), player->GetVelocity(), player->GetVelocity());
		return;
	}

	// first try just moving to the destination	
	dest[player->GetIndex()][0] = player->GetAbsOrigin()[0] + player->GetVelocity()[0] * Interfaces::pGlobals->frametime;
	dest[player->GetIndex()][1] = player->GetAbsOrigin()[1] + player->GetVelocity()[1] * Interfaces::pGlobals->frametime;
	dest[player->GetIndex()][2] = player->GetAbsOrigin()[2];

	// first try moving directly to the next spot
	TracePlayerBBox(player->GetAbsOrigin(), dest[player->GetIndex()], MASK_PLAYERSOLID, 8, pm[player->GetIndex()], player);

	// If we made it all the way, then copy trace end as new player position.
	Interfaces::pMoveHelper->SetHost(player);
	Interfaces::pMoveHelper->m_outWishVel += wishdir[player->GetIndex()] * wishspeed[player->GetIndex()];
	Interfaces::pMoveHelper->SetHost(nullptr);

	if (pm[player->GetIndex()].fraction == 1)
	{
		player->SetAbsOrigin(pm[player->GetIndex()].endpos);
		// Now pull the base velocity back out.   Base velocity is set if you are on a moving object, like a conveyor (or maybe another monster?)
		VectorSubtract(player->GetVelocity(), player->GetVelocity(), player->GetVelocity());

		//tayOnGround(); //needed?
		return;
	}

	// Don't walk up stairs if not on ground.
	if (!(player->GetFlags() & FL_ONGROUND))
	{
		// Now pull the base velocity back out.   Base velocity is set if you are on a moving object, like a conveyor (or maybe another monster?)
		VectorSubtract(player->GetVelocity(), player->GetVelocity(), player->GetVelocity());
		return;
	}

	StepMove(player, dest[player->GetIndex()], pm[player->GetIndex()]);

	// Now pull the base velocity back out.   Base velocity is set if you are on a moving object, like a conveyor (or maybe another monster?)
	VectorSubtract(player->GetVelocity(), player->GetVelocity(), player->GetVelocity());


}

void RebuildGameMovement::FinishGravity(CBaseEntity *player)
{
	float ent_gravity;

	ent_gravity = Interfaces::pCVar->FindVar("sv_gravity")->GetFloat();

	// Get the correct velocity for the end of the dt 
	player->GetVelocity()[2] -= (ent_gravity * Interfaces::pCVar->FindVar("sv_gravity")->GetFloat() * Interfaces::pGlobals->frametime * 0.5);

	CheckVelocity(player);
}

void RebuildGameMovement::FullWalkMove(CBaseEntity *player)
{

	StartGravity(player);

	// Fricion is handled before we add in any base velocity. That way, if we are on a conveyor, 
	//  we don't slow when standing still, relative to the conveyor.
	if (player->GetFlags() & FL_ONGROUND)
	{
		player->GetVelocity()[2] = 0.0;
		Friction(player);
	}

	// Make sure velocity is valid.
	CheckVelocity(player);

	if (player->GetFlags() & FL_ONGROUND)
	{
		WalkMove(player);
	}
	else
	{
		AirMove(player);  // Take into account movement when in air.
	}

	// Make sure velocity is valid.
	CheckVelocity(player);

	// Add any remaining gravitational component.
	FinishGravity(player);


	// If we are on ground, no downward velocity.
	if (player->GetFlags() & FL_ONGROUND)
	{
		player->GetVelocity()[2] = 0;
	}

	CheckFalling(player);
}

void RebuildGameMovement::Friction(CBaseEntity *player)
{
	// Calculate speed
	speed[player->GetIndex()] = player->GetVelocity().Length();

	// If too slow, return
	if (speed[player->GetIndex()] < 0.1f)
	{
		return;
	}

	drop[player->GetIndex()] = 0;

	// apply ground friction
	if (player->GetFlags() & FL_ONGROUND)  // On an CBaseEntity that is the ground
	{
		friction[player->GetIndex()] = Interfaces::pCVar->FindVar("sv_friction")->GetFloat() * player->m_surfaceFriction();

		//  Bleed off some speed, but if we have less than the bleed
		//  threshold, bleed the threshold amount.


		control[player->GetIndex()] = (speed[player->GetIndex()] < Interfaces::pCVar->FindVar("sv_stopspeed")->GetFloat()) ? Interfaces::pCVar->FindVar("sv_stopspeed")->GetFloat() : speed[player->GetIndex()];

		// Add the amount to the drop amount.
		drop[player->GetIndex()] += control[player->GetIndex()] * friction[player->GetIndex()] * Interfaces::pGlobals->frametime;
	}

	// scale the velocity
	newspeed[player->GetIndex()] = speed[player->GetIndex()] - drop[player->GetIndex()];
	if (newspeed[player->GetIndex()] < 0)
		newspeed[player->GetIndex()] = 0;

	if (newspeed[player->GetIndex()] != speed[player->GetIndex()])
	{
		// Determine proportion of old speed we are using.
		newspeed[player->GetIndex()] /= speed[player->GetIndex()];
		// Adjust velocity according to proportion.
		VectorMultiply(player->GetVelocity(), newspeed[player->GetIndex()], player->GetVelocity());
	}

	// TO DO FIX THIS SHIT!
	//player->GetVelocity() -= (1.f - newspeed[player->GetIndex()]) * player->GetVelocity();
}


void RebuildGameMovement::CheckFalling(CBaseEntity *player)
{
	// this function really deals with landing, not falling, so early out otherwise
	if (player->GetFallVelocity() <= 0)
		return;

	if (!player->GetHealth() && player->GetFallVelocity() >= 303.0f)
	{
		bool bAlive = true;
		float fvol = 0.5;

		//
		// They hit the ground.
		//
		if (player->GetVelocity().z < 0.0f)
		{
			// Player landed on a descending object. Subtract the velocity of the ground CBaseEntity.
			player->GetFallVelocity() += player->GetVelocity().z;
			player->GetFallVelocity() = max(0.1f, player->GetFallVelocity());
		}

		if (player->GetFallVelocity() > 526.5f)
		{
			fvol = 1.0;
		}
		else if (player->GetFallVelocity() > 526.5f / 2)
		{
			fvol = 0.85;
		}
		else if (player->GetFallVelocity() < 173)
		{
			fvol = 0;
		}

	}

	// let any subclasses know that the player has landed and how hard

	//
	// Clear the fall velocity so the impact doesn't happen again.
	//
	player->GetFallVelocity() = 0;
}
const int nanmask = 255 << 23;
#define	IS_NAN(x) (((*(int *)&x)&nanmask)==nanmask)
void RebuildGameMovement::CheckVelocity(CBaseEntity *player)
{
	Vector org = player->GetAbsOrigin();

	for (i[player->GetIndex()] = 0; i[player->GetIndex()] < 3; i[player->GetIndex()]++)
	{
		// See if it's bogus.
		if (IS_NAN(player->GetVelocity()[i[player->GetIndex()]]))
		{
			player->GetVelocity()[i[player->GetIndex()]] = 0;
		}

		if (IS_NAN(org[i[player->GetIndex()]]))
		{
			org[i[player->GetIndex()]] = 0;
			player->SetAbsOrigin(org);
		}

		// Bound it.
		if (player->GetVelocity()[i[player->GetIndex()]] > Interfaces::pCVar->FindVar("sv_maxvelocity")->GetFloat())
		{
			player->GetVelocity()[i[player->GetIndex()]] = Interfaces::pCVar->FindVar("sv_maxvelocity")->GetFloat();
		}
		else if (player->GetVelocity()[i[player->GetIndex()]] < -Interfaces::pCVar->FindVar("sv_maxvelocity")->GetFloat())
		{
			player->GetVelocity()[i[player->GetIndex()]] = -Interfaces::pCVar->FindVar("sv_maxvelocity")->GetFloat();
		}
	}
}
void RebuildGameMovement::StartGravity(CBaseEntity *player)
{
	if (!player || !player->GetHealth())
		return;

	Vector pVel = player->GetVelocity();

	pVel[2] -= (Interfaces::pCVar->FindVar("sv_gravity")->GetFloat() * 0.5f * Interfaces::pGlobals->interval_per_tick);
	pVel[2] += (player->GetVelocity()[2] * Interfaces::pGlobals->interval_per_tick);

	player->GetVelocity() = pVel;

	Vector tmp = player->GetVelocity();
	tmp[2] = 0.f;
	player->GetVelocity() = tmp;
}