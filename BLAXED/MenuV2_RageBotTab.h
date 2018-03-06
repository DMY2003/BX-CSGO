#pragma once

#include "AntiAimV2.h"

void MenuV2::DrawRagebotTab()
{
	static vector<string> rageTabs;

	if (rageTabs.size() == 0)
	{
		rageTabs.push_back(xorstr("AIMBOT"));
		rageTabs.push_back(xorstr("ANTI-AIM"));
		rageTabs.push_back(xorstr("AUTO STUFF"));
		rageTabs.push_back(xorstr("RESOLVER"));
		rageTabs.push_back(xorstr("FAKE LAG"));
		rageTabs.push_back(xorstr("ADVANCED"));
	}

	static int selectedTabRage = 0;

	ImGui::TabLabels(rageTabs, selectedTabRage, true);
	{
		switch (selectedTabRage)
		{
		case 0: DrawAimbotTab(); break;
		case 1: DrawAntiAimTab(); break;
		case 2: DrawAutoFireTab(); break;
		case 3: DrawResolverTab(); break;
		case 4: DrawFakeLagTab(); break;
		case 5: DrawAdvancedRageBotTab(); break;
		}
	}
}

void MenuV2::DrawAntiAimTab()
{
	ImGui::Checkbox(MENU_TRANSLATE("Ativar anti-aim", "ENABLE"), &cfg.Rage.AntiAim.enabled);


	if (cfg.Rage.AntiAim.enabled)
	{
		ImGui::Columns(2);

		static vector<string> antiAims;
		static char antiAimsStr[1024];

		if (antiAims.size() == 0)
		{
			antiAims.push_back(xorstr("NONE"));
			antiAims.push_back(xorstr("LEGIT"));
			antiAims.push_back(xorstr("BACKWARD"));
			antiAims.push_back(xorstr("SPIN"));
			antiAims.push_back(xorstr("LBY INVERSE"));
			antiAims.push_back(xorstr("AUTO"));
			antiAims.push_back(xorstr("BACKWARD AT TARGETS"));
			antiAims.push_back(xorstr("LBY SPIN"));
			antiAims.push_back(xorstr("INVADE AA"));
			antiAims.push_back(xorstr("SYNCHRONIZE AA"));

			VectorToArray(antiAims, antiAimsStr);
		}

		static vector<string> antiAimsLBY;
		static char antiAimsLBYStr[1024];

		if (antiAimsLBY.size() == 0)
		{
			antiAimsLBY.push_back(xorstr("NONE"));
			antiAimsLBY.push_back(xorstr("INVERSE"));
			antiAimsLBY.push_back(xorstr("AUTO OUT"));
			antiAimsLBY.push_back(xorstr("AUTO IN"));
			antiAimsLBY.push_back(xorstr("AIR"));
			antiAimsLBY.push_back(xorstr("CUSTOM"));
			antiAimsLBY.push_back(xorstr("AIR INVERSE"));

			VectorToArray(antiAimsLBY, antiAimsLBYStr);
		}

		static vector<string> antiAimsPitch;
		static char antiAimsPitchStr[1024];

		if (antiAimsPitch.size() == 0)
		{
			antiAimsPitch.push_back(xorstr("NONE"));
			antiAimsPitch.push_back(xorstr("LEGIT"));
			antiAimsPitch.push_back(xorstr("DOWN"));
			antiAimsPitch.push_back(xorstr("UP"));
			antiAimsPitch.push_back(xorstr("FAKE DOWN [UT]"));
			antiAimsPitch.push_back(xorstr("FAKE UP [UT]"));
			antiAimsPitch.push_back(xorstr("FAKE ZERO [UT]"));
			antiAimsPitch.push_back(xorstr("FAKE UP CYCLE [UT]"));
			antiAimsPitch.push_back(xorstr("CUSTOM"));

			VectorToArray(antiAimsPitch, antiAimsPitchStr);
		}

		ImGui::Combo(xorstr("PITCH"), &cfg.Rage.AntiAim.pitchStyle, antiAimsPitchStr);

		if (cfg.Rage.AntiAim.pitchStyle == AntiAimV2::X_CUSTOM)
		{
			ImGui::Text(MENU_TRANSLATE("Pitch personalizado", "CUSTOM PITCH"));
			ImGui::InputFloat(xorstr("##aadummycustompitch"), &cfg.Rage.AntiAim.customPitch);
		}

		ImGui::Combo(MENU_TRANSLATE("Real parado", "REAL STANDING"), &cfg.Rage.AntiAim.realStyle, antiAimsStr);
		ImGui::Combo(MENU_TRANSLATE("Real movimentando", "REAL MOVING"), &cfg.Rage.AntiAim.movingRealStyle, antiAimsStr);
		ImGui::Combo(MENU_TRANSLATE("Real agaixando", "REAL DUCKING"), &cfg.Rage.AntiAim.duckingRealStyle, antiAimsStr);
		ImGui::Combo(MENU_TRANSLATE("Anti resolver", "ANTI-RESOLVER"), &cfg.Rage.AntiAim.lowerBodyStyle, antiAimsLBYStr);
		ImGui::Combo(xorstr("FAKE"), &cfg.Rage.AntiAim.fakeStyle, antiAimsStr);

		if (cfg.Rage.AntiAim.lowerBodyStyle == AntiAimV2::LBY_CUSTOM)
		{
			ImGui::Text(xorstr("ANTI RESOLVER OFFSET"));
			ImGui::SliderFloat(xorstr("##aadummyfakelbyoffset"), &cfg.Rage.AntiAim.fakelLbyOffset, -180.0f, 180.0f);
		}

		ImGui::Text(xorstr("REAL OFFSET"));
		ImGui::SliderFloat(xorstr("##aadummyrealoffset"), &cfg.Rage.AntiAim.realYawOffset, -180.0f, 180.0f);
		ImGui::Text(xorstr("FAKE OFFSET"));
		ImGui::SliderFloat(xorstr("##aadummyfakeoffset"), &cfg.Rage.AntiAim.fakeYawOffset, -180.0f, 180.0f);

		ImGui::Text(MENU_TRANSLATE("Real LJitter parado", "REAL LJITTER STANDING"));
		ImGui::SliderFloat(xorstr("##aadummyjitter"), &cfg.Rage.AntiAim.lbyJitter, 0.0f, 90.0f);
		ImGui::Text(MENU_TRANSLATE("Real LJitter movimentando", "REAL LJITTER MOVING"));
		ImGui::SliderFloat(xorstr("##aadummyjittermoving"), &cfg.Rage.AntiAim.lbyJitterMoving, 0.0f, 90.0f);
		ImGui::SameLine();
		ImGui::Checkbox(MENU_TRANSLATE("Devagar", "SLOW"), &cfg.Rage.AntiAim.lbyJitterOnlySlow);
		ImGui::Text(MENU_TRANSLATE("Slow motion", "SLOW MOTION"));
		ImGui::SliderInt(xorstr("##aadummyslowmotionticks"), &cfg.Rage.AntiAim.slowMotionTicks, 0, 15);

		ImGui::NextColumn();

		ImGui::Checkbox(xorstr("STATIC BASE"), &cfg.Rage.AntiAim.staticAA);
		ImGui::Checkbox(xorstr("LOWERBODY BASE"), &cfg.Rage.AntiAim.lowerBody);
		ImGui::Checkbox(MENU_TRANSLATE("Virar para alvos", "AT TARGETS"), &cfg.Rage.AntiAim.atTargets);

		ImGui::Checkbox(xorstr("REAL TJITTER"), &cfg.Rage.AntiAim.realJitter);
		ImGui::Checkbox(xorstr("FAKE TJITTER"), &cfg.Rage.AntiAim.fakeJitter);

		ImGui::Checkbox(MENU_TRANSLATE("Destivar na faca", "DISABLE WITH KNIFE"), &cfg.Rage.AntiAim.removeOnKnife);

		ImGui::Checkbox(MENU_TRANSLATE("Mostrar AA real", "SHOW REAL AA"), &cfg.Rage.AntiAim.viewAntiAim);
		ImGui::Checkbox(MENU_TRANSLATE("Mostrar AA fake", "SHOW FAKE AA"), &cfg.Rage.AntiAim.viewAntiAimFake);
		ImGui::Checkbox(MENU_TRANSLATE("Mostrar LBY [Ghost]", "SHOW LBY [GHOST]"), &cfg.Rage.AntiAim.viewLowerBodyYawTarget);

		ImGui::Checkbox(MENU_TRANSLATE("Terceira pessoa [Bind: MOUSE3]", "THIRD PERSON [BIND: MOUSE3]"), &cfg.Misc.thirdPerson);
	}
}

