#pragma once

#include "main.h"
#include "Vector.h"

#define INRANGE(x,a,b)    (x >= a && x <= b) 
#define getBits( x )    (INRANGE((x&(~0x20)),'A','F') ? ((x&(~0x20)) - 'A' + 0xa) : (INRANGE(x,'0','9') ? x - '0' : 0))
#define getByte( x )    (getBits(x[0]) << 4 | getBits(x[1]))

#define M_PI		3.14159265358979323846f
#define M_RADPI		57.295779513082f
#define M_PI_F		((float)(M_PI))	// Shouldn't collide with anything.
#define RAD2DEG( x  )  ( (float)(x) * (float)(180.f / M_PI_F) )
#define DEG2RAD( x  )  ( (float)(x) * (float)(M_PI_F / 180.f) )

namespace Mem
{
	template <typename Fn> __forceinline Fn CallVirtualFunction(void* pClassBase, int nFunctionIndex) {
		return (Fn)((PDWORD)*(PDWORD*)pClassBase)[nFunctionIndex];
	}
	inline std::uint8_t *PatternScan(void* module, const char* signature)
	{

		static auto pattern_to_byte = [](const char* pattern) {
			auto bytes = std::vector<int>{};
			auto start = const_cast<char*>(pattern);
			auto end = const_cast<char*>(pattern) + strlen(pattern);

			for (auto current = start; current < end; ++current) {
				if (*current == '?') {
					++current;
					if (*current == '?')
						++current;
					bytes.push_back(-1);
				}
				else {
					bytes.push_back(strtoul(current, &current, 16));
				}
			}
			return bytes;
		};

		auto dosHeader = (PIMAGE_DOS_HEADER)module;
		auto ntHeaders = (PIMAGE_NT_HEADERS)((std::uint8_t*)module + dosHeader->e_lfanew);

		auto sizeOfImage = ntHeaders->OptionalHeader.SizeOfImage;
		auto patternBytes = pattern_to_byte(signature);
		auto scanBytes = reinterpret_cast<std::uint8_t*>(module);

		auto s = patternBytes.size();
		auto d = patternBytes.data();

		for (auto i = 0ul; i < sizeOfImage - s; ++i) {
			bool found = true;
			for (auto j = 0ul; j < s; ++j) {
				if (scanBytes[i + j] != d[j] && d[j] != -1) {
					found = false;
					break;
				}
			}
			if (found) {
				return &scanBytes[i];
			}
		}
		return nullptr;
	}
	inline uintptr_t FindMemoryPattern(HANDLE ModuleHandle, char* strpattern, int length)
	{
		//double startfilter = QPCTime();
		//Filter out junk and get a clean hex version of the signature
		unsigned char *signature = new unsigned char[length + 1];
		bool *skippable = new bool[length + 1];
		int signaturelength = 0;
		for (int byteoffset = 0; byteoffset < length - 1; byteoffset += 2)
		{
			char charhex[4]; //4 to keep sscanf happy
			*(short*)charhex = *(short*)&strpattern[byteoffset];
			if (charhex[0] != ' ')
			{
				if (charhex[0] == '?')
				{
					signature[signaturelength] = '?';
					skippable[signaturelength] = true;
				}
				else
				{
					//Convert ascii to hex
					charhex[2] = NULL; //add null terminator
					signature[signaturelength] = (unsigned char)std::stoul(charhex, nullptr, 16);

					//sscanf(charhex, "%x", &signature[signaturelength]);
					skippable[signaturelength] = false;
				}
				signaturelength++;
			}
		}
		//double timetakentofilter = QPCTime() - startfilter;

		//Search for the hex signature in memory
		int searchoffset = 0;
		int maxoffset = signaturelength - 1;
		//double timetakentosearch = 0.0;
		//double startsearch = QPCTime();
#if 0
		uintptr_t adr = (uintptr_t)ModuleHandle;
		while (1)
		{
			try {
				unsigned char hextosearch = signature[searchoffset];
				bool skip = hextosearch == '?';
				if (ReadByte(adr) == hextosearch || skip)
				{
					searchoffset++;
					if (searchoffset > maxoffset)
					{
						delete[] signature;
						//timetakentosearch = QPCTime() - startsearch;
						return adr - maxoffset; //FOUND OFFSET!
					}
				}
				else
				{
					searchoffset = 0;
				}
			}
			catch (...) {
				break;
			}
			adr++;
		}
#else
		MODULEINFO dllinfo;
		GetModuleInformation(GetCurrentProcess(), (HMODULE)ModuleHandle, &dllinfo, sizeof(MODULEINFO));
		DWORD endadr = (DWORD)ModuleHandle + dllinfo.SizeOfImage;
		DWORD adrafterfirstmatch = NULL;
		for (DWORD adr = (DWORD)ModuleHandle; adr < endadr; adr++)
		{
			//if (adr == ((DWORD)ModuleHandle + 0x43F57C))
			//	printf("test");

			if (skippable[searchoffset] || *(char*)adr == signature[searchoffset] || *(unsigned char*)adr == signature[searchoffset])
			{
				if (searchoffset == 0)
				{
					adrafterfirstmatch = adr + 1;
				}
				searchoffset++;
				if (searchoffset > maxoffset)
				{
					delete[] signature;
					delete[] skippable;
					//timetakentosearch = QPCTime() - startsearch;
					return adr - maxoffset; //FOUND OFFSET!
				}
			}
			else if (adrafterfirstmatch)
			{
				adr = adrafterfirstmatch;
				searchoffset = 0;
				adrafterfirstmatch = NULL;
			}
		}
#endif
		//timetakentosearch = QPCTime() - startsearch;
		delete[] signature;
		delete[] skippable;
		return NULL; //NOT FOUND!
	}
	inline uintptr_t FindPatternByRange(uintptr_t rangeStart, uintptr_t rangeEnd, const char* pattern)
	{
		const char* pat = pattern;

		uintptr_t firstMatch = 0;

		for (uintptr_t pCur = rangeStart; pCur < rangeEnd; pCur++)
		{
			if (!*pat)
				return firstMatch;

			if (*(BYTE*)pCur == getByte(pat) || *(PBYTE)pat == '\?')
			{
				if (!firstMatch)
					firstMatch = pCur;

				if (!pat[2])
					return firstMatch;

				if (*(PWORD)pat == '\?\?' || *(PBYTE)pat != '\?')
					pat += 3;
				else
					pat += 2;
			}
			else
			{
				pat = pattern;
				firstMatch = 0;
			}
		}

		return NULL;
	}
	inline DWORD FindPatternAimbot(char *pattern, char *mask, DWORD offset, char *moduleName)
	{
		HMODULE hModule;
		MODULEINFO moduleInfo;
		DWORD dModule, dModuleSize;

		hModule = GetModuleHandle(moduleName);
		GetModuleInformation(GetCurrentProcess(), hModule, &moduleInfo, sizeof(MODULEINFO));

		dModule = (DWORD)moduleInfo.lpBaseOfDll;
		dModuleSize = moduleInfo.SizeOfImage;

		DWORD start = dModule;
		DWORD end = dModule + dModuleSize;

		int patternLength = strlen(mask);
		bool found = false;

		for (DWORD i = start; i < end - patternLength; i++)
		{
			found = true;

			for (int idx = 0; idx < patternLength; idx++)
			{
				if (*(mask + idx) == 'x' && *(pattern + idx) != *(char*)(i + idx))
				{
					found = false;
					break;
				}
			}

			if (found)
				return i + offset;
		}
		return NULL;
	}
	inline uintptr_t FindPattern(const char* moduleName, const char* pattern)
	{
		uintptr_t firstMatch = 0;
		uintptr_t rangeStart = (uintptr_t)GetModuleHandle(moduleName);

		MODULEINFO miModInfo;

		GetModuleInformation(GetCurrentProcess(), (HMODULE)rangeStart, &miModInfo, sizeof(MODULEINFO));

		uintptr_t rangeEnd = rangeStart + miModInfo.SizeOfImage;

		return FindPatternByRange(rangeStart, rangeEnd, pattern);
	}
}

