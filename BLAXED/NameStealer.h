#pragma once


class NameStealer
{
public:
	int delay;
	int idx;
	bool started;

	NameStealer();
	void Tick();
	
	char bkpName[128];
};

extern NameStealer *nameStealer;
