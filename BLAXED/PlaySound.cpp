#include "SDK.h"
#include "PlaySound.h"
#include "Global.h"

PlaySoundFn oPlaySound;

void __fastcall HookedPlaySound(void* ecx, void* edx, const char* filename)
{
	if (G::exit)
		return oPlaySound(ecx, filename);

	oPlaySound(ecx, filename);

	if (cfg.Misc.autoaccept)
	{
		typedef void(__cdecl* IsReadyFn)();
		static IsReadyFn IsReady = NULL;
		
		if (!IsReady)
		{
			//char *isreadysig = "81  66  70  00  00  F0  FF  C7  46  6C  00  00  00  00  C7  46  74  00  00  00  00  5E  E9  ??  ??  ??  ??  CC  CC  CC  CC  CC  CC  CC  CC  CC  CC  CC  CC  CC  55  8B  EC  83  E4  F8  83  EC  08  56  8B  35  ??  ??  ??  ??  57";

			std::string sig = "";			
			sig += xorstr("81  66  70  00  00  F0  FF  C7  46  6C  00  00  00  00  ");
			sig += xorstr("C7  46  74  00  00  00  00  5E  E9 ? ? ? ? ? ? ? ? CC  CC  CC  ");
			sig += xorstr("CC  CC  CC  CC  CC  CC  CC  CC  CC  CC  55  8B  EC  83  E4  F8  ");
			sig += xorstr("83  EC  08  56  8B  35 ? ? ? ? ? ? ? ? 57");

			IsReady = (IsReadyFn)(Mem::FindMemoryPattern(GetModuleHandle(xorstr("client.dll")), (char*)sig.c_str(), 74) + 0x28);
		}

		if (!Interfaces::pEngineClient->IsInGame() && !strcmp(filename, "!UI/competitive_accept_beep.wav"))
		{
			IsReady();
		}
	}
}
