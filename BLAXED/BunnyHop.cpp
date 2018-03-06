#include "SDK.h"
#include "Global.h"
#include "Configs.h"
#include "BunnyHop.h"
#include "AntiAimV2.h"
#include "ConvarSpoofer.h"
#include "CircleStrafe.h"
#include "BoxCircleStrafe.h"
#include "AutoStrafer.h"
#include "CircleStrafeV2.h"

bool GetWorldIntersection(Vector start, QAngle direction, float range, CBaseEntity *skip, trace_t &trace = trace_t())
{
	Ray_t ray;
	CTraceFilter filter;
	Vector forward;

	Math::AngleVectors(direction, forward);
	forward.NormalizeInPlace();

	Vector end = start + forward * range;

	ray.Init(start, end);
	filter.pSkip = skip;

	I::pEngineTrace->TraceRay(ray, MASK_SOLID, (ITraceFilter*)&filter, &trace);

	if (trace.m_pEnt && trace.m_pEnt->GetTeam() == G::pLocal->GetTeam())
		return false;

	return trace.fraction != 1.0f;
}

struct ColisionData
{
	trace_t traceF, traceL, traceR, traceM;
	bool colF, colL, colR;
};

bool GetCollision(CBaseEntity *entity, float baseYaw, float maxDist, ColisionData &data)
{
	Vector origin = entity->GetOrigin();

	data.colF = false;
	data.colL = false;
	data.colR = false;

	Vector forward, left;

	Math::AngleVectors(QAngle(0, baseYaw, 0), forward);
	Math::AngleVectors(QAngle(0, baseYaw - 90.0f, 0), left);

	forward.NormalizeInPlace();
	left.NormalizeInPlace();

	Vector startPos;

	float bodyWidth = 64 / 2.0f;

	startPos = origin;
	if (GetWorldIntersection(startPos, QAngle(0, baseYaw, 0), maxDist - bodyWidth, entity, data.traceF))
		data.colF = true;

	startPos = origin + left * bodyWidth;
	if (GetWorldIntersection(startPos, QAngle(0, baseYaw, 0), maxDist - bodyWidth, entity, data.traceL))
		data.colL = true;

	startPos = origin - left * bodyWidth;
	if (GetWorldIntersection(startPos, QAngle(0, baseYaw, 0), maxDist - bodyWidth, entity, data.traceR))
		data.colR = true;

	if (data.colF)
		data.traceM = data.traceF;
	else if (data.colL)
		data.traceM = data.traceL;
	else if (data.colR)
		data.traceM = data.traceR;

	if (data.colF || data.colL || data.colR)
	{
		return true;
	}

	return false;
}

float GetMinFreeAngle(CBaseEntity *entity, float baseYaw, float maxDist, bool &hasCollision)
{
	Vector origin = entity->GetOrigin();

	float maxDistCol = 0;
	float min = 0;

	hasCollision = false;
		
	for (float i = 0.0f; i > -360.0f; i -= 15.0f)
	{
		ColisionData colData;

		bool colision = GetCollision(entity, baseYaw + i, maxDist, colData);
				
		if (colision)
		{
			hasCollision = true;
			
			float dist = (origin - colData.traceM.endpos).Length();

			if (dist < maxDistCol)
			{
				min = i;
				maxDistCol = dist;
			}			
		}
		else
		{
			min = i;
			break;
		}
	}

	return min;
}

BunnyHop::BunnyHop()
{
	circleStrafeDbg = QAngle(0, 0, 0);
	d_col = Vector(0, 0, 0);
	d_colL = Vector(0, 0, 0);
	d_colR = Vector(0, 0, 0);
	usingTick = false;
}

