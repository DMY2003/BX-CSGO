#pragma once

#include "ImGUI\imconfig.h"
#include "ImGUI\imgui.h"
#include "ImGUI\dx9\imgui_dx9.h"

#include "SDK.h"
#include "Configs.h"
#include "Global.h"

#define MENU_TRANSLATE(ptbr, eng) cfg.language == 0 ? xorstr(eng) : xorstr(ptbr)
class MenuV2
{
public:
	static void ResetStyle();
	static void GUI_Init();

	static void DrawAimbotTab();
	static void DrawVisualTab();
	static void DrawRagebotTab();
	static void DrawAntiAimTab();
	static void DrawAutoFireTab();
	static void DrawResolverTab();
	static void DrawFakeLagTab();
	static void DrawMiscTab();
	static void DrawConfigTab();
	static void DrawPlayerListTab();
	static void DrawLegitBotTab();
	static void DrawAdvancedRageBotTab();
	static void DrawVisualColorsTab();
	static void DrawVisualVisualTab();

	static void Draw();

	static void VectorToArray(std::vector<string> vec, char *arr);
	static void replace(std::string &string, const std::string& search, const std::string& replace);

	static std::string SanitizeName(char *name);
	static std::string GetHitboxName(int hitbox);
};


