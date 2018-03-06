#pragma once

#include "ConVar.h"

class SpoofedConvar {
public:
	SpoofedConvar(const char* szCVar);
	SpoofedConvar(SourceEngine::ConVar* pCVar);

	~SpoofedConvar();

	bool		IsSpoofed();
	void		Spoof();

	void		SetFlags(int flags);
	int			GetFlags();

	void		SetValue(int iValue);
	void		SetFloat(float flValue);
	void		SetString(const char* szValue);

	int			GetInt();
	float		GetFloat();
	const char* GetString();

private:
	SourceEngine::ConVar* m_pOriginalCVar = nullptr;
	SourceEngine::ConVar* m_pDummyCVar = nullptr;
	SourceEngine::ConVar* m_pFakeCVar = nullptr;

	char m_szDummyName[128];
	char m_szDummyValue[128];
	char m_szOriginalName[128];
	char m_szOriginalValue[128];
	int m_iOriginalFlags;
};
