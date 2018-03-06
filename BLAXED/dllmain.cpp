#include "SDK.h"
#include "MainThread.h"
#include "Global.h"
#include "Protection.h"
#pragma comment(lib,"d3d9.lib") 
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"Gdi32.lib")

DWORD WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		::DisableThreadLibraryCalls(hModule);
	
		Global::exit = false;	
		Global::module = hModule;
		
		Global::mainThread = (HANDLE)CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&MainThread, hModule, 0, NULL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		Global::exit = true;		
	}
	else if (dwReason == DLL_THREAD_ATTACH)
	{

	}
	else if (dwReason == DLL_THREAD_DETACH)
	{

	}

	return 1;
}
