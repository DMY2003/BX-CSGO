#pragma once

class HUDIndicators
{
public:

	bool lowerBodyChoke;
	bool lagCompensated;

	HUDIndicators();

	void Paint();
};

extern HUDIndicators *hudIndicators;
