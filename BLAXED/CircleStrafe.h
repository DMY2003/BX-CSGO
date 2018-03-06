#include "SDK.h"
#include "Global.h"
#include "Prediction.h"

namespace CircleStrafe
{
	float RightMovement = -1;
	bool IsActive = false;
	float StrafeAngle = 0;
	QAngle CurrentAngles = Vector(0, 0, 0);
	QAngle LastAngles = Vector(0,0,0);

	float GetTraceFractionWorldProps(Vector start, Vector end)
	{
		Ray_t rayF, rayL, rayR;
		CTraceFilterWorldOnly filter;
		trace_t traceF, traceL, traceR;

		//filter.pLocal = G::pLocal;

		QAngle angle = Math::CalcAngle(start, end);		
		Vector forward, left;
		Math::AngleVectors(angle, forward);
		Math::AngleVectors(angle + QAngle(0, 90, 0), forward);
		forward.NormalizeInPlace();
		left.NormalizeInPlace();

		float bodyOffset = 32;

		rayF.Init(start, end);
		rayL.Init(start + left * bodyOffset, end + left * bodyOffset);
		rayR.Init(start - left * bodyOffset, end - left * bodyOffset);

		float fractionF, fractionL, fractionR;

		I::pEngineTrace->TraceRay(rayF, MASK_ALL, (ITraceFilter*)&filter, &traceF);
		I::pEngineTrace->TraceRay(rayL, MASK_ALL, (ITraceFilter*)&filter, &traceL);
		I::pEngineTrace->TraceRay(rayR, MASK_ALL, (ITraceFilter*)&filter, &traceR);

		fractionF = traceF.fraction;
		fractionL = traceL.fraction;
		fractionR = traceR.fraction;

		return min(fractionF, min(fractionL, fractionR));
	}
	
	void Rotate(CUserCmd* pCmd, float rotation)
	{
		rotation = DEG2RAD(rotation);

		float cos_rot = cos(rotation);
		float sin_rot = sin(rotation);

		float new_forwardmove = (cos_rot * pCmd->forwardmove) - (sin_rot * pCmd->sidemove);
		float new_sidemove = (sin_rot * pCmd->forwardmove) + (cos_rot * pCmd->sidemove);

		pCmd->forwardmove = new_forwardmove;
		pCmd->sidemove = new_sidemove;
	}

	void Start(CUserCmd* pCmd)
	{
		auto pLocal = G::pLocal;
		
		if (!IsActive)
			StrafeAngle = 0;

		IsActive = true;

		CurrentAngles = pCmd->viewangles;
		/*I::pEngineClient->GetViewAngles(CurrentAngles);
		CurrentAngles.x = 0;*/

		if (pLocal->GetVelocity().Length() > 100)
		{
			Math::VectorAngles(pLocal->GetVelocity(), CurrentAngles);
		}

		Vector Forward, Right, Left, VelForward;
		Math::AngleVectors(CurrentAngles, Forward);
		Math::AngleVectors(CurrentAngles + QAngle(0, -90, 0), Right);
		Math::AngleVectors(CurrentAngles + QAngle(0, 90, 0), Left);

		Forward.NormalizeInPlace();
		Right.NormalizeInPlace();
		Left.NormalizeInPlace();

		float frac = GetTraceFractionWorldProps(pLocal->GetAbsOrigin(), pLocal->GetVelocity() * TICKS_TO_TIME(5));
		Vector origin = Vector(0, 0, 5) + pLocal->GetAbsOrigin() + pLocal->GetVelocity().GetNormalized() * frac;

		float LeftPath = 0; // GetTraceFractionWorldProps(origin + Vector(0, 0, 5), origin + Left * 250.0f + Vector(0, 0, 5));
		float RightPath = 0; // GetTraceFractionWorldProps(origin + Vector(0, 0, 5), origin + Right * 250.0f + Vector(0, 0, 5));

		for (float i = 5.0f; i < 90.0f; i += 5.0f)
		{
			Vector val, var;
			Math::AngleVectors(CurrentAngles + QAngle(0, i, 0), val);
			Math::AngleVectors(CurrentAngles + QAngle(0, -i, 0), var);

			LeftPath += min(
				GetTraceFractionWorldProps(origin + Vector(0, 0, 5), origin + val * 250.0f + Vector(0, 0, 5)),
				min(
					GetTraceFractionWorldProps(origin + Vector(0, 0, 5 + 32), origin + val * 250.0f + Vector(0, 0, 5 + 32)),
					min(
						GetTraceFractionWorldProps(origin + Vector(0, 0, 5) + Left * 50, origin + val * 250.0f + Vector(0, 0, 5) + Left * 50),
						GetTraceFractionWorldProps(origin + Vector(0, 0, 5) + Right * 50, origin + val * 250.0f + Vector(0, 0, 5) + Right * 50)
					)
				)
			);

			RightPath += min(
				GetTraceFractionWorldProps(origin + Vector(0, 0, 5), origin + var * 250.0f + Vector(0, 0, 5)),
				min(
					GetTraceFractionWorldProps(origin + Vector(0, 0, 5 + 32), origin + var * 250.0f + Vector(0, 0, 5 + 32)),
					min(
						GetTraceFractionWorldProps(origin + Vector(0, 0, 5) + Left * 50, origin + var * 250.0f + Vector(0, 0, 5) + Left * 50),
						GetTraceFractionWorldProps(origin + Vector(0, 0, 5) + Right * 50, origin + var * 250.0f + Vector(0, 0, 5) + Right * 50)
					)
				)
			);
		}

		if (LeftPath > RightPath)
			RightMovement = -1;
		else
			RightMovement = 1;
	}

