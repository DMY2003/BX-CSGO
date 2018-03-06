#include "SDK.h"
#include "ConvarSpoofer.h"

namespace ConvarSpoofer
{
	bool m_bReady = false;
	std::map<std::string, SpoofedConvar* > m_pSpoofeds;

	void Init()
	{
		m_pSpoofeds.clear();
		m_bReady = false;
	}

	void Release()
	{
		m_bReady = false;

		for (auto it = m_pSpoofeds.begin(); it != m_pSpoofeds.end(); it++)
		{
			delete it->second;
		}

		m_pSpoofeds.clear();
	}

	void Add(const char *name)
	{
		m_pSpoofeds.insert(std::make_pair(name, nullptr));
	}

	void Spoof()
	{
		for (auto it = m_pSpoofeds.begin(); it != m_pSpoofeds.end(); it++)
		{
			it->second = new SpoofedConvar(it->first.c_str());
		}

		m_bReady = true;
	}

	SpoofedConvar *Get(const char *name)
	{
		auto find = m_pSpoofeds.find(name);

		if (find != m_pSpoofeds.end())
			return find->second;

		return NULL;
	}

	bool IsReady()
	{
		return m_bReady;
	}
};
