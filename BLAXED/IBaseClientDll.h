#pragma once

class IBaseClientDll
{
public:
	ClientClass* GetAllClasses()
	{
		typedef ClientClass*(__thiscall* OriginalFn)(PVOID);
		return Mem::CallVirtualFunction<OriginalFn>(this, 8)(this);
	}
};
