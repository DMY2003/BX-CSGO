#pragma once

#include "Singleton.h"
#include "SDK.h"
class CBaseEntity;

class RebuildGameMovement : public Singleton<RebuildGameMovement>
{
private:

	int bumpcount[64], numbumps[64];
	Vector dir[64];
	float d[64];
	int numplanes[64];
	Vector primal_velocity[64], original_velocity[64];
	Vector new_velocity[64];
	int i[64], j[64];
	trace_t	pm[64];
	Vector end[64];
	Vector vec3_origin[64];
	float time_left[64], allFraction[64];
	int blocked[64];

	float addspeed[64], accelspeed[64], currentspeed[64];
	float wishspd[64];
	Vector wishvel[64];
	float spd[64];
	float fmove[64], smove[64];
	Vector wishdir[64];
	float wishspeed[64];

	Vector dest[64];
	Vector forward[64], right[64], up[64];
	float speed[64], newspeed[64], control[64];
	float friction[64];
	float drop[64];
	float ent_gravity[64];

private:

public:

	void SetAbsOrigin(CBaseEntity *C_BasePlayer, const Vector &vec);
	void FullWalkMove(CBaseEntity *C_BasePlayer);
	void CheckVelocity(CBaseEntity *C_BasePlayer);
	void FinishGravity(CBaseEntity *C_BasePlayer);
	void StepMove(CBaseEntity *C_BasePlayer, Vector &vecDestination, trace_t &trace);
	int ClipVelocity(Vector &in, Vector &normal, Vector &out, float overbounce);
	int TryPlayerMove(CBaseEntity *C_BasePlayer, Vector *pFirstDest, trace_t *pFirstTrace);
	void Accelerate(CBaseEntity *C_BasePlayer, Vector &wishdir, float wishspeed, float accel);
	void Friction(CBaseEntity *C_BasePlayer);
	void AirAccelerate(CBaseEntity *C_BasePlayer, Vector &wishdir, float wishspeed, float accel);
	void AirMove(CBaseEntity *C_BasePlayer);
	void WalkMove(CBaseEntity *C_BasePlayer);
	void CheckFalling(CBaseEntity *C_BasePlayer);
	void StartGravity(CBaseEntity *C_BasePlayer);
	void TracePlayerBBox(const Vector &start, const Vector &end, unsigned int fMask, int collisionGroup, trace_t &pm, CBaseEntity *C_BasePlayer);
};