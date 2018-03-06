#include "MenuV2.h"
#include "Render.h"
#include "imgui_tabs.h"


#include "Prediction.h"

#include "MenuV2_AimbotTab.h"
#include "MenuV2_VisualTab.h"
#include "MenuV2_RageBotTab.h"
#include "MenuV2_PlayerListTab.h"
#include "MenuV2_ConfigTab.h"
#include "MenuV2_MiscTab.h"
#include "MenuV2_LegitBotTab.h"

#define RGBA_TO_FLOAT(r,g,b,a) (float)r/255.0f, (float)g/255.0f, (float)b/255.0f, (float)a/255.0f

std::string MenuV2::SanitizeName(char *name)
{
	name[127] = '\0';

	std::string tmp(name);
	//replace(tmp, std::string("\n"), std::string(""));
	//replace(tmp, std::string("\r"), std::string(""));

	for (int i = 0; i < (int)tmp.length(); i++)
	{
		if ((
			tmp[i] >= 'a' && tmp[i] <= 'z' ||
			tmp[i] >= 'A' && tmp[i] <= 'Z' ||
			tmp[i] >= '0' && tmp[i] <= '9' ||
			tmp[i] == ' ' || tmp[i] == '.' || tmp[i] == '/' || tmp[i] == ':' ||
			tmp[i] == ',' || tmp[i] == '_' || tmp[i] == '#' || tmp[i] == '$' || 
			tmp[i] == '<' || tmp[i] == '>' || tmp[i] == '-' || tmp[i] == '+' || 
			tmp[i] == '*' || tmp[i] == '%' || tmp[i] == '@' || tmp[i] == '('||
			tmp[i] == ')' || tmp[i] == '{' || tmp[i] == '}' || tmp[i] == '[' || tmp[i] == ']' || 
			tmp[i] == '!' || tmp[i] == '&' || tmp[i] == '~' || tmp[i] == '^'
			) == false)
		{
			tmp[i] = '_';
		}
	}
	
	if (tmp.length() > 20)
	{
		tmp.erase(20, (tmp.length() - 20));
		tmp.append("...");
	}

	return tmp;
}

std::string MenuV2::GetHitboxName(int hitbox)
{
	static std::vector<std::string> names;

	if (names.size() == 0)
	{
		names.push_back(std::string(xorstr("HEAD")));
		names.push_back(std::string(xorstr("NECK")));
		names.push_back(std::string(xorstr("LOWER_NECK")));
		names.push_back(std::string(xorstr("PELVIS")));
		names.push_back(std::string(xorstr("STOMACH")));
		names.push_back(std::string(xorstr("LOWER_CHEST")));		
		names.push_back(std::string(xorstr("CHEST")));
		names.push_back(std::string(xorstr("UPPER_CHEST")));
		names.push_back(std::string(xorstr("LEFT_THIGH")));
		names.push_back(std::string(xorstr("RIGHT_THIGH")));
		names.push_back(std::string(xorstr("LEFT_SHIN")));
		names.push_back(std::string(xorstr("RIGHT_SHIN")));
		names.push_back(std::string(xorstr("LEFT_FOOT")));
		names.push_back(std::string(xorstr("RIGHT_FOOT")));
		names.push_back(std::string(xorstr("LEFT_HAND")));
		names.push_back(std::string(xorstr("RIGHT_HAND")));
		names.push_back(std::string(xorstr("LEFT_UPPER_ARM")));
		names.push_back(std::string(xorstr("LEFT_LOWER_ARM")));
		names.push_back(std::string(xorstr("RIGHT_UPPER_ARM")));
		names.push_back(std::string(xorstr("RIGHT_LOWER_ARM")));	
	}

	return names[hitbox];
}

