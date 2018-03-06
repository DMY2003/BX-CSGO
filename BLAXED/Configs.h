#pragma once

#include <map>
#include "json.hpp"

#include "XorStr.hpp"

#include "Color.h"

using json = nlohmann::json;

class ConfigItem
{
public:
	std::string name;
	void *pointer;
	std::string type;

	ConfigItem(std::string name, void *pointer, std::string type)
	{
		this->name = name;
		this->pointer = pointer;
		this->type = type;
	}
};

class ConfigManager
{
public:
	ConfigManager()
	{
		actualConfig = "Default";
		configs.clear();
	}

	std::vector<ConfigItem> configs;
	std::string actualConfig;

	ConfigItem *FindItem(std::string name);

	void SetupConfigs();

	void AddConfig(void *pointer, const char *name, const char *category, std::string type);
	void DeclareConfig(int *pointer, int value, const char *name, const char *category);
	void DeclareConfig(float *pointer, float value, const char *name, const char *category);
	void DeclareConfig(bool *pointer, bool value, const char *name, const char *category);
	void DeclareConfig(std::vector<int> *pointer, int size, const char *name, const char *category);
	void DeclareConfig(std::vector<float> *pointer, int size, const char *name, const char *category);
	void DeclareConfig(Color *pointer, Color value, const char *name, const char *category);

	static std::string GetConfigPath();
	std::vector<std::string> files;

	void GetConfigFiles();
	void WriteConfig(std::string fileName);
	void ReadConfig(std::string fileName);
	void RemoveConfig(std::string fileName);
};

enum HITSCAN_TYPE : int
{
	HITSCAN_NONE,
	HITSCAN_NORMAL,
	HITSCAN_HIGH_DAMAGE,
	HITSCAN_PRIORIZE_FREE_PATH,
};

enum MULTIBOX_TYPE : int
{
	MULTIBOX_NONE,
	MULTIBOX_HEAD_NECK,
	MULTIBOX_HEAD_CHEST,
	MULTIBOX_HEAD_NECK_CHEST,	
};

class Configs
{
public:

	bool showMenu;
	int language;

	struct Overlay_t
	{
		bool radar;
		bool spectators;
	}Overlay;

	struct Aimbot_t
	{
		bool enabled;
		float fov;
		bool onlyX;

		struct Smooth_t
		{
			bool enabled;
			float ammount;
		}Smooth;
		
		bool silent;
		int recoverTime;

		struct Aimstep_t
		{
			bool enabled;
			float ammount;
		}Aimstep;

		int position;

		bool friendlyFire;
	}Aimbot;

	struct Rage_t
	{
		struct AutoWall_t
		{
			bool enabled;
			float minDamage;
			bool fast;			
		}AutoWall;

		struct AutoFire_t
		{
			bool enabled;
			bool autoStop;
			bool autocrouch;
			float maxSpread;
			bool useMaxSpread;
			float maxSpeed;
			bool useMaxSpeed;
			float hitchance;
			int hitchanceMaxSeed;
			bool hitchanceBasedAimPos;
			bool inGround;
			bool AutoBaim;
			bool autoScope;
			int BaimAfterShots;
			bool prioritize;
			int hitscan;
			int multiBox;
			bool onlyHS;
			bool onlyHsMoving;
			float pointScale;
			bool autoRevolver;
			bool smartAim;
			bool preferBody;
			bool accuracyBoost;
			bool multiPoint;
			bool baimOnLowHitchance;
			bool delayShoot;
			bool antiSilent;
			bool onlyHsIfResolved;
			bool rage_multiHitboxes[14] =
			{
				true, true, true, true, true,
				true, true, true, true, true,
				true, true, true, true
			};
		}AutoFire;

		struct Hitscan_t
		{
			std::vector<float> scale;
			std::vector<int> points;
			std::vector<int> ignore;
			std::vector<int> priority;
		}Hitscan;

