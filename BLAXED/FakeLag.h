#pragma once

#include "SDK.h"
#include "Global.h"
#include "Configs.h"

const int MAX_CHOKE = 15;
const float TELEPORT_DISTANCE = 64.f;

class FakeLag
{
public:

	bool isLagCompensated;

	int choked;

	FakeLag();	

	void Tick(bool &chokePacket);
	void EndTick(bool chokePacket);
};

extern FakeLag *fakeLag;

/*
static constexpr int MAX_CHOKE = 15;
static constexpr float TELEPORT_DISTANCE 64.f;

int Fakelag_AdaptiveFactor() {
auto local_player = Features::GetLocalPlayer();
auto velocity = local_player->GetVelocity();
velocity.z = 0.f;
auto speed = velocity.Length();
auto distance_per_tick = speed *
Interfaces->GetGlobalVars()->interval_per_tick;
int choked_ticks = std::ceilf(TELEPORT_DISTANCE / distance_per_tick);
return std::min<int>(choked_ticks, MAX_CHOKE);

}
*/
