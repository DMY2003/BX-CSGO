#include "SDK.h"
#include "Global.h"
#include "PaintTraverse.h"
#include "Draw.h"
#include "HUDIndicators.h"
#include "HitMarkers.h"
#include "ESP.h"
#include "LobbyManager.h"
#include "ResolverV2.h"
#include "AimbotRecode.h"
PaintTraverse_t oPaintTraverse = NULL;

void __fastcall PaintTraverse(IPanel* _this, void *edx, unsigned int VGUIPanel, bool forceRepaint, bool allowForce)
{
	if (G::exit)
		return oPaintTraverse(_this, VGUIPanel, forceRepaint, allowForce);

	if (!I::pSurface || !I::pPanel)
		return oPaintTraverse(_this, VGUIPanel, forceRepaint, allowForce);

	if (!I::pEngineClient || !I::pClientEntityList)
		return oPaintTraverse(_this, VGUIPanel, forceRepaint, allowForce);

	static unsigned int drawPanel = 0;
	static bool setup = false;

	if (!setup)
	{
		D::SetupFonts();
		setup = true;
	}

	const char* panelname = I::pPanel->GetName(VGUIPanel);

	int height, width;
	I::pEngineClient->GetScreenSize(width, height);

	if (cfg.Visual.noScope)
	{
		if (!strcmp(panelname, xorstr("HudZoom")) && G::pLocal && G::pLocal->GetLifeState() == LIFE_ALIVE)
		{
			bool old = G::pLocal->IsScoped();

			G::pLocal->SetIsScoped(false);

			oPaintTraverse(_this, VGUIPanel, forceRepaint, allowForce);

			G::pLocal->SetIsScoped(old);

			if (G::pLocal->IsScoped())
			{
				D::DrawLine(width / 2, 0, width / 2, height, Color(0, 0, 0));
				D::DrawLine(0, height / 2, width, height / 2, Color(0, 0, 0));
			}

			return;
		}
	}

	if (!drawPanel)
	{
		// GetName(VGUIPanel).compare("MatSystemTopPanel"))
		if (panelname[0] == 'M' && panelname[2] == 't')
			drawPanel = VGUIPanel;
	}

	if (VGUIPanel != drawPanel)
		return oPaintTraverse(_this, VGUIPanel, forceRepaint, allowForce);

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			G::viewMatrix[i][j] = I::pEngineClient->WorldToScreenMatrix()[i][j];
		}
	}

	int max = I::pEngineClient->GetMaxClients();
	for (int i = 0; i < max; i++)
	{
		CBaseEntity *entity = I::pClientEntityList->GetClientEntity(i);

		if (ESP::Filter(entity)) continue;

		if (entity->GetIndex() >= 0 && entity->GetIndex() < MAX_PLAYERS)
		{
			G::boneCacheESP[i].valid = entity->SetupBones(G::boneCacheESP[i].pBoneToWorldOut, MAX_BONES, 0x100, I::pEngineClient->GetLastTimeStamp());
			G::absOriginCache[i] = entity->GetAbsOrigin();
		}
	}

	if (G::pLocal && G::pLocal->GetIndex() >= 0 && G::pLocal->GetIndex() < MAX_PLAYERS)
		G::absOriginCache[G::pLocal->GetIndex()] = G::pLocal->GetAbsOrigin();

	G::canDrawESP = true;

	if (cfg.Visual.screenshotCleaner)
		return oPaintTraverse(_this, VGUIPanel, forceRepaint, allowForce);

	oPaintTraverse(_this, VGUIPanel, forceRepaint, allowForce);

	I::pSurface->DrawSetColor(Color(0, 0, 0));
	I::pSurface->DrawFilledRect(1, 1, 1 + 1, 1 + 1);

	hitMarkers->Paint();
	hudIndicators->Paint();
	AimRage::Get().MultiPointDraw();
	CBaseEntity *pLocal = G::pLocal;

}
