#pragma once


namespace ESP
{
	void Render();
	void DrawAimbotFOV(float FOV, bool dinamic, D3DCOLOR color);
	void DrawCrossHair(float size, D3DCOLOR color);
	void DrawTracer(CBaseEntity *entity);
	void DrawName(CBaseEntity *entity);
	void DrawWeapon(CBaseEntity *entity);
	void DrawHealth(CBaseEntity *entity);
	void DrawBox(CBaseEntity *entity);
	void DrawBoneESP(CBaseEntity *entity);
	void DrawBone(CBaseEntity *entity, matrix3x4_t *pBoneToWorldOut, D3DCOLOR color);
	void DrawItem(CBaseEntity *entity);
	bool GetPlayerBox(CBaseEntity *entity, float &x, float &y, float &width, float &height, Vector offset = Vector(0, 0, 0));
	bool GetItemBox(CBaseEntity *entity, float &x, float &y, float &width, float &height, Vector offset = Vector(0, 0, 0));
	bool Filter(CBaseEntity *entity);
	void PunchAnglesToScreen(int height, int width, int FOV, int *resultX, int *resultY);
	bool CanShowDormant(CBaseEntity *entity);
	int GetDormantAlpha(CBaseEntity *entity);

	extern bool canUseSetupBones;
};
