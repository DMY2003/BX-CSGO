#include "SDK.h"
#include "IMaterialSystem.h"

void InitKeyValues(KeyValues* pKeyValues, const char* name)
{
	static InitKeyValuesFn InitKeyValuesEx = NULL;

	if (!InitKeyValuesEx)
	{
		DWORD addr = (DWORD)(Mem::FindPattern(xorstr("client.dll"), xorstr("68 ?? ?? ?? ?? 8B C8 E8 ?? ?? ?? ?? 89 45 FC EB 07 C7 45 ?? ?? ?? ?? ?? 8B 03 56")) + 7);
		InitKeyValuesEx = (InitKeyValuesFn)(addr + *(DWORD*)(addr + 1) + 5);

		//InitKeyValuesEx = (InitKeyValuesFn)Mem::FindPattern(xorstr("client.dll"), xorstr("55 8B EC 51 33 C0 C7 45"));
	}

	InitKeyValuesEx(pKeyValues, name);
}

void LoadFromBuffer(KeyValues* pKeyValues, const char* resourceName, const char* pBuffer, void* pFileSystem, const char* pPathID, void* pfnEvaluateSymbolProc)
{
	static LoadFromBufferFn LoadFromBufferEx = NULL;

	if (!LoadFromBufferEx)
	{
		//DWORD addr = (DWORD)Mem::FindPattern(xorstr("client.dll"), xorstr("55 8B EC 83 EC 48 53 56 57 8B F9 89 7D F4"));
		DWORD addr = (DWORD)Mem::FindPattern(xorstr("client.dll"), xorstr("55 8B EC 83 E4 F8 83 EC 34 53 8B 5D 0C 89 4C 24 04"));		
		LoadFromBufferEx = (LoadFromBufferFn)(addr);
	}

	LoadFromBufferEx(pKeyValues, resourceName, pBuffer, pFileSystem, pPathID, pfnEvaluateSymbolProc);
}

int created = 0;

IMaterial* IMaterialSystem::CreateMaterial(bool shouldIgnoreZ, bool isLit, bool isWireframe, std::string matdata)
{
	std::string type = (!isLit) ? xorstr("UnlitGeneric") : xorstr("VertexLitGeneric");

	//std::string matdata = "\"" + type + "\"\n{\n\t\"$basetexture\" \"vgui/white_additive\"\n\t\"$envmap\"  \"\"\n\t\"$model\" \"1\"\n\t\"$flat\" \"1\"\n\t\"$nocull\"  \"0\"\n\t\"$selfillum\" \"1\"\n\t\"$halflambert\" \"1\"\n\t\"$nofog\"  \"0\"\n\t\"$znearer\" \"0\"\n\t\"$wireframe\" \"" + std::to_string(isWireframe) + "\"\n\t\"$ignorez\" \"" + std::to_string(shouldIgnoreZ) + "\"\n}\n";

	if (matdata == "")
	{
		matdata += xorstr("\"") + type + xorstr("\"\n{\n");
		matdata += xorstr("\t\"$basetexture\" \"vgui/white_additive\"\n");
		matdata += xorstr("\t\"$envmap\" \"\"\n");
		matdata += xorstr("\t\"$model\" \"1\"\n");
		matdata += xorstr("\t\"$flat\" \"1\"\n");
		matdata += xorstr("\t\"$nocull\" \"0\"\n");
		matdata += xorstr("\t\"$selfillum\" \"1\"\n");
		matdata += xorstr("\t\"$halflambert\" \"1\"\n");
		matdata += xorstr("\t\"$znearer\" \"0\"\n");
		matdata += xorstr("\t\"$wireframe\" \"") + std::to_string(isWireframe) + xorstr("\"\n");
		matdata += xorstr("\t\"$ignorez\" \"") + std::to_string(shouldIgnoreZ) + xorstr("\"\n");
		matdata += xorstr("}\n");
	}

	//printf(matdata.c_str());

	std::string matname = xorstr("ctmlg_") + std::to_string(rand() % 30) + std::to_string(created) + xorstr(".vmt");
	++created;

	/*KeyValues* pKeyValues = new KeyValues(matname.c_str());
	InitKeyValues(pKeyValues, type.c_str());*/

	KeyValues* pKeyValues = reinterpret_cast<KeyValues*>(KeyValues::operator new(36));
	pKeyValues->InitKeyValues(matname.c_str());
	LoadFromBuffer(pKeyValues, matname.c_str(), matdata.c_str());

	typedef IMaterial*(__thiscall* OriginalFn)(void*, const char *pMaterialName, KeyValues *pVMTKeyValues);
	IMaterial* createdMaterial = Mem::CallVirtualFunction<OriginalFn>(this, 83)(this, matname.c_str(), pKeyValues);

	createdMaterial->IncrementReferenceCount();
	
	return createdMaterial;
}
