#include "SDK.h"
#include "Global.h"
#include "Hooks.h"
#include "IsPlayingTimeDemo.h"
#include "ConvarSpoofer.h"
#include "ResolverV2.h"
#include <stdio.h>  
#include <intrin.h>  

#pragma intrinsic(_ReturnAddress)  
// https://www.unknowncheats.me/forum/counterstrike-global-offensive/197689-kids-using-interplist.html

IsPlayingTimeDemo_t oIsPlayingTimeDemo = NULL;

bool __fastcall IsPlayingTimeDemo(void *this_, void *edx)
{
	if (G::exit || !ConvarSpoofer::IsReady())
		return  oIsPlayingTimeDemo(this_);

	bool ret = oIsPlayingTimeDemo(this_);

	static DWORD returnaddr = NULL;

	if (!returnaddr)
		returnaddr = (DWORD)Mem::FindPattern(xorstr("client.dll"), xorstr("84 C0 75 14 8B 0D ? ? ? ? 8B 01 8B 80 ? ? ? ? FF D0 84 C0 74 07"));

	//.text:101E2957		mov     s_bInterpolate, bl
	//.text:101E295D		mov     eax, [ecx]
	//.text:101E295F		mov     eax, [eax+150h] ; IsPlayingTimeDemo (engine, 84)
	//.text:101E2965		call    eax
	//.text:101E2967		test    al, al

	if (cfg.Rage.Resolver.noInterpolation)
	{
		if (_ReturnAddress() == (void*)returnaddr)
		{
			// return true to disable interpolation
			return true;
		}
	}

	return ret;
}
