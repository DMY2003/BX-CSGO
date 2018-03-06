#include "SDK.h"
#include "InPrediction.h"
#include "Global.h"
#include "Hooks.h"

InPrediction_t oInPrediction = NULL;

bool __fastcall InPrediction(void *_this, void *edx)
{
	if (G::exit)
		return oInPrediction(_this);

	return oInPrediction(_this);
}