	void Strafe(CUserCmd* pCmd)
	{
		auto pLocal = G::pLocal;
		Vector Velocity = pLocal->GetVelocity();

		Vector velFw = Velocity.GetNormalized();

		Vector VelLeft, VelRight, VelAng;

		Math::VectorAngles(Velocity, VelAng);
		Math::AngleVectors(Velocity + QAngle(0, -90, 0), VelRight);
		Math::AngleVectors(Velocity + QAngle(0, 90, 0), VelLeft);
		VelLeft.Normalize();
		VelRight.Normalize();

		float frac = GetTraceFractionWorldProps(pLocal->GetAbsOrigin(), pLocal->GetVelocity() * TICKS_TO_TIME(5));
		Vector origin = Vector(0, 0, 5) + pLocal->GetAbsOrigin() + pLocal->GetVelocity().GetNormalized() * frac;

		Velocity.z = 0;
		float Speed = Velocity.Length();
		if (Speed < 45) Speed = 45;
		//if (Speed > 800) Speed = 800;

		float retrack = /*5.9f*/cfg.Misc.retrackSpeed + (Speed / 1500.0f) * 5.0f;

		float FinalPath = min(
			GetTraceFractionWorldProps(origin + Vector(0, 0, 5), origin + Vector(0, 0, 5) + Velocity / cfg.Misc.speedDivisor),
			min(
				GetTraceFractionWorldProps(origin + Vector(0, 0, 5 + 32), origin + Vector(0, 0, 5 + 32) + Velocity / cfg.Misc.speedDivisor),
				min(
					GetTraceFractionWorldProps(origin + Vector(0, 0, 5) + VelLeft * 50, origin + Vector(0, 0, 5) + Velocity / cfg.Misc.speedDivisor + VelLeft * 50),
					GetTraceFractionWorldProps(origin + Vector(0, 0, 5) + VelRight * 50, origin + Vector(0, 0, 5) + Velocity / cfg.Misc.speedDivisor + VelRight * 50)
				)
			)
		);

		float Delta = (275.0f / Speed) * (2.0f / FinalPath) * (128.0f / (1.7f / I::pGlobals->interval_per_tick)) * retrack;
	
		static int lastChangeDir = 0;

		/*if (Delta > 30)
		{
			if (abs(lastChangeDir - I::pGlobals->tickcount) > 16)
			{
				RightMovement *= -1;
				lastChangeDir = I::pGlobals->tickcount;
			}
		}*/

		float DeltaAngle = RightMovement * min(Delta, 15.0f);
		StrafeAngle += DeltaAngle;
		
		if (fabs(StrafeAngle) >= 360.0f)
		{
			StrafeAngle = 0.0f;
			IsActive = false;
			//RightMovement = 0;
			LastAngles = Vector(0, 0, 0);
		}
		else
		{	
			if (LastAngles != Vector(0,0,0))
				StrafeAngle -= Math::NormalizeYawInPlace(LastAngles.y - pCmd->viewangles.y);

			pCmd->forwardmove = (cos((StrafeAngle + 90 * RightMovement) * (M_PI / 180.0f)) * 450.0f);
			pCmd->sidemove = (sin((StrafeAngle + 90 * RightMovement) * (M_PI / 180.0f)) * 450.0f);
		}

		LastAngles = pCmd->viewangles;
	}

};