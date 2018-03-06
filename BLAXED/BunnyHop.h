#pragma once

void RotateMovement(CUserCmd* pCmd, float rotation);

class BunnyHop
{
public:
	QAngle oldAngles;

	void Tick(bool chokePacket);

	BunnyHop();

	// debug
	QAngle circleStrafeDbg;
	Vector d_col;
	Vector d_colL;
	Vector d_colR;

	bool usingTick;
};

extern BunnyHop *bunnyHop;
