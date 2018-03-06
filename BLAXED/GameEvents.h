#pragma once

#define EVENT_DEBUG_ID_INIT 42 
#define EVENT_DEBUG_ID_SHUTDOWN 13 

class GameEvents : public IGameEventListener2
{
public:
	GameEvents();

	void Register();
	void Unregister();
	void HandleGameEvent(IGameEvent* event);
	int IndicateEventHandling();
};

extern GameEvents *gameEvents;
