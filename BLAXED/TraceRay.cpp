#include "SDK.h"
#include "TraceRay.h"
#include "Global.h"
#include "AutoWall.h"

TraceRayHook_t oTraceRayHook = NULL;
bool useTraceRayHook = false;

void __fastcall TraceRayHook(void* ths, void*, Ray_t const &ray, unsigned int mask, ITraceFilter *filter, trace_t &trace)
{
	if (G::pLocal && useTraceRayHook)
	{
		/*CAutowallFilter newfilter;

		newfilter.me = G::pLocal;
		newfilter.mywep = newfilter.me->GetWeapon();

		oTraceRayHook(ths, ray, mask, &newfilter, trace);

		trace.surface.flags |= 4;*/

		CTraceFilterSkipTeam newFilter;

		newFilter.pLocal = G::pLocal;

		oTraceRayHook(ths, ray, mask, &newFilter, trace);
	}
	else
	{
		oTraceRayHook(ths, ray, mask, filter, trace);
	}
}

