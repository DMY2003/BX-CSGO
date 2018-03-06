#include "SDK.h"
#include "Global.h"
#include "Configs.h"
#include "FireEventClientSide.h"
#include "Hooks.h"

//http://www.unknowncheats.me/forum/counterstrike-global-offensive/188593-simple-hook-igameevent-catching.html
//http://www.unknowncheats.me/forum/counterstrike-global-offensive/150897-using-game-events.html

FireEvent_t oFireEvent = NULL;
FireEventClientSide_t oFireEventClientSide = NULL;

bool __fastcall FireEvent(void* ths, void* edx, void** evnt, bool bDontBroadcast)
{	
	// not working
	__asm pushad

	IGameEvent* e = (IGameEvent*)evnt;
		
	if (e != NULL)
	{
		printf("FE %s\n", e->GetName());
	}

	__asm popad

	return oFireEvent(ths, evnt, bDontBroadcast);
}


bool __fastcall FireEventClientSide(void* ths, void* edx, void** evnt)
{
	// not working good

	if (evnt != NULL)
	{
		IGameEvent* e = (IGameEvent*)evnt;		
	}

	return oFireEventClientSide(ths, evnt);
}
