#include "SDK.h"

class IPanel
{
public:
	const char* GetName(int iIndex)
	{
		typedef const char*(__thiscall* OriginalFn)(void*, int);
		return Mem::CallVirtualFunction<OriginalFn>(this, 36)(this, iIndex);
	}
};