void MenuV2::Draw()
{
	ImGuiIO& io = ImGui::GetIO();

	io.MousePos.x = (float)render->mouse.x;
	io.MousePos.y = (float)render->mouse.y;
	io.MouseDrawCursor = cfg.showMenu;

	ImGui_ImplDX9_NewFrame();

	static bool switchMouse = false;

	if (cfg.showMenu)
	{
		if (!switchMouse)
		{
			I::pEngineClient->ClientCmd_Unrestricted(xorstr("cl_mouseenable 0"));
			switchMouse = true;
		}
	}
	else
	{
		if (switchMouse)
		{
			I::pEngineClient->ClientCmd_Unrestricted(xorstr("cl_mouseenable 1"));
			switchMouse = false;
		}
	}

	if (cfg.Overlay.spectators)
	{
		const int INVALID_EHANDLE_INDEX = 0xFFFFFFFF;

		if (ImGui::Begin(MENU_TRANSLATE("Espectadores", "SPECTATORS:"), &cfg.Overlay.spectators, ImVec2(300, 100), 0.5f, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text(MENU_TRANSLATE("Espectadores", "SPECTATORS:"));

			int max = I::pEngineClient->GetMaxClients();

			for (int i = 0; i < max; i++)
			{
				CBaseEntity *entity = I::pClientEntityList->GetClientEntity(i);

				if (NULL == entity) continue;
				if (entity->IsDormant()) continue;

				if (entity->GetClientClass()->m_ClassID != _CCSPlayer) continue;

				int sh = entity->GetObserverTarget();

				if (sh == INVALID_EHANDLE_INDEX) continue;

				CBaseEntity *spectating = I::pClientEntityList->GetClientEntityFromHandle(sh);

				if (spectating != NULL && spectating == G::pLocal)
				{
					char name[128];

					player_info_t info;
					if (I::pEngineClient->GetPlayerInfo(entity->GetIndex(), &info))
					{
						std::string tmp = SanitizeName(info.m_szPlayerName);

						ImGui::Text("%s", tmp.c_str());
					}
				}
			}

			ImGui::End();
		}
	}

	if (cfg.showMenu)
	{
		/*static uint32_t cnt = 0;
		float freq = .002f;

		ImVec4 color = ImVec4(
			(sinf(freq*cnt + 0) * 127 + 2) / 255.0f,
			(sinf(freq*cnt + 2) * 127 + 2) / 255.0f,
			(sinf(freq*cnt + 4) * 127 + 2) / 255.0f,
			(230)/255.0f);

		if (cnt++ >= (uint32_t)-1) 
			cnt = 0;
		
		ImGuiStyle& style = ImGui::GetStyle();
		style.Colors[ImGuiCol_WindowBg] = color;*/

		if (ImGui::Begin(xorstr("BLAXED.COM - RECODED VERSION!"), &cfg.showMenu, ImVec2(625, 750), 0.8f, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse))
		{
			vector<string> tabNames;

			tabNames.push_back(MENU_TRANSLATE("RageBot", "RAGEBOT"));
			tabNames.push_back(MENU_TRANSLATE("LegitBot", "LEGITBOT"));
			tabNames.push_back(MENU_TRANSLATE("Visuais", "VISUALS"));			
			tabNames.push_back(MENU_TRANSLATE("Variados", "MISC"));
			tabNames.push_back(MENU_TRANSLATE("PList", "PLIST"));
			tabNames.push_back(MENU_TRANSLATE("Config", "CONFIGS"));			

			static int selectedTab = 0;

			ImGui::Separator();
			
			ImGui::TabLabels(tabNames, selectedTab, true);
			{
				ImGui::Separator();

				ImGui::BeginChild(xorstr("ALL"), ImGui::GetContentRegionAvail(), true);
				{
					switch (selectedTab)
					{
					case 0: DrawRagebotTab(); break;
					case 1: DrawLegitBotTab(); break;
					case 2: DrawVisualTab(); break;					
					case 3: DrawMiscTab(); break;
					case 4: DrawPlayerListTab(); break;
					case 5: DrawConfigTab(); break;
					}

					ImGui::EndChild();
				}
			}

		//	ImGui::End();
		}
	}

	ImGui::Render();
}

void MenuV2::VectorToArray(std::vector<string> vec, char *arr)
{
	int k = 0;
	for (int i = 0; i < (int)vec.size(); i++)
	{
		int len = (int)strlen(vec[i].c_str());

		for (int j = 0; j < len; j++)
			arr[k++] = vec[i].c_str()[j];

		arr[k++] = '\0';
	}
}

void MenuV2::ResetStyle()
{
	ImGuiStyle& style = ImGui::GetStyle();

	style.Colors[ImGuiCol_Text] = ImVec4(RGBA_TO_FLOAT(255, 255, 255, 255));
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(RGBA_TO_FLOAT(170, 170, 170, 255));
	style.Colors[ImGuiCol_WindowBg] = ImVec4(RGBA_TO_FLOAT(10, 10, 10, 255));
	style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(RGBA_TO_FLOAT(20, 20, 20, 255));
	style.Colors[ImGuiCol_Border] = ImVec4(RGBA_TO_FLOAT(0, 0, 0, 255));
	style.Colors[ImGuiCol_FrameBg] = ImVec4(RGBA_TO_FLOAT(0, 0, 0, 70));
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(RGBA_TO_FLOAT(40, 40, 40, 255));
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(RGBA_TO_FLOAT(10, 10, 10, 255));
	style.Colors[ImGuiCol_TitleBg] = ImVec4(RGBA_TO_FLOAT(0, 0, 0, 230));
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(RGBA_TO_FLOAT(0, 0, 0, 60));
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(RGBA_TO_FLOAT(0, 0, 0, 240));
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(RGBA_TO_FLOAT(50, 50, 50, 255));
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(RGBA_TO_FLOAT(60, 60, 60, 255));
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(RGBA_TO_FLOAT(100, 100, 100, 255));
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(RGBA_TO_FLOAT(70, 70, 70, 255));
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(RGBA_TO_FLOAT(110, 110, 110, 255));
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(RGBA_TO_FLOAT(100, 100, 100, 180));
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(RGBA_TO_FLOAT(110, 110, 110, 255));
	style.Colors[ImGuiCol_Button] = ImVec4(RGBA_TO_FLOAT(60, 60, 60, 255));
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(RGBA_TO_FLOAT(0, 30, 48, 255));
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(RGBA_TO_FLOAT(0, 30, 48, 255));
	style.Colors[ImGuiCol_Header] = ImVec4(RGBA_TO_FLOAT(100, 100, 100, 255));
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(RGBA_TO_FLOAT(100, 100, 100, 170));
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(RGBA_TO_FLOAT(50, 50, 50, 255));
	style.Colors[ImGuiCol_Column] = ImVec4(RGBA_TO_FLOAT(10, 10, 10, 255));
	style.Colors[ImGuiCol_ColumnHovered] = ImVec4(RGBA_TO_FLOAT(10, 10, 10, 255));
	style.Colors[ImGuiCol_ColumnActive] = ImVec4(RGBA_TO_FLOAT(10, 10, 10, 255));
	//style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(RGBA_TO_FLOAT(20, 20, 20, 255));

	style.WindowRounding = 13.0f;
	style.FramePadding = ImVec2(15, 5);
	style.ScrollbarSize = 15.0f;
	style.ScrollbarRounding = 13.0f;
	style.GrabMinSize = 5.0f;	
	style.ChildWindowRounding = 13.0f;
	style.FrameRounding = 3.0f;

	ImGuiIO& io = ImGui::GetIO();

	char dir[MAX_PATH];
	GetWindowsDirectory(dir, MAX_PATH);

	strcat(dir, xorstr("\\Fonts\\verdana.ttf"));

	io.Fonts->AddFontFromFileTTF(dir, 13);
	io.FontAllowUserScaling = false;
	io.Fonts->Build();
}

void MenuV2::GUI_Init()
{
	ImGui_ImplDX9_Init(render->window, render->pDevice);

	ResetStyle();

	G::GUIInited = true;
}

void MenuV2::replace(std::string &string, const std::string& search, const std::string& replace)
{
	unsigned int rLen = replace.length();

	while (1)
	{
		int pos = string.find(search);
		unsigned int len = pos + rLen < string.length() ? rLen : string.length() - (pos + rLen);

		if (pos != -1)
		{
			string.replace(pos, len, replace);
		}
		else
		{
			break;
		}
	}
}