namespace InterfaceMgr
{
	class InterfaceReg
	{
		typedef void* (*InstantiateInterfaceFn)();

	public:
		InstantiateInterfaceFn	m_CreateFn;
		const char				*m_pName;

		InterfaceReg			*m_pNext; // For the global list.
		static InterfaceReg		*s_pInterfaceRegs;
	};

	template <typename Interface>
	inline Interface CreateInterfaceExact(const char *dll, const char *name)
	{
		if (!GetProcAddress(GetModuleHandle(dll), xorstr("CreateInterface")))
			return NULL;

		return (Interface)(((void *(*)(const char *, void *))GetProcAddress(GetModuleHandle(dll), xorstr("CreateInterface")))(name, 0));
	}

	inline InterfaceReg *GetInterfaceList(char *module)
	{
		void *create_interface = GetProcAddress(GetModuleHandle(module), xorstr("CreateInterface"));

		if (!create_interface)
			return NULL;

		size_t jmp_instruction = (size_t)(create_interface)+4;
		//rva from end of instruction
		size_t jmp_target = jmp_instruction + *(size_t*)(jmp_instruction + 1) + 5;

		InterfaceReg *interface_list = **(InterfaceReg***)(jmp_target + 6);

		return interface_list;
	}

	template <typename Interface>
	inline Interface CreateInterface(char *dll, char *name)
	{
		InterfaceReg *list = GetInterfaceList(dll);

		int partialNameLen = strlen(name);

		while (list)
		{
			if (!strncmp(list->m_pName, name, partialNameLen))
			{
				int version = atoi(list->m_pName + partialNameLen);

				if (version > 0)
				{
					return (Interface)list->m_CreateFn();
				}
			}

			list = list->m_pNext;
		}

		return 0;
	}

	/*void Dump(InterfaceReg *interfaceList)
	{
	while (interfaceList)
	{
	printf("%s %x\n", interfaceList->m_pName, interfaceList->m_CreateFn);
	interfaceList = interfaceList->m_pNext;
	}
	}*/
}

