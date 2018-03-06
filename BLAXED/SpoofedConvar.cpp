#include "SDK.h"
#include "SpoofedConvar.hpp"

using namespace SourceEngine;

SpoofedConvar::SpoofedConvar(const char* szCVar) {
	m_pOriginalCVar = I::pCVar->FindVar(szCVar);
	Spoof();
}
SpoofedConvar::SpoofedConvar(SourceEngine::ConVar* pCVar) {
	m_pOriginalCVar = pCVar;
	Spoof();
}
SpoofedConvar::~SpoofedConvar() {
	if (IsSpoofed()) {
		//Restore flags
		SetFlags(m_iOriginalFlags);

		//Restore value
		SetString(m_szOriginalValue);

		//Restore the name
		DWORD dwOldProtect;
		VirtualProtect((LPVOID)m_pOriginalCVar->GetName(), 128, PAGE_READWRITE, &dwOldProtect);
		strcpy((char*)m_pOriginalCVar->GetName(), m_szOriginalName);
		VirtualProtect((LPVOID)m_pOriginalCVar->GetName(), 128, dwOldProtect, NULL);

		//Unregister dummy cvar
		I::pCVar->UnregisterConCommand(m_pDummyCVar);
		free(m_pDummyCVar);
		m_pDummyCVar = nullptr;

		I::pCVar->UnregisterConCommand(m_pFakeCVar);
		delete m_pFakeCVar;
		m_pFakeCVar = nullptr;
	}
}
bool SpoofedConvar::IsSpoofed() {
	return m_pDummyCVar != nullptr;
}
void SpoofedConvar::Spoof() {
	if (!IsSpoofed() && m_pOriginalCVar) {
		//Save old name value and flags so we can restore the cvar lates if needed
		m_iOriginalFlags = m_pOriginalCVar->m_nFlags;
		strcpy_s(m_szOriginalName, 128, m_pOriginalCVar->GetName());
		strcpy_s(m_szOriginalValue, 128, m_pOriginalCVar->GetString());

		if (!strcmp(m_szOriginalName, xorstr("sv_cheats")))
			strcpy_s(m_szOriginalValue, 128, m_pOriginalCVar->GetDefault());

		sprintf_s(m_szDummyName, 128, "%s%s", xorstr("ldc_"), m_szOriginalName);

		//Create the dummy cvar
		m_pDummyCVar = (SourceEngine::ConVar*)malloc(sizeof(SourceEngine::ConVar));
		memcpy(m_pDummyCVar, m_pOriginalCVar, sizeof(SourceEngine::ConVar));

		m_pDummyCVar->m_pNext = nullptr;

		//Register it
		I::pCVar->RegisterConCommand(m_pDummyCVar);
		
		//Rename the cvar
		//Fix "write access violation" bullshit
		DWORD dwOldProtect;
		VirtualProtect((LPVOID)m_pOriginalCVar->GetName(), 128, PAGE_READWRITE, &dwOldProtect);		
		strcpy((char*)m_pOriginalCVar->GetName(), m_szDummyName);
		VirtualProtect((LPVOID)m_pOriginalCVar->m_pszName, 128, dwOldProtect, NULL);

		//Create a cvar with the original name
		m_pFakeCVar = new SourceEngine::ConVar(m_szOriginalName, m_szOriginalValue, m_iOriginalFlags, xorstr("cvar"));
		I::pCVar->RegisterConCommand(m_pFakeCVar);

		m_pFakeCVar->m_nFlags = m_pOriginalCVar->m_nFlags;

		SetFlags(FCVAR_NONE);
	}
}
void SpoofedConvar::SetFlags(int flags) {
	if (IsSpoofed()) {
		m_pOriginalCVar->m_nFlags = flags;
	}
}
int SpoofedConvar::GetFlags() {
	if (IsSpoofed()) {
		return m_pOriginalCVar->GetFlags();
	}

	return -1;
}
void SpoofedConvar::SetValue(int iValue) {
	if (IsSpoofed()) {
		m_pOriginalCVar->SetValue(iValue);
	}
}
void SpoofedConvar::SetFloat(float flValue) {
	if (IsSpoofed()) {
		m_pOriginalCVar->SetValue(flValue);
	}
}
void SpoofedConvar::SetString(const char* szValue) {
	if (IsSpoofed()) {
		m_pOriginalCVar->SetValue(szValue);
	}
}
int SpoofedConvar::GetInt() {
	if (IsSpoofed()) {
		return m_pOriginalCVar->GetInt();
	}

	return -1;
}
float SpoofedConvar::GetFloat() {
	if (IsSpoofed()) {
		return m_pOriginalCVar->GetFloat();
	}

	return -1;
}
const char* SpoofedConvar::GetString() {
	if (IsSpoofed()) {
		return m_pOriginalCVar->GetString();
	}

	return 0;
}