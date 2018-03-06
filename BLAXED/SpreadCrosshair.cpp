#pragma once

#include "SDK.h"
#include "Render.h"
#include "NoRecoil.h"
#include "SpreadCrosshair.h"
#include "AimbotV2.h"

SpreadCrosshair *spreadCrosshair = new SpreadCrosshair();

SpreadCrosshair::SpreadCrosshair()
{
	memset(&fbd, 0, sizeof(fbd));
}

void SpreadCrosshair::Tick(QAngle angles)
{
	CBaseEntity *pLocal = G::pLocal;

	if (!pLocal)
		return;

	if (!pLocal->IsAlive())
		return;

	float range = G::weapInfoCache[pLocal->GetIndex()].m_flRange;

	trace_t tr;
	Ray_t ray;
	CTraceFilter filter;
	filter.pSkip = pLocal;

	Vector start = pLocal->GetEyePosition();

	if (!cfg.Rage.noRecoil)
		angles += aimbotV2->RecoilCompensation(pLocal);

	Vector fw;
	Math::AngleVectors(angles, fw);
	fw.Normalize();

	Vector end = start + fw * range;

	ray.Init(start, end);
	I::pEngineTrace->TraceRay(ray, MASK_SHOT, &filter, &tr);
	center = tr.endpos;

	for (int i = 0; i < 256; i++)
	{
		fw = aimbotV2->GetSpreadForward(angles, i);
		end = start + fw * range;

		ray.Init(start, end);
		I::pEngineTrace->TraceRay(ray, MASK_SHOT, &filter, &tr);

		points[i] = tr.endpos;
	}

	AutoWall::GetDamageEx(end, start, NULL, pLocal, HITBOX_HEAD, &fbd);
}

void SpreadCrosshair::Draw()
{
	CBaseEntity *pLocal = G::pLocal;

	if (!pLocal->IsAlive())
		return;

	Vector w2s[256];
	Vector mins, maxs;
	
	mins = Vector(INFINITY / 2, INFINITY / 2, INFINITY / 2);
	maxs = Vector(0, 0, 0);

	Vector sCenter;

	if (render->WorldToScreen(center, sCenter))
	{
		for (int i = 0; i < 256; i++)
		{
			if (render->WorldToScreen(points[i], w2s[i]))
			{
				if (w2s[i].x < mins.x)
					mins.x = w2s[i].x;
				if (w2s[i].y < mins.y)
					mins.y = w2s[i].y;

				if (w2s[i].x > maxs.x)
					maxs.x = w2s[i].x;
				if (w2s[i].y > maxs.y)
					maxs.y = w2s[i].y;

				render->DrawFilledRectangle(w2s[i].x - 1, w2s[i].y - 1, 2, 2, D3D_COLOR_WHITE(170));
			}
		}

		float radius = min(render->viewPort.Width, max((maxs.x - mins.x) / 2, (maxs.y - mins.y) / 2));

		//render->DrawCircleFast(sCenter.x, sCenter.y, radius, radius, D3D_COLOR_WHITE(190));

		render->DrawStringWithFont(render->fntVerdana10, render->viewPort.Width / 2 + 10, render->viewPort.Height / 2 + 10 + 1, D3D_COLOR_BLACK(255), "%s:%d\n%s:%d", xorstr("P"), 3 - fbd.penetrate_count, xorstr("D"), (int)floorf(max(0, fbd.current_damage)));
		render->DrawStringWithFont(render->fntVerdana10, render->viewPort.Width / 2 + 10, render->viewPort.Height / 2 + 10, 3 - fbd.penetrate_count > 0 ? D3D_COLOR_GREEN(255) : D3D_COLOR_WHITE(255), "%s:%d\n%s:%d", xorstr("P"), 3 - fbd.penetrate_count, xorstr("D"), (int)floorf(max(0, fbd.current_damage)));
		
		//render->DrawCircleFast(render->viewPort.Width / 2, render->viewPort.Height / 2, radius, radius, D3D_COLOR_WHITE(190));
		
		//render->DrawStringWithFont(render->fntVerdana10, render->viewPort.Width / 2 + 10, render->viewPort.Height / 2 + 10 + 1, D3D_COLOR_BLACK(255), "%s:%d\n%s:%d", xorstr("P"), 3 - fbd.penetrate_count, xorstr("D"), (int)floorf(max(0, fbd.current_damage)));
		//render->DrawStringWithFont(render->fntVerdana10, render->viewPort.Width / 2 + 10, render->viewPort.Height / 2 + 10, D3D_COLOR_WHITE(255), "%s:%d\n%s:%d", xorstr("P"), 3 - fbd.penetrate_count, xorstr("D"), (int)floorf(max(0, fbd.current_damage)));
	}
}
