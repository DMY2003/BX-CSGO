#pragma once

#include "SDK.h"
#include "Render.h"
#include "AutoWall.h"

class SpreadCrosshair
{
public:
	Vector points[256];
	Vector center;
	FireBulletData fbd;

	SpreadCrosshair();

	void Tick(QAngle angles);
	void Draw();
};

extern SpreadCrosshair* spreadCrosshair;
