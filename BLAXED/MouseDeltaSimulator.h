#pragma once

class MouseDeltaSimulator
{
public:
	void Tick(bool chokePacket)
	{
		CUserCmd *pCmd = G::pCmd;

		if (!chokePacket && (aimbotV2->usingTick || antiAim->usingTick))
		{
			static QAngle lastAngles = QAngle(0, 0, 0);

			static float m_yaw = 0.022f;
			static float m_pitch = 0.022f;

			QAngle delta = pCmd->viewangles - lastAngles;
			delta.Clamp();

			short mousedx = (short)(-(delta.y / m_yaw));
			short mousedy = (short)((delta.x / m_pitch));

			//printf("%d %d | %d %d\n", pCmd->mousedx, pCmd->mousedy, mousedx, mousedy);

			pCmd->mousedx = mousedx;
			pCmd->mousedy = mousedy;

			lastAngles = pCmd->viewangles;
		}
	}
};

extern MouseDeltaSimulator *mouseDeltaSimulator;
