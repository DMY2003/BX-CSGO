#include <iostream>
#include <fstream>

#include "SDK.h"
#include "Configs.h"
#include "licence.h"

Configs cfg;

void ConfigManager::SetupConfigs()
{
	DeclareConfig(&cfg.Visual.screenshotCleaner, false, xorstr("cfg.Visual.screenshotCleaner"), xorstr("Visual"));
				  
	DeclareConfig(&cfg.showMenu, true, xorstr("cfg.showMenu"), xorstr("Unknown"));
	DeclareConfig(&cfg.language, 0, xorstr("cfg.language"), xorstr("Unknown"));

	DeclareConfig(&cfg.Overlay.radar, false, xorstr("cfg.Overlay.radar"), xorstr("Overlay"));
	DeclareConfig(&cfg.Overlay.spectators, false, xorstr("cfg.Overlay.spectators"), xorstr("Overlay"));
				  
	DeclareConfig(&cfg.Aimbot.enabled, false, xorstr("cfg.Aimbot.enabled"), xorstr("Aimbot"));
	DeclareConfig(&cfg.Aimbot.fov, 1.5f, xorstr("cfg.Aimbot.fov"), xorstr("Aimbot"));
	DeclareConfig(&cfg.Aimbot.onlyX, false, xorstr("cfg.Aimbot.onlyX"), xorstr("Aimbot"));
	DeclareConfig(&cfg.Aimbot.Smooth.enabled, false, xorstr("cfg.Aimbot.Smooth.enabled"), xorstr("Aimbot"));
	DeclareConfig(&cfg.Aimbot.Smooth.ammount, 1.0f, xorstr("cfg.Aimbot.Smooth.ammount"), xorstr("Aimbot"));
	DeclareConfig(&cfg.Aimbot.silent, false, xorstr("cfg.Aimbot.silent"), xorstr("Aimbot"));
	DeclareConfig(&cfg.Aimbot.recoverTime, (int)500, xorstr("cfg.Aimbot.recoverTime"), xorstr("Aimbot"));
	DeclareConfig(&cfg.Aimbot.Aimstep.enabled, false, xorstr("cfg.Aimbot.Aimstep.enabled"), xorstr("Aimbot"));
	DeclareConfig(&cfg.Aimbot.Aimstep.ammount, 30.0f, xorstr("cfg.Aimbot.Aimstep.ammount"), xorstr("Aimbot"));	
	DeclareConfig(&cfg.Aimbot.position, 1, xorstr("cfg.Aimbot.hitscan"), xorstr("Aimbot"));
	DeclareConfig(&cfg.Rage.Resolver.backtrackingtype, 1, xorstr("cfg.Rage.Resolver.backtrackingtype"), xorstr("Rage"));
	DeclareConfig(&cfg.Aimbot.friendlyFire, false, xorstr("cfg.Aimbot.friendlyFire"), xorstr("Aimbot"));
	DeclareConfig(&cfg.Rage.AutoWall.enabled, false, xorstr("cfg.Rage.AutoWall.enabled"), xorstr("Rage"));	
	DeclareConfig(&cfg.Rage.AutoWall.minDamage, 1.0f, xorstr("cfg.Rage.AutoWall.minDamage"), xorstr("Rage"));
	DeclareConfig(&cfg.Rage.AutoWall.fast, false, xorstr("cfg.Rage.AutoWall.fast"), xorstr("Rage"));
	DeclareConfig(&cfg.Rage.AntiAim.enabled, false, xorstr("cfg.Rage.AntiAim.enabled"), xorstr("Rage"));
	DeclareConfig(&cfg.Rage.AntiAim.viewAntiAim, false, xorstr("cfg.Rage.AntiAim.viewAntiAim"), xorstr("Rage"));
	DeclareConfig(&cfg.Rage.AntiAim.viewAntiAimFake, false, xorstr("cfg.Rage.AntiAim.viewAntiAimFake"), xorstr("Rage"));
	DeclareConfig(&cfg.Rage.AntiAim.viewLowerBodyYawTarget, false, xorstr("cfg.Rage.AntiAim.viewLowerBodyYawTarget"), xorstr("Rage"));
	DeclareConfig(&cfg.Rage.AntiAim.realStyle, 0, xorstr("cfg.Rage.AntiAim.realStyle"), xorstr("Rage"));
	DeclareConfig(&cfg.Rage.AntiAim.fakeStyle, 0, xorstr("cfg.Rage.AntiAim.fakeStyle"), xorstr("Rage"));
	DeclareConfig(&cfg.Rage.AntiAim.lowerBodyStyle, 0, xorstr("cfg.Rage.AntiAim.lowerBodyStyle"), xorstr("Rage"));
	DeclareConfig(&cfg.Rage.AntiAim.pitchStyle, 0, xorstr("cfg.Rage.AntiAim.pitchStyle"), xorstr("Rage"));
	DeclareConfig(&cfg.Rage.AntiAim.staticAA, false, xorstr("cfg.Rage.AntiAim.staticAA"), xorstr("Rage"));	
	DeclareConfig(&cfg.Rage.AntiAim.atTargets, false, xorstr("cfg.Rage.AntiAim.atTargets"), xorstr("Rage"));
	DeclareConfig(&cfg.Rage.AntiAim.untrusted, true, xorstr("cfg.Rage.AntiAim.untrusted"), xorstr("Rage"));
	DeclareConfig(&cfg.Rage.AntiAim.lowerBody, false, xorstr("cfg.Rage.AntiAim.lowerBody"), xorstr("Rage"));
	DeclareConfig(&cfg.Rage.AntiAim.removeOnKnife, false, xorstr("cfg.Rage.AntiAim.removeOnKnife"), xorstr("Rage"));
	DeclareConfig(&cfg.Rage.AntiAim.fakeYawOffset, 0.0f, xorstr("cfg.Rage.AntiAim.fakeYawOffset"), xorstr("Rage"));
	DeclareConfig(&cfg.Rage.AntiAim.realYawOffset, 0.0f, xorstr("cfg.Rage.AntiAim.realYawOffset"), xorstr("Rage"));
	DeclareConfig(&cfg.Rage.AntiAim.movingRealStyle, 0, xorstr("cfg.Rage.AntiAim.movingRealStyle"), xorstr("Rage"));
	DeclareConfig(&cfg.Rage.AntiAim.duckingRealStyle, 0, xorstr("cfg.Rage.AntiAim.duckingRealStyle"), xorstr("Rage"));
	DeclareConfig(&cfg.Rage.AntiAim.lbyJitter, 0.0f, xorstr("cfg.Rage.AntiAim.lbyJitter"), xorstr("Rage"));
	DeclareConfig(&cfg.Rage.AntiAim.lbyJitterMoving, 0.0f, xorstr("cfg.Rage.AntiAim.lbyJitterMoving"), xorstr("Rage"));
	DeclareConfig(&cfg.Rage.AntiAim.lbyJitterOnlySlow, false, xorstr("cfg.Rage.AntiAim.lbyJitterOnlySlow"), xorstr("Rage"));
	DeclareConfig(&cfg.Rage.AntiAim.realJitter, false, xorstr("cfg.Rage.AntiAim.realJitter"), xorstr("Rage"));
	DeclareConfig(&cfg.Rage.AntiAim.fakeJitter, false, xorstr("cfg.Rage.AntiAim.fakeJitter"), xorstr("Rage"));
	DeclareConfig(&cfg.Rage.AntiAim.slowMotionTicks, false, xorstr("cfg.Rage.AntiAim.slowMotionTicks"), xorstr("Rage"));
	DeclareConfig(&cfg.Rage.AntiAim.fakelLbyOffset, 0.0f, xorstr("cfg.Rage.AntiAim.fakelLbyOffset"), xorstr("Rage"));
	DeclareConfig(&cfg.Rage.AntiAim.customPitch, 0.0f, xorstr("cfg.Rage.AntiAim.customPitch"), xorstr("Rage"));
		
	DeclareConfig(&cfg.Rage.AutoFire.enabled, false, xorstr("cfg.Rage.AutoFire.enabled"), xorstr("Rage"));
	DeclareConfig(&cfg.Rage.AutoFire.hitchance, 0.0f, xorstr("cfg.Rage.AutoFire.hitchance"), xorstr("Rage"));
	DeclareConfig(&cfg.Rage.AutoFire.hitchanceMaxSeed, 256, xorstr("cfg.Rage.AutoFire.hitchanceMaxSeed"), xorstr("Rage"));
	DeclareConfig(&cfg.Rage.AutoFire.hitchanceBasedAimPos, false, xorstr("cfg.Rage.AutoFire.hitchanceBasedAimPos"), xorstr("Rage"));
	DeclareConfig(&cfg.Rage.AutoFire.autoStop, false, xorstr("cfg.Rage.AutoFire.autoStop"), xorstr("Rage"));
	DeclareConfig(&cfg.Rage.AutoFire.useMaxSpread, false, xorstr("cfg.Rage.AutoFire.useMaxSpread"), xorstr("Rage"));
	DeclareConfig(&cfg.Rage.AutoFire.maxSpread, 0.0f, xorstr("cfg.Rage.AutoFire.maxSpread"), xorstr("Rage"));	
	DeclareConfig(&cfg.Rage.AutoFire.useMaxSpeed, false, xorstr("cfg.Rage.AutoFire.useMaxSpeed"), xorstr("Rage"));
	DeclareConfig(&cfg.Rage.AutoFire.maxSpeed, 80.0f, xorstr("cfg.Rage.AutoFire.maxSpeed"), xorstr("Rage"));
	DeclareConfig(&cfg.Rage.AutoFire.inGround, false, xorstr("cfg.Rage.AutoFire.inGround"), xorstr("Rage"));
	DeclareConfig(&cfg.Rage.AutoFire.autoScope, false, xorstr("cfg.Rage.AutoFire.autoScope"), xorstr("Rage"));
	DeclareConfig(&cfg.Rage.AutoFire.AutoBaim, false, xorstr("cfg.Rage.AutoFire.AutoBaim"), xorstr("Rage"));
	DeclareConfig(&cfg.Rage.AutoFire.BaimAfterShots, 0, xorstr("cfg.Rage.AntiAim.BaimAfterShots"), xorstr("Rage"));
	DeclareConfig(&cfg.Rage.AutoFire.prioritize, false, xorstr("cfg.Rage.AutoFire.prioritize"), xorstr("Rage"));
	DeclareConfig(&cfg.Rage.AutoFire.autocrouch, false, xorstr("cfg.Rage.AutoFire.autocrouch"), xorstr("Rage"));
	DeclareConfig(&cfg.Rage.AutoFire.hitscan, HITSCAN_NORMAL, xorstr("cfg.Rage.AutoFire.hitscan"), xorstr("Aimbot"));
	DeclareConfig(&cfg.Rage.AutoFire.multiBox, MULTIBOX_NONE, xorstr("cfg.Rage.AutoFire.multiBox"), xorstr("Aimbot"));
	DeclareConfig(&cfg.Rage.AutoFire.onlyHS, false, xorstr("cfg.Rage.AutoFire.onlyHS"), xorstr("Aimbot"));
	DeclareConfig(&cfg.Rage.AutoFire.onlyHsMoving, false, xorstr("cfg.Rage.AutoFire.onlyHsMoving"), xorstr("Aimbot"));
	DeclareConfig(&cfg.Rage.AutoFire.pointScale, 1.0f, xorstr("cfg.Rage.AutoFire.pointScale"), xorstr("Aimbot"));
	DeclareConfig(&cfg.Rage.AutoFire.autoRevolver, false, xorstr("cfg.Rage.AutoFire.autoRevolver"), xorstr("Aimbot"));
	DeclareConfig(&cfg.Rage.AutoFire.smartAim, false, xorstr("cfg.Rage.AutoFire.smartAim"), xorstr("Aimbot"));
	DeclareConfig(&cfg.Rage.AutoFire.preferBody, false, xorstr("cfg.Rage.AutoFire.preferBody"), xorstr("Aimbot"));
	DeclareConfig(&cfg.Rage.AutoFire.accuracyBoost, false, xorstr("cfg.Rage.AutoFire.accuracyBoost"), xorstr("Aimbot"));
	DeclareConfig(&cfg.Rage.AutoFire.multiPoint, false, xorstr("cfg.Rage.AutoFire.multiPoint"), xorstr("Aimbot"));
	DeclareConfig(&cfg.Rage.AutoFire.baimOnLowHitchance, false, xorstr("cfg.Rage.AutoFire.baimOnLowHitchance"), xorstr("Aimbot"));
	DeclareConfig(&cfg.Rage.AutoFire.delayShoot, false, xorstr("cfg.Rage.AutoFire.delayShoot"), xorstr("Aimbot"));
	DeclareConfig(&cfg.Rage.AutoFire.antiSilent, false, xorstr("cfg.Rage.AutoFire.antiSilent"), xorstr("Aimbot"));
	DeclareConfig(&cfg.Rage.AutoFire.onlyHsIfResolved, false, xorstr("cfg.Rage.AutoFire.onlyHsIfResolved"), xorstr("Aimbot"));
	
	DeclareConfig(&cfg.Rage.Hitscan.scale, HITBOX_COUNT, xorstr("cfg.Rage.Hitscan.scale"), xorstr("Hitscan"));
	DeclareConfig(&cfg.Rage.Hitscan.points, HITBOX_COUNT, xorstr("cfg.Rage.Hitscan.points"), xorstr("Hitscan"));
	DeclareConfig(&cfg.Rage.Hitscan.ignore, HITBOX_COUNT, xorstr("cfg.Rage.Hitscan.ignore"), xorstr("Hitscan"));
	DeclareConfig(&cfg.Rage.Hitscan.priority, HITBOX_COUNT, xorstr("cfg.Rage.Hitscan.priority"), xorstr("Hitscan"));
	
	for (int i = 0; i < HITBOX_COUNT; i++)
	{
		cfg.Rage.Hitscan.scale[i] = 0.8f;
		cfg.Rage.Hitscan.points[i] = 5;
		cfg.Rage.Hitscan.ignore[i] = false;
		cfg.Rage.Hitscan.priority[i] = i;
	}
	
	DeclareConfig(&cfg.Rage.Resolver.enabled, false, xorstr("cfg.Rage.Resolver.enabled"), xorstr("Rage"));
	DeclareConfig(&cfg.Rage.Resolver.resetTime, 3000, xorstr("cfg.Rage.Resolver.resetTime"), xorstr("Rage"));
	DeclareConfig(&cfg.Rage.Resolver.prediction, false, xorstr("cfg.Rage.Resolver.prediction"), xorstr("Rage")); 
	DeclareConfig(&cfg.Rage.Resolver.extrapolate, false, xorstr("cfg.Rage.Resolver.extrapolate"), xorstr("Rage"));
	DeclareConfig(&cfg.Rage.Resolver.fixPitch, false, xorstr("cfg.Rage.Resolver.fixPitch"), xorstr("Rage"));
	DeclareConfig(&cfg.Rage.Resolver.forceYaw, false, xorstr("cfg.Rage.Resolver.forceYaw"), xorstr("Rage"));
	DeclareConfig(&cfg.Rage.Resolver.lispCorrect, false, xorstr("cfg.Rage.Resolver.lispCorrect"), xorstr("Rage"));
	DeclareConfig(&cfg.Rage.Resolver.deltaCorrect, false, xorstr("cfg.Rage.Resolver.deltaCorrect"), xorstr("Rage"));
	DeclareConfig(&cfg.Rage.Resolver.fireOnlyResolved, false, xorstr("cfg.Rage.Resolver.fireOnlyResolved"), xorstr("Rage"));
	DeclareConfig(&cfg.Rage.Resolver.noInterpolation, false, xorstr("cfg.Rage.Resolver.noInterpolation"), xorstr("Rage"));
	DeclareConfig(&cfg.Rage.Resolver.lagCompensation, false, xorstr("cfg.Rage.Resolver.lagCompensation"), xorstr("Rage"));
	DeclareConfig(&cfg.Rage.Resolver.baimUnresolved, false, xorstr("cfg.Rage.Resolver.baimUnresolved"), xorstr("Rage"));
	DeclareConfig(&cfg.Rage.Resolver.backTrack, false, xorstr("cfg.Rage.Resolver.backTrack"), xorstr("Rage"));
	DeclareConfig(&cfg.Rage.Resolver.forcedBackTrack, false, xorstr("cfg.Rage.Resolver.forcedBackTrack"), xorstr("Rage"));
	DeclareConfig(&cfg.Rage.Resolver.backTrackOffset, 0, xorstr("cfg.Rage.Resolver.backTrackOffset"), xorstr("Rage"));
	DeclareConfig(&cfg.Rage.Resolver.backTrackMinOffset, 0, xorstr("cfg.Rage.Resolver.backTrackMinOffset"), xorstr("Rage"));
	DeclareConfig(&cfg.Rage.Resolver.drawRecord, false, xorstr("cfg.Rage.Resolver.drawRecord"), xorstr("Rage"));
	DeclareConfig(&cfg.Rage.Resolver.forceJitter, false, xorstr("cfg.Rage.Resolver.forceJitter"), xorstr("Rage"));
	DeclareConfig(&cfg.Rage.Resolver.minBruteforceBullets, 0, xorstr("cfg.Rage.Resolver.minBruteforceBullets"), xorstr("Rage"));
	DeclareConfig(&cfg.Rage.Resolver.inverseUnresolved, false, xorstr("cfg.Rage.Resolver.inverseUnresolved"), xorstr("Rage"));
	DeclareConfig(&cfg.Rage.Resolver.pingExploit, false, xorstr("cfg.Rage.Resolver.pingExploit"), xorstr("Rage"));
	DeclareConfig(&cfg.Rage.Resolver.noSpreadMode, false, xorstr("cfg.Rage.Resolver.noSpreadMode"), xorstr("Rage"));
	DeclareConfig(&cfg.Rage.Resolver.resolveLegits, false, xorstr("cfg.Rage.Resolver.resolveLegits"), xorstr("Rage"));
	DeclareConfig(&cfg.Rage.Resolver.BruteForceShots, 0, xorstr("cfg.Rage.Resolver.BruteForceShots"), xorstr("Rage"));
	DeclareConfig(&cfg.Rage.noRecoil, false, xorstr("cfg.Rage.noRecoil"), xorstr("Rage"));
	DeclareConfig(&cfg.Rage.pSilent, false, xorstr("cfg.Rage.pSilent"), xorstr("Rage"));
	DeclareConfig(&cfg.Rage.ignoreHead, false, xorstr("cfg.Rage.ignoreHead"), xorstr("Rage"));
	DeclareConfig(&cfg.Rage.ignoreArm, false, xorstr(" cfg.Rage.ignoreArm"), xorstr("Rage"));
	DeclareConfig(&cfg.Rage.FakeLag.enabled, false, xorstr("cfg.Rage.FakeLag.enabled"), xorstr("Rage"));
	DeclareConfig(&cfg.Rage.FakeLag.min, 1, xorstr("cfg.Rage.FakeLag.min"), xorstr("Rage"));
	DeclareConfig(&cfg.Rage.FakeLag.max, 1, xorstr("cfg.Rage.FakeLag.max"), xorstr("Rage"));
	DeclareConfig(&cfg.Rage.FakeLag.adaptive, false, xorstr("cfg.Rage.FakeLag.adaptive"), xorstr("Rage"));
	DeclareConfig(&cfg.Rage.FakeLag.air, 0, xorstr("cfg.Rage.FakeLag.air"), xorstr("Rage"));
	DeclareConfig(&cfg.Rage.FakeLag.edge, 0, xorstr("cfg.Rage.FakeLag.edge"), xorstr("Rage"));
	DeclareConfig(&cfg.Rage.FakeLag.random, false, xorstr("cfg.Rage.FakeLag.random"), xorstr("Rage"));
	DeclareConfig(&cfg.Rage.teleportF9, false, xorstr("cfg.Rage.teleportF9"), xorstr("Rage"));
				  
	DeclareConfig(&cfg.Visual.enabled, false, xorstr("cfg.Visual.enabled"), xorstr("Visual"));
	DeclareConfig(&cfg.Visual.dinamicFOV, false, xorstr("cfg.Visual.dinamicFOV"), xorstr("Visual"));
	DeclareConfig(&cfg.Visual.drawFOV, false, xorstr("cfg.Visual.drawFOV"), xorstr("Visual"));
	DeclareConfig(&cfg.Visual.drawCrosshair, false, xorstr("cfg.Visual.drawCrosshair"), xorstr("Visual"));
	DeclareConfig(&cfg.Visual.drawTracers, false, xorstr("cfg.Visual.drawTracers"), xorstr("Visual"));
	DeclareConfig(&cfg.Visual.drawNames, false, xorstr("cfg.Visual.drawNames"), xorstr("Visual"));
	DeclareConfig(&cfg.Visual.drawWeapons, false, xorstr("cfg.Visual.drawWeapons"), xorstr("Visual"));
	DeclareConfig(&cfg.Visual.drawHealth, false, xorstr("cfg.Visual.drawHealth"), xorstr("Visual"));
	DeclareConfig(&cfg.Visual.drawBox, false, xorstr("cfg.Visual.drawBox"), xorstr("Visual"));
	DeclareConfig(&cfg.Visual.drawBones, false, xorstr("cfg.Visual.drawBones"), xorstr("Visual"));
	DeclareConfig(&cfg.Visual.drawItems, false, xorstr("cfg.Visual.drawItems"), xorstr("Visual"));
	DeclareConfig(&cfg.Visual.drawItemsNames, false, xorstr("cfg.Visual.drawItemsNames"), xorstr("Visual"));	
	DeclareConfig(&cfg.Visual.chams, false, xorstr("cfg.Visual.chams"), xorstr("Visual"));
	DeclareConfig(&cfg.Visual.wallChams, false, xorstr("cfg.Visual.wallChams"), xorstr("Visual"));
	DeclareConfig(&cfg.Visual.espAlpha, 90, xorstr("cfg.Visual.espAlpha"), xorstr("Visual"));
	DeclareConfig(&cfg.Visual.handChams, false, xorstr("cfg.Visual.handChams"), xorstr("Visual"));
	DeclareConfig(&cfg.Visual.drawWeaponIcons, 0, xorstr("cfg.Visual.drawWeaponIcons"), xorstr("Visual"));
	DeclareConfig(&cfg.Visual.hvhChams, false, xorstr("cfg.Visual.hvhChams"), xorstr("Visual"));
	DeclareConfig(&cfg.Visual.noScope, false, xorstr("cfg.Visual.noScope"), xorstr("Visual"));
	DeclareConfig(&cfg.Visual.nadeTraces, false, xorstr("cfg.Visual.nadeTraces"), xorstr("Visual"));
	DeclareConfig(&cfg.Visual.nadePredict, false, xorstr("cfg.Visual.nadePredict"), xorstr("Visual"));
	DeclareConfig(&cfg.Visual.showTeam, false, xorstr("cfg.Visual.showTeam"), xorstr("Visual"));
	DeclareConfig(&cfg.Visual.drawAmmo, false, xorstr("cfg.Visual.drawAmmo"), xorstr("Visual"));
	DeclareConfig(&cfg.Visual.hitmarkersound, false, xorstr("cfg.Visual.hitmarkersound"), xorstr("Visual"));
	DeclareConfig(&cfg.Visual.boxFill, false, xorstr("cfg.Visual.boxFill"), xorstr("Visual"));
	DeclareConfig(&cfg.Visual.glassHands, false, xorstr("cfg.Visual.glassHands"), xorstr("Visual"));
	DeclareConfig(&cfg.Visual.granadePrediction, false, xorstr("cfg.Visual.granadePrediction"), xorstr("Visual"));
	DeclareConfig(&cfg.Visual.disablePostProcess, false, xorstr("cfg.Visual.disablePostProcess"), xorstr("Visual"));
	DeclareConfig(&cfg.Visual.disableRagdollPhysics, false, xorstr("cfg.Visual.disableRagdollPhysics"), xorstr("Visual"));
	DeclareConfig(&cfg.Visual.dormantEsp, false, xorstr("cfg.Visual.dormantEsp"), xorstr("Visual"));
	DeclareConfig(&cfg.Visual.nightMode, false, xorstr("cfg.Visual.nightMode"), xorstr("Visual"));
	DeclareConfig(&cfg.Visual.localPlayerVisuals, false, xorstr("cfg.Visual.localPlayerVisuals"), xorstr("Visual"));
	DeclareConfig(&cfg.Visual.darkHandChams, false, xorstr("cfg.Visual.darkHandChams"), xorstr("Visual"));
	DeclareConfig(&cfg.Visual.debugAutowall, false, xorstr("cfg.Visual.debugAutowall"), xorstr("Visual"));
	DeclareConfig(&cfg.Visual.flashLight, false, xorstr("cfg.Visual.flashLight"), xorstr("Visual"));
	DeclareConfig(&cfg.Visual.noFlash, false, xorstr("cfg.Visual.noFlash"), xorstr("Visual"));
	DeclareConfig(&cfg.Visual.glow, false, xorstr("cfg.Visual.glow"), xorstr("Visual"));
	DeclareConfig(&cfg.Visual.showAimPoint, false, xorstr("cfg.Visual.showAimPoint"), xorstr("Visual"));
	DeclareConfig(&cfg.Visual.fpsBoost, false, xorstr("cfg.Visual.fpsBoost"), xorstr("Visual"));
	DeclareConfig(&cfg.Visual.debugMultipoint, false, xorstr("cfg.Visual.debugMultipoint"), xorstr("Visual"));
	DeclareConfig(&cfg.Visual.lockFov, false, xorstr("cfg.Visual.lockFov"), xorstr("Visual"));
	DeclareConfig(&cfg.Visual.lagometer, false, xorstr("cfg.Visual.lagometer"), xorstr("Visual"));
	DeclareConfig(&cfg.Visual.drawHealthDivision, false, xorstr("cfg.Visual.drawHealthDivision"), xorstr("Visual"));
	DeclareConfig(&cfg.Visual.colorGlow, Color(255, 0, 0, 255), xorstr("cfg.Visual.colorGlow"), xorstr("Visual"));
	DeclareConfig(&cfg.Visual.colorChamsVisible, Color(255, 0, 0, 255), xorstr("cfg.Visual.colorChamsVisible"), xorstr("Visual"));
	DeclareConfig(&cfg.Visual.colorChamsHidden, Color(255, 255, 255, 255), xorstr("cfg.Visual.colorChamsHidden"), xorstr("Visual"));
	DeclareConfig(&cfg.Visual.damageESP, false, xorstr("cfg.Visual.damageESP"), xorstr("Visual"));

	DeclareConfig(&cfg.Misc.bunnyhop, false, xorstr("cfg.Misc.bunnyhop"), xorstr("Misc"));
	DeclareConfig(&cfg.Misc.autoStrafe, false, xorstr("cfg.Misc.autoStrafe"), xorstr("Misc"));
	DeclareConfig(&cfg.Misc.fastAutoStrafe, false, xorstr("cfg.Misc.fastAutoStrafe"), xorstr("Misc"));
	DeclareConfig(&cfg.Misc.bunnyCircle, false, xorstr("cfg.Misc.bunnyCircle"), xorstr("Misc"));
	DeclareConfig(&cfg.Misc.bunnyAutoDuck, false, xorstr("cfg.Misc.bunnyAutoDuck"), xorstr("Misc"));
	DeclareConfig(&cfg.Misc.clanTagAnimation, 0, xorstr("cfg.Misc.clanTagAnimation"), xorstr("Misc"));
	DeclareConfig(&cfg.Misc.autoBlock, false, xorstr("cfg.Misc.autoBlock"), xorstr("Misc"));
	DeclareConfig(&cfg.Misc.thirdPerson, false, xorstr("cfg.Misc.thirdPerson"), xorstr("Misc"));
	DeclareConfig(&cfg.Misc.fovChanger, 0.0f, xorstr("cfg.Misc.fovChanger"), xorstr("Misc"));
	DeclareConfig(&cfg.Misc.nameStealer, false, xorstr("cfg.Misc.nameStealer"), xorstr("Misc"));
	DeclareConfig(&cfg.Misc.noSmoke, false, xorstr("cfg.Misc.noSmoke"), xorstr("Misc"));
	DeclareConfig(&cfg.Misc.bypassSvCheats, false, xorstr("cfg.Misc.bypassSvCheats"), xorstr("Misc"));
	DeclareConfig(&cfg.Misc.retrackSpeed, 5.9f, xorstr("cfg.Misc.retrackSpeed"), xorstr("Misc"));
	DeclareConfig(&cfg.Misc.buunyhopStopAtWall, false, xorstr("cfg.Misc.buunyhopStopAtWall"), xorstr("Misc"));
	DeclareConfig(&cfg.Misc.speedDivisor, 6.0f, xorstr("cfg.Misc.speedDivisor"), xorstr("Misc"));
	DeclareConfig(&cfg.Misc.boxCircleStrafe, false, xorstr("cfg.Misc.boxCircleStrafe"), xorstr("Misc"));
	DeclareConfig(&cfg.Misc.spoofAutoBunnyhop, false, xorstr("cfg.Misc.spoofAutoBunnyhop"), xorstr("Misc"));
	DeclareConfig(&cfg.Misc.unlockFps, false, xorstr("cfg.Misc.unlockFps"), xorstr("Misc"));
	DeclareConfig(&cfg.Misc.airStuck, false, xorstr("cfg.Misc.airStuck"), xorstr("Misc"));
	DeclareConfig(&cfg.Misc.autoaccept, 0, xorstr("cfg.Misc.autoaccept"), xorstr("Misc"));


	DeclareConfig(&cfg.LegitBot.enabled, false, xorstr("cfg.LegitBot.enabled"), xorstr("LegitBot"));
	DeclareConfig(&cfg.LegitBot.aimbot, false, xorstr("cfg.LegitBot.aimbot"), xorstr("LegitBot"));
	DeclareConfig(&cfg.LegitBot.fov, 35.0f, xorstr("cfg.LegitBot.fov"), xorstr("LegitBot"));
	DeclareConfig(&cfg.LegitBot.smooth, 0.5f, xorstr("cfg.LegitBot.smooth"), xorstr("LegitBot"));
	DeclareConfig(&cfg.LegitBot.backtrack, false, xorstr("cfg.LegitBot.backtrack"), xorstr("LegitBot"));
	DeclareConfig(&cfg.LegitBot.triggerBot, false, xorstr("cfg.LegitBot.triggerBot"), xorstr("LegitBot"));
	DeclareConfig(&cfg.LegitBot.friendlyFire, false, xorstr("cfg.LegitBot.friendlyFire"), xorstr("LegitBot"));
	DeclareConfig(&cfg.LegitBot.silent, false, xorstr("cfg.LegitBot.silent"), xorstr("LegitBot"));
	DeclareConfig(&cfg.LegitBot.delay, 0.5f, xorstr("cfg.LegitBot.delay"), xorstr("LegitBot"));
	DeclareConfig(&cfg.LegitBot.aimLock, false, xorstr("cfg.LegitBot.aimLock"), xorstr("LegitBot"));
	DeclareConfig(&cfg.LegitBot.triggerDelay, 0.095f, xorstr("cfg.LegitBot.triggerDelay"), xorstr("LegitBot"));
}