void BunnyHop::Tick(bool chokePacket)
{
	d_col = Vector(0, 0, 0);
	d_colL = Vector(0, 0, 0);
	d_colR = Vector(0, 0, 0);

	usingTick = false;

	if (!cfg.Misc.bunnyhop && !cfg.Misc.autoStrafe)
		return;

	CUserCmd *pCmd = G::pCmd;
	CBaseEntity *pLocal = G::pLocal;

	Vector velocity = pLocal->GetVelocity();
	Vector origin = pLocal->GetOrigin();
	QAngle viewAngles = pCmd->viewangles;

	QAngle velocityAngles;
	Math::VectorAngles(velocity, velocityAngles);
	velocityAngles.NormalizeAngles();

	static bool bLastJumped = false;
	static bool circleStarted = false;
	static float circle = 0;
	static float startCircleYaw;
	static float dodge = 0;

	static bool swap = false;

	static bool circleKeyPressed = false;
	bool circleKey = GetAsyncKeyState(VK_LSHIFT) & 1;

	static bool cStrafeStart = false;
	static bool duckSwitch = false;

	if (circleKey)
		circleKeyPressed = !circleKeyPressed;

	float speed = velocity.Length2D();

	//bool inGround = (pLocal->GetFlags() & FL_ONGROUND);
	//bool inGround = (pLocal->GetVelocity().z <= 5.0f && pLocal->GetVelocity().z >= 0.0f || (pLocal->GetFlags() & FL_ONGROUND));

	bool freePath = true;

	//if (speed < 250)
	if (cfg.Misc.buunyhopStopAtWall)
	{
		trace_t trace1, trace2;
		
		antiAim->TraceWorld(QAngle(0, pCmd->viewangles.y, 0), pLocal->GetOrigin() + Vector(0, 0, 16), 64, trace1);
		antiAim->TraceWorld(QAngle(0, pCmd->viewangles.y, 0), pLocal->GetOrigin() + Vector(0, 0, 32), 32, trace2);

		float zdiff = fabsf(trace2.endpos.z - trace1.endpos.z);

		if (trace1.fraction != 1.0f && trace2.fraction != 1.0f && fabsf(zdiff - 16.0f) < 1.0f)
			freePath = false;
	}

	if (!freePath)
		return;

	/*{
		static float ls = pLocal->GetVelocity().Length();

		printf("%3d\n", (int)(pLocal->GetVelocity().Length() - ls));

		ls = pLocal->GetVelocity().Length();;
	}*/

	if (ConvarSpoofer::IsReady())
	{
		if (cfg.Misc.spoofAutoBunnyhop)
		{
			ConvarSpoofer::Get(xorstr("sv_autobunnyhopping"))->SetValue(1);
			ConvarSpoofer::Get(xorstr("sv_enablebunnyhopping"))->SetValue(1);
		}
		else
		{
			ConvarSpoofer::Get(xorstr("sv_autobunnyhopping"))->SetValue(I::pCVar->FindVar(xorstr("sv_autobunnyhopping"))->GetInt());
			ConvarSpoofer::Get(xorstr("sv_enablebunnyhopping"))->SetValue(I::pCVar->FindVar(xorstr("sv_enablebunnyhopping"))->GetInt());
		}
	}

	bool inGround = pLocal->CheckOnGround();

	if (pCmd->buttons & IN_JUMP || (circleStarted && !cfg.Misc.buunyhopStopAtWall))
	{
		usingTick = true;

		if (speed > 0)
		{
			if (inGround)
			{
				if (cfg.Misc.bunnyhop || cfg.Misc.spoofAutoBunnyhop)
					pCmd->buttons |= IN_JUMP;

				duckSwitch = false;
			}
		
			/*if (velocity.z >= -5.0f && velocity.z <= 0 && cfg.Misc.bunnyAutoDuck)
			{
				duckSwitch = true;
			}

			if (duckSwitch)
				pCmd->buttons |= IN_DUCK;*/

			if (cfg.Misc.bunnyAutoDuck && speed > 250)
				pCmd->buttons |= IN_DUCK;

			if (cfg.Misc.bunnyhop && !inGround)
				pCmd->buttons &= ~IN_JUMP;

			//if (velocity.z < -5.0f)
			
			if (cfg.Misc.autoStrafe)
			{
				swap = pCmd->command_number % 2 == 0;
				
				if (cfg.Misc.fastAutoStrafe)
				{
					/*static float circle = 0;

					ColisionData colDataF, colDataL, colDataR;

					float maxDist = speed / 1.6f;
					float targetAng = -15;

					bool colF = GetCollision(pLocal, velocityAngles.y, maxDist, colDataF);
					bool colL = GetCollision(pLocal, velocityAngles.y + 35.0f, maxDist, colDataL);
					bool colR = GetCollision(pLocal, velocityAngles.y - 35.0f, maxDist, colDataR);

					bunnyHop->d_col = Vector(0, 0, 0);
					bunnyHop->d_colL = Vector(0, 0, 0);
					bunnyHop->d_colR = Vector(0, 0, 0);

					bunnyHop->d_col = colF ? colDataF.traceM.endpos : bunnyHop->d_col;
					bunnyHop->d_colL = colL ? colDataL.traceM.endpos : bunnyHop->d_colL;
					bunnyHop->d_colR = colR ? colDataR.traceM.endpos : bunnyHop->d_colR;

					float finalPath = 1.0f;

					if (colF)
					{
						finalPath = colDataF.traceM.fraction;
					}
					else if (colR)
					{
						finalPath = colDataR.traceM.fraction;
					}

					float retrackSpeed = 2.0f;

					if (speed >= 700)
						retrackSpeed = 2.0f + 0.75f * speed / 1000;

					float turn = (275.0f / speed) * (2.0f / finalPath) * (128.0f / (1.7f / I::pGlobals->interval_per_tick)) * retrackSpeed;
					
					if (circleKeyPressed || !circleKeyPressed && (Math::NormalizeYawInPlace(viewAngles.y + circle) < 180.0f - 5.0f) && circleStarted)
					{	
						circle += turn;

						pCmd->sidemove = -450.0f;
						RotateMovement(pCmd, pCmd->viewangles.y + circle);

						circleStarted = true;
						Math::NormalizeYaw(circle);
					}*/
					
					if (!inGround)
					{
						if (circleKeyPressed || (!circleKeyPressed && cStrafeStart && CircleStrafe::IsActive))
						{
							circleStarted = true;

							if (!cfg.Misc.boxCircleStrafe)
							{
								if (!cStrafeStart)
								{
									CircleStrafe::Start(pCmd);
									cStrafeStart = true;
								}

								if (!CircleStrafe::IsActive)
								{
									CircleStrafe::IsActive = true;
									CircleStrafe::Start(pCmd);
								}

								CircleStrafe::Strafe(pCmd);
							}
							else
							{
								if (!cStrafeStart)
								{
									BoxCircleStrafe::Start(pCmd);
									cStrafeStart = true;
								}

								BoxCircleStrafe::started = true;

								BoxCircleStrafe::Strafe(pCmd);
							}
						}
						else
						{
							cStrafeStart = false;

							circle = viewAngles.y;
							circleStarted = false;
							CircleStrafe::LastAngles = Vector(0, 0, 0);
							CircleStrafe::IsActive = false;
							CircleStrafe::StrafeAngle = 0;

							BoxCircleStrafe::Stop();

							if (pCmd->mousedx < 0)
							{
								pCmd->forwardmove = 0;
								pCmd->sidemove = -450.0f;
							}
							else if (pCmd->mousedx > 0)
							{
								pCmd->forwardmove = 0;
								pCmd->sidemove = +450.0f;
							}
							else
							{
								/*pCmd->sidemove = swap ? 450.0f : -450.0f;
								RotateMovement(pCmd, (swap ? -yaw : yaw));*/

								/*pCmd->forwardmove = (10000.f) / (speed + 1);
								pCmd->sidemove = swap ? 450.f : -450.f;*/

								QAngle origAngles = pCmd->viewangles;

								if (pCmd->sidemove < 0)
									origAngles += 45.0f;
								else if (pCmd->sidemove > 0)
									origAngles -= 45.0f;

								AutoStrafer::BhopMovement(pCmd, origAngles);
							}
						}
					}
				}
				else
				{
					if (pCmd->mousedx < 0)
					{
						pCmd->sidemove = -450.0f;
					}
					else if (pCmd->mousedx > 0)
					{
						pCmd->sidemove = +450.0f;
					}
				}
			}			
		}
		else
		{
			if (!cfg.Misc.buunyhopStopAtWall)
			{
				bool freeSpace = true != GetWorldIntersection(pLocal->GetOrigin() + Vector(0, 0, 5), QAngle(0, viewAngles.y, 0), 50.0f, pLocal);

				if (freeSpace)
				{
					pCmd->forwardmove = 450.0f;
				}
			}
		}
	}
	else
	{
		circleStarted = false;
		circleKeyPressed = false;

		CircleStrafe::LastAngles = Vector(0, 0, 0);
		CircleStrafe::IsActive = false;
		CircleStrafe::StrafeAngle = 0;
		BoxCircleStrafe::Stop();
	}

	circleStrafeDbg = QAngle(0, circle, 0);

	oldAngles = pCmd->viewangles;
}

void RotateMovement(CUserCmd* pCmd, float rotation)
{
	rotation = DEG2RAD(rotation);

	float cos_rot = cos(rotation);
	float sin_rot = sin(rotation);

	float new_forwardmove = (cos_rot * pCmd->forwardmove) - (sin_rot * pCmd->sidemove);
	float new_sidemove = (sin_rot * pCmd->forwardmove) + (cos_rot * pCmd->sidemove);

	pCmd->forwardmove = new_forwardmove;
	pCmd->sidemove = new_sidemove;
}