void MenuV2::DrawAutoFireTab()
{
	ImGui::Checkbox(MENU_TRANSLATE("Atirar automaticamente", "AUTO FIRE"), &cfg.Rage.AutoFire.enabled);

	//if (cfg.Rage.AutoFire.enabled)
	{
		ImGui::Columns(2);

		/*vector<string> hitscanTypes;
		char hitscanTypesStr[1024];
		memset(hitscanTypesStr, 0, 1024);

		hitscanTypes.push_back(MENU_TRANSLATE("Desativado", "Off"));
		hitscanTypes.push_back(MENU_TRANSLATE(u8"Padrão", "Default"));
		hitscanTypes.push_back(MENU_TRANSLATE("Maior dano", "High damage"));
		hitscanTypes.push_back(MENU_TRANSLATE("Priorizar caminho livre", "Priorize free path"));
		VectorToArray(hitscanTypes, hitscanTypesStr);*/

		ImGui::Checkbox(xorstr("AUTO WALL"), &cfg.Rage.AutoWall.enabled);

		ImGui::Text(MENU_TRANSLATE(u8"Dano minímo", "MIN DAMAGE"));
		ImGui::SliderFloat(xorstr("##dummyragebot0"), &cfg.Rage.AutoWall.minDamage, 1.0f, 120.f, "%.0f");

		ImGui::Text(MENU_TRANSLATE("Hitchance", "HITCHANCE"));
		ImGui::SliderFloat(xorstr("##dummyautofire0"), &cfg.Rage.AutoFire.hitchance, 0.0f, 100.0f, "%.0f");

		//ImGui::SliderInt(xorstr("Hitchance max seed"), &cfg.Rage.AutoFire.hitchanceMaxSeed, 0, 256);
		cfg.Rage.AutoFire.hitchanceMaxSeed = 256;

		ImGui::SliderInt("BODY-AIM", &cfg.Rage.AutoFire.BaimAfterShots, 0, 10);

		/*ImGui::Checkbox(xorstr("Spread Limit"), &cfg.Rage.AutoFire.useMaxSpread);
		if (cfg.Rage.AutoFire.useMaxSpread)
		{
		//ImGui::SameLine();
		ImGui::SliderFloat(xorstr("spred max"), &cfg.Rage.AutoFire.maxSpread, 0.0f, 100.0f);
		}*/

		/*ImGui::Checkbox(MENU_TRANSLATE(u8"Velocidade máxima", "Max speed"), &cfg.Rage.AutoFire.useMaxSpeed);
		if (cfg.Rage.AutoFire.useMaxSpeed)
		{
		//ImGui::SameLine();
		ImGui::SliderFloat(xorstr("vel max"), &cfg.Rage.AutoFire.maxSpeed, 0.0f, 200.0f);
		}*/


		//ImGui::Text(MENU_TRANSLATE("Hitscan", "Hitscan"));
		//ImGui::Combo(xorstr("##dummyaimbot2"), &cfg.Rage.AutoFire.hitscan, hitscanTypesStr);

		ImGui::Checkbox(MENU_TRANSLATE(u8"Adaptar ao FPS", "FPS ADAPTIVE"), &cfg.Rage.AutoWall.fast);
		ImGui::Checkbox(MENU_TRANSLATE(u8"Ignorar cabeça", "IGNORE HEAD"), &cfg.Rage.ignoreHead);
		//ImGui::SameLine();
		ImGui::Checkbox(MENU_TRANSLATE(u8"Ignorar braços e pernas", "IGNORE ARMS & LEGS"), &cfg.Rage.ignoreArm);
		ImGui::Checkbox(MENU_TRANSLATE("Apenas HS", "HS ONLY"), &cfg.Rage.AutoFire.onlyHS);
		ImGui::Checkbox(MENU_TRANSLATE("Apenas HS se resolvido", "HS ONLY IF RESOLVED"), &cfg.Rage.AutoFire.onlyHsIfResolved);

		//ImGui::SameLine();
		ImGui::Checkbox(MENU_TRANSLATE("Apenas HS se movimentando", "HS ONLY AT MOVING TARGETS"), &cfg.Rage.AutoFire.onlyHsMoving);
		ImGui::Checkbox(xorstr("SMART AIM"), &cfg.Rage.AutoFire.smartAim);
		//ImGui::SameLine();
		ImGui::Checkbox(MENU_TRANSLATE("Preferir baim", "PREFER BAIM"), &cfg.Rage.AutoFire.preferBody);
		ImGui::Checkbox(MENU_TRANSLATE("Baim caso baixa hitchance", "BAIM ON LOW HITCHANCE"), &cfg.Rage.AutoFire.baimOnLowHitchance);
		ImGui::Checkbox(MENU_TRANSLATE("Preferir baim", "AUTO BAIM"), &cfg.Rage.AutoFire.AutoBaim);

		static vector<string> multibox;
		static char multiboxStr[1024];

		if (multibox.size() == 0)
		{
			multibox.push_back(xorstr("NONE"));
			multibox.push_back(xorstr("HEAD - NECK"));
			multibox.push_back(xorstr("HEAD - CHEST"));
			multibox.push_back(xorstr("HEAD - NECK - CHEST"));

			VectorToArray(multibox, multiboxStr);
		}

		ImGui::Combo(xorstr("MULTIBOX"), &cfg.Rage.AutoFire.multiBox, multiboxStr);

		ImGui::NextColumn();

		ImGui::Checkbox(MENU_TRANSLATE(u8"Boost de acurácia", "ACCURACY BOOST"), &cfg.Rage.AutoFire.accuracyBoost);
		ImGui::Checkbox(MENU_TRANSLATE("Atrasar tiro", "DELAY SHOOT"), &cfg.Rage.AutoFire.delayShoot);
		ImGui::Checkbox(xorstr("PSILENT"), &cfg.Rage.pSilent);
		ImGui::Checkbox(xorstr("ANTI PSILENT"), &cfg.Rage.AutoFire.antiSilent);

		if (cfg.Rage.AutoFire.antiSilent)
			cfg.Rage.pSilent = false;

		ImGui::Checkbox(xorstr("AUTO REVOLVER"), &cfg.Rage.AutoFire.autoRevolver);
		ImGui::Checkbox(MENU_TRANSLATE("Compensar recuo", "NO RECOIL"), &cfg.Rage.noRecoil);
		//ImGui::Checkbox(xorstr("Apenas no chao"), &cfg.Rage.AutoFire.inGround);
		cfg.Rage.AutoFire.inGround = false;
		ImGui::Checkbox(xorstr("AUTO SCOPE"), &cfg.Rage.AutoFire.autoScope);
		ImGui::Checkbox(MENU_TRANSLATE("Parar automaticamente", "AUTO STOP"), &cfg.Rage.AutoFire.autoStop);
		ImGui::Checkbox(MENU_TRANSLATE("Parar Crouch", "AUTO CROUCH"), &cfg.Rage.AutoFire.autocrouch);
	}
}

