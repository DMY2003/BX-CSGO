#include "SDK.h"
#include "Global.h"
#include "Prediction.h"

float Prediction::GetLerp()
{
	static ConVar* cl_updaterate = NULL;
	static ConVar* sv_minupdaterate = NULL;
	static ConVar* sv_maxupdaterate = NULL;
	static ConVar* cl_interp_ratio = NULL;
	static ConVar* cl_interp = NULL;
	static ConVar* sv_client_min_interp_ratio = NULL;
	static ConVar* sv_client_max_interp_ratio = NULL;

	if (!cl_updaterate)
	{
		cl_updaterate = I::pCVar->FindVar(xorstr("cl_updaterate"));
		sv_minupdaterate = I::pCVar->FindVar(xorstr("sv_minupdaterate"));
		sv_maxupdaterate = I::pCVar->FindVar(xorstr("sv_maxupdaterate"));
		cl_interp_ratio = I::pCVar->FindVar(xorstr("cl_interp_ratio"));
		cl_interp = I::pCVar->FindVar(xorstr("cl_interp"));
		sv_client_min_interp_ratio = I::pCVar->FindVar(xorstr("sv_client_min_interp_ratio"));
		sv_client_max_interp_ratio = I::pCVar->FindVar(xorstr("sv_client_max_interp_ratio"));
	}

	float flLerpRatio = cl_interp_ratio->GetFloat();

	if (flLerpRatio == 0)
		flLerpRatio = 1.0f;

	float flLerpAmount = cl_interp->GetFloat();

	if (sv_client_min_interp_ratio->GetFloat() != -1)
	{
		flLerpRatio = Math::Clamp(flLerpRatio, sv_client_min_interp_ratio->GetFloat(), sv_client_max_interp_ratio->GetFloat());
	}
	else
	{
		if (flLerpRatio == 0)
			flLerpRatio = 1.0f;
	}
	
	float updaterate = Math::Clamp(cl_updaterate->GetFloat(), sv_minupdaterate->GetFloat(), sv_maxupdaterate->GetFloat());

	return max(flLerpAmount, flLerpRatio / updaterate);	
}

int Prediction::GetEstimatedServerTickCount(float latency)
{
	return (int)floorf(((latency / I::pGlobals->interval_per_tick) + 0.5f) + 1.0f + (float)(I::pGlobals->tickcount));	
}

float Prediction::GetEstimatedServerTime(float latency)
{
	return G::pLocal->GetTickBase() * I::pGlobals->interval_per_tick + latency;
}

float Prediction::GetLatencyIn()
{
	INetChannelInfo *net = I::pEngineClient->GetNetChannelInfo();
	
	if (net == NULL)
		return 0;
	
	return net->GetLatency(FLOW_INCOMING);
}

float Prediction::GetLatencyOut()
{
	INetChannelInfo *net = I::pEngineClient->GetNetChannelInfo();

	if (net == NULL)
		return 0;

	return net->GetLatency(FLOW_OUTGOING);
}

bool Prediction::TickIsValid(int tick)
{
	INetChannel *net = (INetChannel*)I::pEngineClient->GetNetChannelInfo();

	const float SV_MAXUNLAG = 1.0f;

	if (fabsf(TICKS_TO_TIME(I::pGlobals->tickcount + TIME_TO_TICKS(GetLerp()) - tick)) > 0.2f)
		return false;

	return true;
}
