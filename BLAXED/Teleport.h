#pragma once

class Teleport
{
	bool teleport;
	int lastTeleport;

public:
	Teleport()
	{
		teleport = false;
		lastTeleport = 0;
	}

	void Tick(bool chokePacket)
	{
		if (!cfg.Rage.AntiAim.untrusted)
			return;

		if (!cfg.Rage.teleportF9)
			return;

		if (G::pLocal->GetHealth() <= 0.0f || G::pLocal->GetLifeState() != LIFE_ALIVE)
			return;

		if (GetTickCount() - lastTeleport < 350)
			return;

		if (G::pressedKeys[VK_F9] & 1)
		{				
			teleport = true;
		}		

		if (chokePacket)
		{
			if (teleport)
			{
				lastTeleport = GetTickCount();
				teleport = false;

				//pCmd->viewangles.z = (float)0x7FFFFFFE;
				//pCmd->viewangles.y = 22249223532141231232142.949962143f;
				//pCmd->viewangles.x = 22249223523413423454322344.9499621213f;

				G::pCmd->viewangles.z = 666000000000666000000000666.666000666000f;
				//pCmd->viewangles.y = 666000000000666000000000666.666000666000f;
				//pCmd->viewangles.x = 666000000000666000000000666.666000666000f;
			}
		}
	}
};

extern Teleport *teleport;