void ConfigManager::AddConfig(void *pointer, const char *name, const char *category, std::string type)
{
	configs.push_back(ConfigItem(std::string(name), pointer, type));
}

void ConfigManager::DeclareConfig(int *pointer, int value, const char *name, const char *category)
{
	AddConfig(pointer, name, category, "int");
	*pointer = value;
}

void ConfigManager::DeclareConfig(float *pointer, float value, const char *name, const char *category)
{
	AddConfig(pointer, name, category, "float");
	*pointer = value;
}

void ConfigManager::DeclareConfig(bool *pointer, bool value, const char *name, const char *category)
{
	AddConfig(pointer, name, category, "bool");
	*pointer = value;
}

void ConfigManager::DeclareConfig(std::vector<int> *pointer, int size, const char *name, const char *category)
{
	AddConfig(pointer, name, category, "vector<int>");

	pointer->clear();

	for (int i = 0; i < size; i++)
		pointer->push_back(0);
}

void ConfigManager::DeclareConfig(std::vector<float> *pointer, int size, const char *name, const char *category)
{
	AddConfig(pointer, name, category, "vector<float>");
	
	pointer->clear();

	for (int i = 0; i < size; i++)
		pointer->push_back(0);
}

void ConfigManager::DeclareConfig(Color *pointer, Color value, const char *name, const char *category)
{
	AddConfig(pointer, name, category, "Color");
	*pointer = value;
}

