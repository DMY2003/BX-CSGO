#pragma once

#include "SDK.h"
#include "Global.h"
#include "FrameStageNotify.h"

#include "ConvarSpoofer.h"

namespace NightMode
{
	void Apply();
	void Remove();
	void FrameStageNotify(ClientFrameStage_t stage);
	void HandleGameEvent(IGameEvent *pEvent);
};