		struct AntiAim_t
		{
			bool enabled;
			bool viewAntiAim;
			bool viewAntiAimFake;
			bool viewLowerBodyYawTarget;
			int realStyle;
			int fakeStyle;
			int lowerBodyStyle;
			int pitchStyle;
			bool staticAA;			
			bool untrusted;
			bool atTargets;
			bool lowerBody;
			bool removeOnKnife;
			float realYawOffset;
			float fakeYawOffset;
			int movingRealStyle;
			int duckingRealStyle;
			float lbyJitter;
			float lbyJitterMoving;
			bool lbyJitterOnlySlow;
			bool realJitter;
			bool fakeJitter;
			int slowMotionTicks;
			float fakelLbyOffset;
			float customPitch;
			
		}AntiAim;

		struct Resolver_t
		{
			bool enabled;
			bool prediction;
			bool fixPitch;
			bool forceYaw;
			bool lispCorrect;
			bool deltaCorrect;
			bool fireOnlyResolved;
			int resetTime;
			bool noInterpolation;
			bool lagCompensation;
			bool baimUnresolved;
			bool backTrack;
			bool extrapolate;
			bool forcedBackTrack;
			bool drawRecord;
			int backTrackOffset;
			int backTrackMinOffset;
			int BruteForceShots;
			int backtrackingtype;
			bool forceJitter;
			int minBruteforceBullets;
			bool inverseUnresolved;
			bool pingExploit;
			bool noSpreadMode;
			bool resolveLegits;
		}Resolver;

		struct FakeLag_t
		{
			bool enabled;
			bool adaptive;
			bool random;
			int max;
			int min;			
			int air;
			int edge;
		}FakeLag;

		bool noRecoil;
		bool pSilent;
		bool ignoreHead;
		bool ignoreArm;
		bool teleportF9;
	}Rage;

	struct Visual
	{
		bool enabled;

		bool dinamicFOV;
		bool drawFOV;
		bool drawCrosshair;
		bool drawTracers;
		bool drawNames;
		bool drawWeapons;
		bool drawHealth;
		bool drawBox;
		bool drawBones;
		bool drawItems;
		bool drawItemsNames;
		int drawWeaponIcons;
		bool hitmarkersound;
		bool chams;
		bool wallChams;
		bool handChams;
		bool hvhChams;
		int espAlpha;		
		bool noScope;
		bool screenshotCleaner;
		bool nadeTraces;
		bool nadePredict;
		bool showTeam;
		bool drawAmmo;
		bool boxFill;
		bool glassHands;
		bool granadePrediction;
		bool disablePostProcess;
		bool disableRagdollPhysics;
		bool dormantEsp;
		bool nightMode;
		bool localPlayerVisuals;
		bool darkHandChams;
		bool debugAutowall;
		bool spreadCrosshair;
		bool flashLight;
		bool noFlash;
		bool glow;
		bool showAimPoint;
		bool fpsBoost;
		bool debugMultipoint;
		bool lockFov;
		bool lagometer;
		bool drawHealthDivision;
		bool damageESP;

		Color colorGlow;
		Color colorChamsVisible;
		Color colorChamsHidden;
	}Visual;

	struct Misc_t
	{
		bool bunnyhop;
		bool autoStrafe;
		bool fastAutoStrafe;
		bool bunnyCircle;
		bool bunnyAutoDuck;
		int clanTagAnimation;
		bool autoBlock;
		bool thirdPerson;
		float fovChanger;
		bool nameStealer;
		bool noSmoke;
		bool bypassSvCheats;
		float retrackSpeed;
		bool buunyhopStopAtWall;
		bool autoaccept;
		float speedDivisor;
		bool boxCircleStrafe;
		bool spoofAutoBunnyhop;
		bool unlockFps;
		bool airStuck;
		bool lobbyChaos;
		int lagExploit;
	}Misc;



	struct LegitBot_t
	{
		bool enabled;

		bool aimbot;

		float fov;
		float smooth;
		
		bool backtrack;

		bool triggerBot;
		float triggerDelay;

		bool friendlyFire;
		bool silent;

		float delay;
		
		bool aimLock;
	}LegitBot;
};

extern Configs cfg;
extern ConfigManager *cfgMgr;

