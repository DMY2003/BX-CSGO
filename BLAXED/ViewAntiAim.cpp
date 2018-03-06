#include "SDK.h"
#include "Global.h"
#include "Configs.h"
#include "FrameStageNotify.h"
#include "AimbotV2.h"
#include "AntiAimV2.h"

void ViewAntiAim(ClientFrameStage_t Stage, int step)
{
	if (!cfg.Rage.AntiAim.enabled)
		return;

	if (!I::pInput->m_fCameraInThirdPerson)
		return;

	CUserCmd *pCmd = G::pCmd;
	CBaseEntity *pLocal = G::pLocal;

	if (!pCmd || !pLocal)
		return;

	static QAngle oldVecAngles = Vector(0, 0, 0);

	if (cfg.Rage.AntiAim.viewAntiAim || cfg.Rage.AntiAim.viewAntiAimFake || cfg.Rage.AntiAim.viewLowerBodyYawTarget)
	{
		if (step == 0)
		{
			bool disable = antiAim->OverrideAA();

			if (Stage == FRAME_RENDER_START)
			{
				oldVecAngles = pLocal->GetVecAngles();

				QAngle angle = pLocal->GetVecAngles();

				if (!disable)
				{
					//if (!cfg.Rage.AntiAim.viewLowerBodyYawTarget)
					{
						if (cfg.Rage.AntiAim.viewAntiAim && cfg.Rage.AntiAim.viewAntiAimFake)
						{
							angle = pCmd->viewangles;
						}
						else if (cfg.Rage.AntiAim.viewAntiAim)
						{
							angle = antiAim->angReal;

							if (cfg.Rage.AntiAim.untrusted)
							{
								if (angle.x > 89.0f)
									angle.x = 89.0f;
								 
								if (angle.x < 89.0f)
									angle.x = 89.0f;
							}
						}
						else if (cfg.Rage.AntiAim.viewAntiAimFake)
						{
							angle = pLocal->GetEyeAngles();
						}

						if (pCmd->buttons & IN_ATTACK && aimbotV2->usingTick && G::SendPacket)
							angle = pCmd->viewangles;					
					}
					/*else
					{
						angle = pCmd->viewangles;
						angle.y = antiAim->lastLowerBodyReceived;

						/*if (cfg.Rage.AntiAim.viewAntiAimFake && G::SendPacket)
							angle = antiAim->chokeAngles;* /
					}*/
				}

				pLocal->SetVecAngles(angle);
			}
		}
		else if (step == 1)
		{
			pLocal->SetVecAngles(oldVecAngles);
		}
	}
}

