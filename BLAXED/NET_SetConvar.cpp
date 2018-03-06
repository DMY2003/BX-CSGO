#include "SDK.h"
#include "NET_SetConvar.h"

#pragma push_macro("and")
#undef and

static __declspec(naked) void __cdecl Invoke_NET_SetConVar(void* pfn, const char *cvar, const char *value)
{
	__asm 
	{
		push    ebp
		mov     ebp, esp
		and     esp, 0FFFFFFF8h
		sub     esp, 44h
		push    ebx
		push    esi
		push    edi
		mov     edi, cvar
		mov     esi, value
		jmp     pfn
	}
}

#pragma pop_macro("and")

void NET_SetConVar(const char* name, const char* value)
{
	//__asm pushad

	static void* pvSetConVar = NULL;
	
	if (!pvSetConVar)
		pvSetConVar = (void*)Mem::FindPattern(xorstr("engine.dll"), xorstr("8D 4C 24 1C E8 ? ? ? ? 56"));

	if (pvSetConVar)
		Invoke_NET_SetConVar(pvSetConVar, name, value);

	//__asm popad
}

void SetName(const char *name)
{
	/*
	ConVar* name = I::pCVar->FindVar(xorstr("name"));
	DWORD old = *(DWORD*)((DWORD)&name->m_fnChangeCallbacks + 0xC);
	*(DWORD*)((DWORD)&name->m_fnChangeCallbacks + 0xC) = NULL;
	name->SetValue(buf);
	*/

	ConVar* cname = I::pCVar->FindVar(xorstr("name"));

	if (name)
	{
		//*(DWORD*)((DWORD)&cname->m_fnChangeCallbacks + 0xC) = NULL;
		cname->m_fnChangeCallbacks.RemoveAll();
		NET_SetConVar(xorstr("name"), name);
		cname->SetValue(name);
		//*(DWORD*)((DWORD)&cname->m_fnChangeCallbacks + 0xC) = NULL;
	}
}

void SetClanTag(const char *tag)
{
	typedef int(__fastcall *SendClanTagFn)(const char*, const char*);
	static SendClanTagFn SendClanTag = NULL;

	if (!SendClanTag)
		SendClanTag = (SendClanTagFn)Mem::FindPattern(xorstr("engine.dll"), xorstr("53 56 57 8B DA 8B F9 FF 15 ?? ?? ?? ??"));

	if (SendClanTag)
	{
		//__asm pushad
		SendClanTag(tag, "");
		//__asm popad
	}
}