std::string ConfigManager::GetConfigPath()
{
	std::string path = "C:\\BLAXED\\CFG\\";
	CreateDirectory(path.c_str(), NULL);
	return path;
}

void ConfigManager::GetConfigFiles()
{
	files.clear();

	std::string path = GetConfigPath() + "/*.json";// "/*.*";

	WIN32_FIND_DATA fd;

	HANDLE hFind = ::FindFirstFile(path.c_str(), &fd);

	if (hFind != INVALID_HANDLE_VALUE) 
	{
		do 
		{
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) 
			{
				files.push_back(fd.cFileName);
			}
		} 
		while (::FindNextFile(hFind, &fd));

		::FindClose(hFind);
	}
}

void ConfigManager::WriteConfig(std::string fileName)
{
	std::ofstream ofs;

	ofs.open(GetConfigPath() + fileName + "", ios::out | ios::trunc);

	json allJson;
	
	for (auto it = configs.begin(); it != configs.end(); ++it)
	{
		ConfigItem &config = *it;
		json j;

		j["name"] = config.name;
		j["type"] = config.type;

		if (!config.type.compare("int"))
		{
			j["value"] = (int)*(int*)config.pointer;
		}
		else if (!config.type.compare("float"))
		{
			j["value"] = (float)*(float*)config.pointer;
		}
		else if (!config.type.compare("bool"))
		{
			j["value"] = (bool)*(bool*)config.pointer;
		}
		else if (!config.type.compare("vector<float>"))
		{
			auto& ptr = *(vector<float>*)config.pointer;

			json ja;

			for (auto& i : ptr)
				ja.push_back(i);

			j["value"] = ja.dump();
		}
		else if (!config.type.compare("vector<int>"))
		{
			auto& ptr = *(vector<int>*)config.pointer;

			json ja;

			for (auto& i : ptr)
				ja.push_back(i);

			j["value"] = ja.dump();
		}
		else if (!config.type.compare("Color"))
		{
			Color c = *(Color*)(config.pointer);

			vector<int> a = { c.r(),  c.g(),  c.b(),  c.a() };

			json ja;

			for (auto& i : a)
				ja.push_back(i);

			j["value"] = ja.dump();
		}

		allJson.push_back(j);
	}

	std::string data = allJson.dump();

	ofs << std::setw(4) << data << std::endl;

	ofs.close();
}