void MenuV2::DrawAdvancedRageBotTab()
{
	ImGui::Text(MENU_TRANSLATE("PS-point scale, MP-multi point, D-desativar", "                                      PS = POINT SCALE, MP = MULTI POINT, D = DISABLE"));

	ImGui::Columns(4);

	for (int i = 0; i < HITBOX_COUNT; i++)
	{
		int hitbox = i;

		ImGui::Text(GetHitboxName(hitbox).c_str());
		ImGui::NextColumn();

		ImGui::Text(MENU_TRANSLATE("PS", "PS"));
		ImGui::SameLine();
		ImGui::SliderFloat(string(string(xorstr("##dummyragebotadvancedscale")) + GetHitboxName(hitbox)).c_str(), &cfg.Rage.Hitscan.scale[hitbox], 0.0f, 1.0f, "%.1f");
		ImGui::NextColumn();

		ImGui::Text(MENU_TRANSLATE("MP", "MP"));
		ImGui::SameLine();
		ImGui::SliderInt(string(string(xorstr("##dummyragebotadvancedpoints")) + GetHitboxName(hitbox)).c_str(), &cfg.Rage.Hitscan.points[hitbox], 0, 8);
		ImGui::NextColumn();

		/*ImGui::Text(MENU_TRANSLATE("I", "I"));
		ImGui::SameLine();
		ImGui::SliderInt(string(string(xorstr("##dummyragebotadvancedpriority")) + GetHitboxName(hitbox)).c_str(), &cfg.Rage.Hitscan.priority[hitbox], 0, HITBOX_COUNT - 1);
		ImGui::NextColumn();*/

		ImGui::Text(MENU_TRANSLATE("D", "D"));
		ImGui::SameLine();
		ImGui::Checkbox(string(string(xorstr("##dummyragebotadvancedignore")) + GetHitboxName(hitbox)).c_str(), (bool*)&cfg.Rage.Hitscan.ignore[hitbox]);
		ImGui::NextColumn();
	}

	ImGui::NextColumn();

	if (ImGui::Button(xorstr("RESET PS")))
	{
		for (int i = 0; i < HITBOX_COUNT; i++)
		{
			cfg.Rage.Hitscan.scale[i] = 0.5f;
		}
	}

	ImGui::NextColumn();

	if (ImGui::Button(xorstr("MIN. MP")))
	{
		for (int i = 0; i < HITBOX_COUNT; i++)
			cfg.Rage.Hitscan.points[i] = 0;
	}

	if (ImGui::Button(xorstr("MEDIUM MP")))
	{
		for (int i = 0; i < HITBOX_COUNT; i++)
			cfg.Rage.Hitscan.points[i] = 3;
	}

	if (ImGui::Button(xorstr("MAX MP")))
	{
		for (int i = 0; i < HITBOX_COUNT; i++)
			cfg.Rage.Hitscan.points[i] = 8;
	}

	ImGui::NextColumn();

	/*if (ImGui::Button(xorstr("Reset I")))
	{
	for (int i = 0; i < HITBOX_COUNT; i++)
	cfg.Rage.Hitscan.priority[i] = i;
	}

	ImGui::NextColumn();*/

	if (ImGui::Button(xorstr("RESET D")))
	{
		for (int i = 0; i < HITBOX_COUNT; i++)
			cfg.Rage.Hitscan.ignore[i] = 0;
	}
}

