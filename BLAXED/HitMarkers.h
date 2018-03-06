#pragma once

class HitMarker
{
public:
	Vector from, to;
	Vector forward;
	float startTime;
	Color color;
	bool show;
	int type;
};

class HitMarkers
{
public:
	static const int MAX_MARKERS = 300;
	static const int CLEAR_TIME = 4;

	unsigned int markerIdx;
	HitMarker markers[MAX_MARKERS];

	float lastKillTime;
	float lastDamage;

	HitMarkers();

	void HandleGameEvent(IGameEvent* event);

	void Clear();
	void RemoveOlder(float time);
	void Paint();
	void Draw();

	int Add(Vector from, Vector to, Vector forward, int type);
	void SetColor(int idx, Color color);
};

extern HitMarkers *hitMarkers;
