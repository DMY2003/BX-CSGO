#pragma once

#define TIME_TO_TICKS( dt )		( (int)( 0.5f + (float)(dt) /  I::pGlobals->interval_per_tick ) )
#define TICKS_TO_TIME( dt )		( I::pGlobals->interval_per_tick * (float)(dt) )

// http://www.unknowncheats.me/forum/counterstrike-global-offensive/193972-useless-shit-resolver-spent-5-days-writing.html

class Prediction
{
public:
	static float GetLerp();
	static bool TickIsValid(int tick);
	static int GetEstimatedServerTickCount(float latency);
	static float GetEstimatedServerTime(float latency);
	static float GetLatencyIn();
	static float GetLatencyOut();
};
