#pragma once
#define _CRT_SECURE_NO_WARNINGS

//#define _PROTECTED_BUILD

#include <Windows.h>
#include <string>
#include <sstream>
#include <process.h>
#include <vector>
#include <list>
#include <TlHelp32.h>
#include <Psapi.h>
#include <algorithm>

#include "XorStr.hpp"

#define SAFE_WAIT(resource, ms, DO_THIS) \
{\
	int wait = 0;\
	while (!resource)\
	{\
		DO_THIS\
		if (resource) break;\
		if (wait > ms)\
		{\
			MessageBoxA(0, xorstr(#resource), xorstr("Failed to get resource"), 0);\
			break;\
		}\
		wait += 10;\
		Sleep(10);\
	}\
}

using namespace std;


