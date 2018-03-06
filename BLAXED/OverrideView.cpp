#include "SDK.h"
#include "Global.h"
#include "Configs.h"
#include "OverrideView.h"
#include "ConvarSpoofer.h"
#include "AntiAimV2.h"

OverrideView_t oOverrideView = NULL;

void __fastcall OverrideView(void* _this, void* _edx, CViewSetup* setup)
{
	if (G::exit)
		return oOverrideView(_this, setup);

	CBaseEntity *pLocal = G::pLocal;

	if (!pLocal)
		return oOverrideView(_this, setup);

	if (!I::pEngineClient->IsInGame() || !I::pEngineClient->IsConnected())
		return oOverrideView(_this, setup);

	bool alive = pLocal->GetLifeState() == LIFE_ALIVE && pLocal->GetHealth() > 0;

	static bool lastCfg = cfg.Misc.thirdPerson;
	
	if (lastCfg != cfg.Misc.thirdPerson)
	{
		if (!cfg.Misc.thirdPerson)
		{
			I::pInput->m_fCameraInThirdPerson = false;
		}
	}

	lastCfg = cfg.Misc.thirdPerson;

	if (alive)// && !aimbot->IsUsingGrenade())
	{
		if (cfg.Misc.thirdPerson)
		{
			I::pInput->m_fCameraInThirdPerson = true;
		}	
	}
	else
	{
		I::pInput->m_fCameraInThirdPerson = false;
	}

	if (cfg.Misc.fovChanger != 0.0f)
	{
		if (!pLocal->IsScoped() || cfg.Visual.lockFov)
		{
			setup->fov = cfg.Misc.fovChanger;
		}

		static ConVar* zoom_sensitivity_ratio_mouse = NULL;
		static float defaultSens = 1.0f;

		if (!zoom_sensitivity_ratio_mouse)
		{
			zoom_sensitivity_ratio_mouse = I::pCVar->FindVar(xorstr("zoom_sensitivity_ratio_mouse"));
			defaultSens = zoom_sensitivity_ratio_mouse->GetFloat();
		}

		if (cfg.Visual.lockFov)
		{
			if (pLocal->IsScoped())
			{
				zoom_sensitivity_ratio_mouse->SetValue(2.0f);
			}
			else
			{
				zoom_sensitivity_ratio_mouse->SetValue(defaultSens);
			}
		}
	}
	
	oOverrideView(_this, setup);
}
