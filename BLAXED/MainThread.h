#pragma once

#include "SDK.h"

DWORD WINAPI MainThread(LPVOID lpParam);

void XorSensitiveBlocks();

void Unload();