void ConfigManager::ReadConfig(std::string fileName)
{
	std::ifstream ifs;
	std::string data;

	std::string path = GetConfigPath() + fileName + "";

	ifs.open(path);

	json allJson;

	ifs >> allJson;

	for (json::iterator it = allJson.begin(); it != allJson.end(); ++it)
	{
		//json j = json::parse(it->dump().c_str());
		json j = *it;

		std::string name = j["name"];
		std::string type = j["type"];

		ConfigItem *item = FindItem(name);

		if (item)
		{
			if (!type.compare("int"))
			{
				*(int*)item->pointer = j["value"].get<int>();
			}
			else if (!type.compare("float"))
			{
				*(float*)item->pointer = j["value"].get<float>();
			}
			else if (!type.compare("bool"))
			{
				*(bool*)item->pointer = j["value"].get<bool>();
			}
			else if (!type.compare("vector<float>"))
			{
				auto ptr = static_cast<vector<float>*>(item->pointer);
				ptr->clear();

				json ja = json::parse(j["value"].get<string>().c_str());
				
				for (json::iterator it = ja.begin(); it != ja.end(); ++it)
					ptr->push_back(*it);
			}
			else if (!type.compare("vector<int>"))
			{
				auto ptr = static_cast<vector<int>*>(item->pointer);
				ptr->clear();

				json ja = json::parse(j["value"].get<string>().c_str());

				for (json::iterator it = ja.begin(); it != ja.end(); ++it)
					ptr->push_back(*it);
			}
			else if (!type.compare("Color"))
			{
				vector<int> a;

				json ja = json::parse(j["value"].get<string>().c_str());

				for (json::iterator it = ja.begin(); it != ja.end(); ++it)
					a.push_back(*it);

				*(Color*)item->pointer = Color((BYTE)a[0], (BYTE)a[1], (BYTE)a[2], (BYTE)a[3]);
			}
		}
	}
	
	ifs.close();

	actualConfig = fileName;
}

ConfigItem *ConfigManager::FindItem(std::string name)
{
	for (int i = 0; i < (int)configs.size(); i++)
	{
		if (configs[i].name.compare(name) == 0)
			return &(configs[i]);
	}

	return nullptr;
}

void ConfigManager::RemoveConfig(std::string fileName)
{
	std::string path = GetConfigPath() + fileName + "";

	std::remove(path.c_str());
}