void MenuV2::DrawResolverTab()
{
	ImGui::Checkbox(xorstr("ANTI ANTI-AIM"), &cfg.Rage.Resolver.enabled);

	if (cfg.Rage.Resolver.enabled)
	{
		ImGui::SliderInt(xorstr("RESET TIME [MS]"), &cfg.Rage.Resolver.resetTime, 500, 5000);
		ImGui::Checkbox(xorstr("FORCE TO LBY"), &cfg.Rage.Resolver.lispCorrect);
		ImGui::SameLine();
		ImGui::Checkbox(xorstr("DELTA FIX"), &cfg.Rage.Resolver.deltaCorrect);
		ImGui::Checkbox(xorstr("BRUTEFORCE YAW"), &cfg.Rage.Resolver.forceYaw);
		ImGui::SameLine();
		ImGui::Checkbox(xorstr("BRUTEFORCE PITCH"), &cfg.Rage.Resolver.fixPitch);
		ImGui::SliderInt(xorstr("BRUTEFORCE CHACHE"), &cfg.Rage.Resolver.minBruteforceBullets, 0, 5);
		ImGui::Checkbox(xorstr("RESOLVE LEGITS"), &cfg.Rage.Resolver.resolveLegits);
		cfg.Rage.Resolver.minBruteforceBullets = 0;
		ImGui::SliderInt(xorstr("BRUTEFORCE AFTER X SHOTS"), &cfg.Rage.Resolver.BruteForceShots, 0, 20);
		ImGui::Checkbox(MENU_TRANSLATE("MODO NOSPREAD", "NOSPREAD MODE"), &cfg.Rage.Resolver.noSpreadMode);
		ImGui::Checkbox(xorstr("FORCE JITTER"), &cfg.Rage.Resolver.forceJitter);
		ImGui::Checkbox(xorstr("NO INTERPOLATION"), &cfg.Rage.Resolver.noInterpolation);
		ImGui::Separator();
		ImGui::Text(xorstr("BACKTRACK"));

		ImGui::Checkbox(MENU_TRANSLATE("Mover no tempo", "ENABLE"), &cfg.Rage.Resolver.backTrack);
		if (cfg.Rage.Resolver.backTrack)
		{
			const char *opt_LagCompType[] = { "ONLY BEST RECORDS", "BEST AND NEWEST", "ALL RECORDS (FPS WARNING)", "ONLY ON SHOT (DEBUG)", "OLDEST (DEBUG)" };
			ImGui::Checkbox(MENU_TRANSLATE("EXIBIR CACHE", "SHOW CACHE"), &cfg.Rage.Resolver.drawRecord);
			ImGui::Combo("Type##Rage", &cfg.Rage.Resolver.backtrackingtype, opt_LagCompType, 5);
			//ImGui::SliderInt(xorstr("Min Offset"), &cfg.Rage.Resolver.backTrackMinOffset, 0, Backtrack::pRecordManager->GetMaxRecords());
			//ImGui::SliderInt(xorstr("Offset"), &cfg.Rage.Resolver.backTrackOffset, 0, Backtrack::pRecordManager->GetMaxRecords());

			/*if (TICKS_TO_TIME(cfg.Rage.Resolver.backTrackMinOffset) > 0.2f && cfg.Rage.AntiAim.untrusted)
			ImGui::Checkbox(xorstr("Ping exploit"), &cfg.Rage.Resolver.pingExploit);
			else
			cfg.Rage.Resolver.pingExploit = false;*/

			if (cfg.Rage.Resolver.backTrackOffset < cfg.Rage.Resolver.backTrackMinOffset)
				cfg.Rage.Resolver.backTrackOffset = cfg.Rage.Resolver.backTrackMinOffset;
		}
		else
		{
			cfg.Rage.Resolver.drawRecord = false;
		}

		ImGui::Separator();
		ImGui::Checkbox(MENU_TRANSLATE("ATIRAR QUANDO RESOLVIDO", "FIRE ONLY RESOLVED"), &cfg.Rage.Resolver.fireOnlyResolved);
		ImGui::Checkbox(MENU_TRANSLATE("BAIM UNRESOLVED", "BAIM UNRESOLVED"), &cfg.Rage.Resolver.baimUnresolved);
		ImGui::SameLine();
		ImGui::Checkbox(xorstr("INVERSE UNRESOLVED"), &cfg.Rage.Resolver.inverseUnresolved);

		ImGui::Separator();
		ImGui::Text(xorstr("PREDICTION"));
		ImGui::Checkbox(MENU_TRANSLATE("EXTRAPOLAR", "EXTRAPOLATE"), &cfg.Rage.Resolver.extrapolate);
	}

	ImGui::Separator();

	ImGui::Checkbox(xorstr("LOCAL PLAYER PREDICTION"), &cfg.Rage.Resolver.prediction);
}

