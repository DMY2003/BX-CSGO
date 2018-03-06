#pragma once

#include "Global.h"
#include "SDK.h"
#include "HUDIndicators.h"
#include "Render.h"
#include "Draw.h"

HUDIndicators *hudIndicators = new HUDIndicators();

HUDIndicators::HUDIndicators()
{
	lowerBodyChoke = true;
	lagCompensated = true;
}

void HUDIndicators::Paint()
{
	CBaseEntity *pLocal = G::pLocal;

	if (!pLocal)
		return;

	if (!I::pEngineClient->IsInGame() || !I::pEngineClient->IsConnected() || !pLocal->IsAlive())
		return;

	int height, width;

	I::pEngineClient->GetScreenSize(width, height);

	
		int offsetH = height / 30;
		int fontT = height / 40;

		I::pSurface->SetFontGlyphSet(hFntIndicators, xorstr("Verdana"), fontT, FW_BOLD, NULL, NULL, FONTFLAG_OUTLINE, 0, 0);

		D::DrawString(hFntIndicators, 4, height - offsetH - fontT, lowerBodyChoke ? Color(255, 40, 40) : Color(40, 255, 40), FONT_LEFT, "%s", xorstr("LBY"));

		if (!G::pLocal->CheckOnGround())
			D::DrawString(hFntIndicators, 4, height - offsetH - 2 * fontT, lagCompensated ? Color(255, 40, 40) : Color(40, 255, 40), FONT_LEFT, "%s", xorstr("LC"));
	
}
