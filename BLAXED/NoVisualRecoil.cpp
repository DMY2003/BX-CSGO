#include "SDK.h"
#include "Global.h"
#include "Configs.h"
#include "FrameStageNotify.h"

void NoVisualRecoil(ClientFrameStage_t Stage, int step)
{
	if (!cfg.Rage.noRecoil)
		return;

	CUserCmd *pCmd = G::pCmd;
	CBaseEntity *pLocal = G::pLocal;

	static QAngle oldAimPunch, oldViewPunch;

	if (step == 0)
	{
		if (Stage == FRAME_RENDER_START)
		{
			oldAimPunch = pLocal->GetAimPunchAngles();
			oldViewPunch = pLocal->GetViewPunchAngles();

			pLocal->SetAimPunchAngles(QAngle(0, 0, 0));
			pLocal->SetViewPunchAngles(QAngle(0, 0, 0));
		}
	}
	else if (step == 1)
	{
		if (Stage == FRAME_RENDER_START)
		{
			pLocal->SetAimPunchAngles(oldAimPunch);
			pLocal->SetViewPunchAngles(oldViewPunch);
		}
	}
}