void MenuV2::DrawFakeLagTab()
{
	ImGui::Checkbox(xorstr("ENABLE"), &cfg.Rage.FakeLag.enabled);

	if (cfg.Rage.FakeLag.enabled)
	{
		ImGui::Checkbox(MENU_TRANSLATE("ADAPTATIVO", "ADAPTIVE"), &cfg.Rage.FakeLag.adaptive);

		if (cfg.Rage.FakeLag.adaptive)
		{
			ImGui::SliderInt(xorstr("MIN"), &cfg.Rage.FakeLag.min, 1, 14);
			ImGui::SliderInt(xorstr("MAX"), &cfg.Rage.FakeLag.max, 1, 14);
			ImGui::Checkbox(xorstr("RANDOM"), &cfg.Rage.FakeLag.random);

			if (cfg.Rage.FakeLag.min > cfg.Rage.FakeLag.max)
				cfg.Rage.FakeLag.max = cfg.Rage.FakeLag.min;

			if (cfg.Rage.FakeLag.max < cfg.Rage.FakeLag.min)
				cfg.Rage.FakeLag.min = cfg.Rage.FakeLag.max;

			cfg.Rage.FakeLag.air = 0;
		}
		else
		{
			ImGui::SliderInt(xorstr("TICKS"), &cfg.Rage.FakeLag.min, 1, 14);
			ImGui::SliderInt(MENU_TRANSLATE("NO AR", "ON AIR"), &cfg.Rage.FakeLag.air, 0, 14);

			cfg.Rage.FakeLag.max = cfg.Rage.FakeLag.min;
		}

		ImGui::SliderInt(xorstr("EDGE"), &cfg.Rage.FakeLag.edge, 0, 14);
	}
}

