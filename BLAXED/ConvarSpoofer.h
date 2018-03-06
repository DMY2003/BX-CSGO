#pragma once

#include <map>

#include "SDK.h"

namespace ConvarSpoofer
{
	extern bool m_bReady;
	extern std::map<std::string, SpoofedConvar* > m_pSpoofeds;

	void Init();
	void Release();

	void Add(const char *name);
	void Spoof();
	SpoofedConvar *Get(const char *name);
	bool IsReady();
};
