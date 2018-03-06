#pragma once

#include "SDK.h"
#include "Global.h"

namespace BoxCircleStrafe
{
	float strafeYaw = 0;
	bool started = false;
	QAngle lastAngles = Vector(0, 0, 0);
	Vector boxCenter = Vector(0, 0, 0);
	float boxRadius = 0;

	void TraceWorld(Vector start, Vector end, trace_t *trace)
	{
		CBaseEntity *pLocal = G::pLocal;

		Ray_t ray;
		ray.Init(start, end);

		CTraceFilterWorldOnly filter;
		//filter.pLocal = pLocal;

		I::pEngineTrace->TraceRay(ray, MASK_ALL, (ITraceFilter*)&filter, trace);
	}

	bool GetBoxCenter(Vector origin, Vector forward, Vector left, float maxSearch, Vector &center, float &radius)
	{
		float minZ = origin.z;

		Vector endFront, endLeft, endBack, endRight;

		for (float i = 0; i < maxSearch; i += 5.0f)
		{
			trace_t trace;

			Vector start = origin + forward * i + Vector(0, 0, 1);
			Vector end = origin + forward * i - Vector(0, 0, 15);

			TraceWorld(start, end, &trace);

			if (fabsf(trace.endpos.z - minZ) > 1.0f)
			{
				break;
			}

			endFront = trace.endpos;
		}

		for (float i = 0; i < maxSearch; i += 5.0f)
		{
			trace_t trace;

			Vector start = origin - forward * i + Vector(0, 0, 1);
			Vector end = origin - forward * i - Vector(0, 0, 15);

			TraceWorld(start, end, &trace);

			if (fabsf(trace.endpos.z - minZ) > 1.0f)
			{
				break;
			}

			endBack = trace.endpos;
		}

		for (float i = 0; i < maxSearch; i += 5.0f)
		{
			trace_t trace;

			Vector start = origin + left * i + Vector(0, 0, 1);
			Vector end = origin + left * i - Vector(0, 0, 15);

			TraceWorld(start, end, &trace);

			if (fabsf(trace.endpos.z - minZ) > 1.0f)
			{
				break;
			}

			endLeft = trace.endpos;
		}

		for (float i = 0; i < maxSearch; i += 5.0f)
		{
			trace_t trace;

			Vector start = origin - left * i + Vector(0, 0, 1);
			Vector end = origin - left * i - Vector(0, 0, 15);

			TraceWorld(start, end, &trace);

			if (fabsf(trace.endpos.z - minZ) > 1.0f)
			{
				break;
			}

			endRight = trace.endpos;
		}

		center = (((endFront + endBack) / 2.0f) + ((endLeft + endRight) / 2.0f)) / 2.0f;
		radius = (center - endFront).Length2D();

		return (radius < maxSearch - 5.0f);
	}

	bool OutOfBox(Vector origin)
	{
		if ((origin - boxCenter).Length2D() > boxRadius)
			return true;

		return false;
	}

	void Start(CUserCmd* pCmd)
	{
		CBaseEntity *pLocal = G::pLocal;

		QAngle angles = QAngle(0, pCmd->viewangles.y, 0);

		if (!started)
			strafeYaw = 0;

		Vector forward, left, right, up;

		Math::AngleVectors(angles, &forward, &right, &up);
		left = right * -1;
		forward.NormalizeInPlace();
		left.NormalizeInPlace();
		right.NormalizeInPlace();
		up.NormalizeInPlace();

		trace_t traceBox;
		TraceWorld(pLocal->GetOrigin(), pLocal->GetOrigin() - Vector(0, 0, 1000), &traceBox);

		GetBoxCenter(traceBox.endpos, forward,left, 64 * 15, boxCenter, boxRadius);

		//printf("radius: %f\n", boxRadius);

		started = true;
	}

	void Stop()
	{
		QAngle lastAngles = Vector(0, 0, 0);
		started = false;
		strafeYaw = 0;
	}

	void Strafe(CUserCmd* pCmd)
	{
		CBaseEntity *pLocal = G::pLocal;

		Vector origin = pLocal->GetOrigin();
		Vector velocity = pLocal->GetVelocity();
		Vector angles = pCmd->viewangles;
		Vector velAngles;
		Math::VectorAngles(velocity, velAngles);
		velAngles.NormalizeAngles();
		
		float speed = velocity.Length();
		float tanAngle = Math::NormalizeYawInPlace(Math::CalcAngle(origin, boxCenter).y - 90.0f);

		float airaccel = I::pCVar->FindVar(xorstr("sv_airaccelerate"))->GetFloat();
		float maxspeed = I::pCVar->FindVar(xorstr("sv_maxspeed"))->GetFloat();
		float maxDelta = (airaccel * maxspeed) * I::pGlobals->interval_per_tick;// / 2;

		float delta = 0;

		if (OutOfBox(origin))
		{
			delta = -cfg.Misc.retrackSpeed + Math::NormalizeYawInPlace(velAngles.y - tanAngle);
		}
		else
		{
			//delta = Math::NormalizeYawInPlace(velAngles.y - tanAngle);

			float FinalPath = 1.0f;
			delta = (275.0f / speed) * (2.0f / FinalPath) * (128.0f / (1.7f / I::pGlobals->interval_per_tick)) * cfg.Misc.retrackSpeed;
		}

		if (delta > maxDelta)
			delta = maxDelta;

		if (delta < -maxDelta)
			delta = -maxDelta;

		strafeYaw += delta;

		if (fabs(strafeYaw) >= 360.0f)
		{
			//QAngle lastAngles = Vector(0, 0, 0);
			started = false;
			strafeYaw = 0;
		}
		else
		{
			if (lastAngles != Vector(0, 0, 0))
				strafeYaw -= Math::NormalizeYawInPlace(lastAngles.y - angles.y);

			pCmd->forwardmove = min(450.0f, cos((strafeYaw + 90) * (M_PI / 180.0f)) * 450.0f);
			pCmd->sidemove = min(450.0f, sin((strafeYaw + 90) * (M_PI / 180.0f)) * 450.0f);
		}

		lastAngles = pCmd->viewangles;
	}
};
