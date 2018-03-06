#include "SDK.h"
#include "Global.h"
#include "WndProc.h"
#include "Render.h"

WNDPROC oldWindowProc = 0;
void OpenMenu();

extern LRESULT ImGui_ImplDX9_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT __stdcall WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{
	if (G::exit) return CallWindowProc(oldWindowProc, hWnd, uMsg, wParam, lParam);

	switch (uMsg) 
	{
	case WM_LBUTTONDOWN:
		Global::pressedKeys[VK_LBUTTON] = true;
		break;
	case WM_LBUTTONUP:
		Global::pressedKeys[VK_LBUTTON] = false;
		break;
	case WM_RBUTTONDOWN:
		Global::pressedKeys[VK_RBUTTON] = true;
		break;
	case WM_RBUTTONUP:
		Global::pressedKeys[VK_RBUTTON] = false;
		break;
	case WM_MBUTTONDOWN:
		Global::pressedKeys[VK_MBUTTON] = true;
		break;
	case WM_MBUTTONUP:
		Global::pressedKeys[VK_MBUTTON] = false;
		break;
	case WM_XBUTTONDOWN:
	{
		UINT button = GET_XBUTTON_WPARAM(wParam);
		if (button == XBUTTON1)
		{
			Global::pressedKeys[VK_XBUTTON1] = true;
		}
		else if (button == XBUTTON2)
		{
			Global::pressedKeys[VK_XBUTTON2] = true;
		}
		break;
	}
	case WM_XBUTTONUP:
	{
		UINT button = GET_XBUTTON_WPARAM(wParam);
		if (button == XBUTTON1)
		{
			Global::pressedKeys[VK_XBUTTON1] = false;
		}
		else if (button == XBUTTON2)
		{
			Global::pressedKeys[VK_XBUTTON2] = false;
		}
		break;
	}
	case WM_KEYDOWN:
		Global::pressedKeys[wParam] = true;
		break;
	case WM_KEYUP:
		Global::pressedKeys[wParam] = false;
		break;
	default: break;
	}

	OpenMenu();

	if (cfg.showMenu && Global::GUIInited && ImGui_ImplDX9_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	return CallWindowProc(oldWindowProc, hWnd, uMsg, wParam, lParam);
}

void OpenMenu()
{
	static bool is_down = false;
	static bool is_clicked = false;

	if (Global::pressedKeys[VK_INSERT])
	{
		is_clicked = false;
		is_down = true;
	}
	else if (!Global::pressedKeys[VK_INSERT] && is_down)
	{
		is_clicked = true;
		is_down = false;
	}
	else
	{
		is_clicked = false;
		is_down = false;
	}

	if (is_clicked)
	{
		cfg.showMenu = !cfg.showMenu;
	}

#ifndef NDEBUG
	if (G::pressedKeys[VK_F11])
	{
		Global::exit = true;		
	}
#endif
}


void SetupWindow()
{
	if (!oldWindowProc)
	{
		oldWindowProc = (WNDPROC)SetWindowLongPtr(render->window, GWLP_WNDPROC, (LONG_PTR)WndProc);
		cfg.showMenu = true;
	}
}

void ResetWindow()
{
	if (oldWindowProc)
	{
		SetWindowLongPtr(render->window, GWLP_WNDPROC, (LONG_PTR)oldWindowProc);
	}
}
