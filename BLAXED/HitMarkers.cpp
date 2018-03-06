#include "SDK.h"
#include "Global.h"
#include "HitMarkers.h"
#include "Draw.h"
#include "Render.h"

HitMarkers *hitMarkers = new HitMarkers();

HitMarkers::HitMarkers()
{
	lastKillTime = 0;
	Clear();
}

void HitMarkers::Clear()
{
	markerIdx = 0;

	for (int i = 0; i < MAX_MARKERS; i++)
	{
		markers[i].show = false;
	}
}

void HitMarkers::RemoveOlder(float time)
{
	for (int i = 0; i < MAX_MARKERS; i++)
	{
		if (fabsf(time - markers[i].startTime) < (float)CLEAR_TIME)
		{
			
		}
		else
		{
			markers[i].show = false;
		}
	}	
}

void HitMarkers::Paint()
{

}

void HitMarkers::Draw()
{
	float fullAlpha = 0.35f;
	float timeDiff = fabsf(I::pGlobals->curtime - lastKillTime);

	if (timeDiff < fullAlpha)
	{
		int height, width;
		I::pEngineClient->GetScreenSize(width, height);

		int x = render->viewPort.Width / 2;
		int y = render->viewPort.Height / 2;
		int size = 6;

		float frac = 1.0f - timeDiff / fullAlpha;
		int alpha = (int)Math::Lerp(frac, 0, 255.0f);

		render->DrawLineFast((float)x - size, (float)y - size, (float)x - size / 2, (float)y - size / 2, D3D_COLOR_WHITE(alpha));
		render->DrawLineFast((float)x + size / 2, (float)y + size / 2, (float)x + size, (float)y + size, D3D_COLOR_WHITE(alpha));

		render->DrawLineFast((float)x + size, (float)y - size, (float)x + size - size / 2, (float)y - size / 2, D3D_COLOR_WHITE(alpha));
		render->DrawLineFast((float)x - size, (float)y + size, (float)x - size + size / 2, (float)y + size / 2, D3D_COLOR_WHITE(alpha));
	}

	RemoveOlder(I::pGlobals->curtime);

	if (cfg.Rage.Resolver.enabled)
	{
		for (int i = 0; i < MAX_MARKERS; i++)
		{
			if (markers[i].show)
			{
				Vector f, t;

				if (markers[i].type == 1)
				{
					
				}
				else if (markers[i].type == 0)
				{
					if (render->WorldToScreen(markers[i].to, t))
					{
						//D::DrawLine((int)f.x, (int)f.y, (int)t.x, (int)t.y, markers[i].color);

						float size = 2.0f;
						float x = t.x;
						float y = t.y;

						//render->DrawLine(x - size, y - size, x + size * 2, y + size * 2, 2.0f, true, D3DCOLOR_ARGB(markers[i].color.a(), markers[i].color.r(), markers[i].color.g(), markers[i].color.b()));
						//render->DrawLine(x + size * 2, y - size, x - size, y + size * 2, 2.0f, true, D3DCOLOR_ARGB(markers[i].color.a(), markers[i].color.r(), markers[i].color.g(), markers[i].color.b()));
						render->DrawFilledRectangle(x - size, y - size, size * 2, size * 2, D3DCOLOR_ARGB(markers[i].color.a(), markers[i].color.r(), markers[i].color.g(), markers[i].color.b()));
					}
				}
			}
		}
	}

	/*if (I::pGlobals->curtime - lastKillTime < 0.15f)
	{
	int height, width;
	I::pEngineClient->GetScreenSize(width, height);

	int x = width / 2;
	int y = height / 2;
	int size = 6;

	D::DrawLine(x - size, y - size, x + size, y + size, Color(255, 255, 255));
	D::DrawLine(x - size, y + size, x + size, y - size, Color(255, 255, 255));
	//D::DrawString(hFntESP, x, y + 15, Color(255, 255, 255), FONT_CENTER, "-%.0f", lastDamage);
	//D::DrawCircle(x, y, size, size, Color(255, 255, 255));
	}*/
}

int HitMarkers::Add(Vector from, Vector to, Vector forward, int type)
{
	int idx = markerIdx;

	markers[idx % MAX_MARKERS].from = from;
	markers[idx % MAX_MARKERS].to = to;
	markers[idx % MAX_MARKERS].forward = forward;
	markers[idx % MAX_MARKERS].show = true;
	markers[idx % MAX_MARKERS].startTime = I::pGlobals->curtime;
	markers[idx % MAX_MARKERS].color = (type == 0 ? Color(0, 255, 0, 255) : Color(255, 255, 255, 255));
	markers[idx % MAX_MARKERS].type = type;

	markerIdx++;

	return idx % MAX_MARKERS;
}

void HitMarkers::SetColor(int idx, Color color)
{
	markers[idx % MAX_MARKERS].color = color;
}

void HitMarkers::HandleGameEvent(IGameEvent* event)
{
	std::string name = event->GetName();

	if (name.find(xorstr("bullet_impact")) != -1)
	{
		int userid = event->GetInt(xorstr("userid"));
	
		CBaseEntity *entity = I::pClientEntityList->GetClientEntity(I::pEngineClient->GetPlayerForUserID(userid));

		if (entity && entity == G::pLocal)
		{
			float x = event->GetFloat(xorstr("x"));
			float y = event->GetFloat(xorstr("y"));
			float z = event->GetFloat(xorstr("z"));

			Vector point = Vector(x, y, z);

			Vector from = G::pLocal->GetEyePosition();

			Vector forward;
			QAngle ang = Math::CalcAngle(from, point);
			Math::AngleVectors(ang, forward);
			forward.NormalizeInPlace();

			hitMarkers->Add(from, point, forward, 0);
		}
	} 
	else if (name.find(xorstr("player_hurt")) != -1)
	{
		int userid = event->GetInt(xorstr("userid"));
		int attackerid = event->GetInt(xorstr("attacker"));
		float dmg_health = event->GetFloat(xorstr("dmg_health"));

		CBaseEntity *entity = I::pClientEntityList->GetClientEntity(I::pEngineClient->GetPlayerForUserID(userid));
		CBaseEntity *attacker = I::pClientEntityList->GetClientEntity(I::pEngineClient->GetPlayerForUserID(attackerid));

		if (entity && attacker)
		{
			if (attacker == G::pLocal)
			{
				lastKillTime = I::pGlobals->curtime;
				lastDamage = dmg_health;
			}
		}
	}
}
