#pragma once

class IPrediction
{
public:
	bool InPrediction()
	{
		typedef bool(__thiscall* oInPrediction)(void*);
		return Mem::CallVirtualFunction<oInPrediction>(this, 14)(this);
	}

	void RunCommand(CBaseEntity *player, CUserCmd *ucmd, IMoveHelper *moveHelper)
	{
		typedef void(__thiscall* oRunCommand)(void*, CBaseEntity*, CUserCmd*, IMoveHelper*);
		return  Mem::CallVirtualFunction<oRunCommand>(this, 19)(this, player, ucmd, moveHelper);
	}

	void SetupMove(CBaseEntity *player, CUserCmd *ucmd, IMoveHelper *moveHelper, void* pMoveData)
	{
		typedef void(__thiscall* oSetupMove)(void*, CBaseEntity*, CUserCmd*, IMoveHelper*, void*);
		return  Mem::CallVirtualFunction<oSetupMove>(this, 20)(this, player, ucmd, moveHelper, pMoveData);
	}

	void FinishMove(CBaseEntity *player, CUserCmd *ucmd, void*pMoveData)
	{
		typedef void(__thiscall* oFinishMove)(void*, CBaseEntity*, CUserCmd*, void*);
		return  Mem::CallVirtualFunction<oFinishMove>(this, 21)(this, player, ucmd, pMoveData);
	}
};

