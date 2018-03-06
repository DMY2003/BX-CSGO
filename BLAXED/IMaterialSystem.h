#pragma once

#include "KeyValues.h"

#define TEXTURE_GROUP_LIGHTMAP						"Lightmaps"
#define TEXTURE_GROUP_WORLD							"World textures"
#define TEXTURE_GROUP_MODEL							"Model textures"
#define TEXTURE_GROUP_VGUI							"VGUI textures"
#define TEXTURE_GROUP_PARTICLE						"Particle textures"
#define TEXTURE_GROUP_DECAL							"Decal textures"
#define TEXTURE_GROUP_SKYBOX						"SkyBox textures"
#define TEXTURE_GROUP_CLIENT_EFFECTS				"ClientEffect textures"
#define TEXTURE_GROUP_OTHER							"Other textures"
#define TEXTURE_GROUP_PRECACHED						"Precached"				// TODO: assign texture groups to the precached materials
#define TEXTURE_GROUP_CUBE_MAP						"CubeMap textures"
#define TEXTURE_GROUP_RENDER_TARGET					"RenderTargets"
#define TEXTURE_GROUP_UNACCOUNTED					"Unaccounted textures"	// Textures that weren't assigned a texture group.
//#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER		"Static Vertex"
#define TEXTURE_GROUP_STATIC_INDEX_BUFFER			"Static Indices"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_DISP		"Displacement Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_COLOR	"Lighting Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_WORLD	"World Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_MODELS	"Model Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_OTHER	"Other Verts"
#define TEXTURE_GROUP_DYNAMIC_INDEX_BUFFER			"Dynamic Indices"
#define TEXTURE_GROUP_DYNAMIC_VERTEX_BUFFER			"Dynamic Verts"
#define TEXTURE_GROUP_DEPTH_BUFFER					"DepthBuffer"
#define TEXTURE_GROUP_VIEW_MODEL					"ViewModel"
#define TEXTURE_GROUP_PIXEL_SHADERS					"Pixel Shaders"
#define TEXTURE_GROUP_VERTEX_SHADERS				"Vertex Shaders"
#define TEXTURE_GROUP_RENDER_TARGET_SURFACE			"RenderTarget Surfaces"
#define TEXTURE_GROUP_MORPH_TARGETS					"Morph Targets"

typedef unsigned short MaterialHandle_t;

typedef void(__thiscall* InitKeyValuesFn)(void* ths, const char* name);
typedef void(__thiscall* LoadFromBufferFn)(void* ths, const char* resourceName, const char* pBuffer, void* pFileSystem, const char* pPathID, void* pfnEvaluateSymbolProc);

void InitKeyValues(KeyValues* pKeyValues, const char* name);
void LoadFromBuffer(KeyValues* pKeyValues, const char* resourceName, const char* pBuffer, void* pFileSystem = nullptr, const char* pPathID = NULL, void* pfnEvaluateSymbolProc = nullptr);

typedef void* (*CreateInterfaceFn)(const char* pName, int* pReturnCode);

enum InitReturnVal_t
{
	INIT_FAILED = 0,
	INIT_OK,

	INIT_LAST_VAL,
};

enum AppSystemTier_t
{
	APP_SYSTEM_TIER0 = 0,
	APP_SYSTEM_TIER1,
	APP_SYSTEM_TIER2,
	APP_SYSTEM_TIER3,

	APP_SYSTEM_TIER_OTHER,
};

struct AppSystemInfo_t
{
	const char* m_pModuleName;
	const char* m_pInterfaceName;
};

class IAppSystem
{
public:
	// Here's where the app systems get to learn about each other 
	virtual bool Connect(CreateInterfaceFn factory) = 0; // 0
	virtual void Disconnect() = 0; // 1

								   // Here's where systems can access other interfaces implemented by this object
								   // Returns NULL if it doesn't implement the requested interface
	virtual void* QueryInterface(const char* pInterfaceName) = 0; // 2

																  // Init, shutdown
	virtual InitReturnVal_t Init() = 0; // 3
	virtual void Shutdown() = 0; // 4

								 // Returns all dependent libraries
	virtual const AppSystemInfo_t* GetDependencies() = 0; // 5

														  // Returns the tier
	virtual AppSystemTier_t GetTier() = 0; // 6

										   // Reconnect to a particular interface
	virtual void Reconnect(CreateInterfaceFn factory, const char* pInterfaceName) = 0; // 7

																					   // New virtual function added in the 26 MAY 2015 update
	virtual void UnkFunc() = 0; // 8
};


class IMaterialSystem : IAppSystem
{
public:
	
	IMaterial* FindMaterial(char const* pMaterialName, const char *pTextureGroupName, bool complain = true, const char *pComplainPrefix = NULL)
	{
		typedef IMaterial*(__thiscall* OriginalFn)(void*, char const* pMaterialName, const char *pTextureGroupName, bool complain, const char *pComplainPrefix);
		return Mem::CallVirtualFunction<OriginalFn>(this, 84)(this, pMaterialName, pTextureGroupName, complain, pComplainPrefix);
	}
	
	IMaterial *CreateMaterial(bool shouldIgnoreZ, bool isLit, bool isWireframe, std::string matdata="");

	MaterialHandle_t FirstMaterial()
	{
		typedef MaterialHandle_t(__thiscall *oFirstMaterial)(void*);
		return Mem::CallVirtualFunction<oFirstMaterial>(this, 86)(this);
	}

	MaterialHandle_t NextMaterial(MaterialHandle_t h)
	{
		typedef MaterialHandle_t(__thiscall *oNextMaterial)(void*, MaterialHandle_t);
		return Mem::CallVirtualFunction<oNextMaterial>(this, 87)(this, h);
	}

	MaterialHandle_t InvalidMaterial()
	{
		typedef MaterialHandle_t(__thiscall *oInvalidMaterial)(void*);
		return Mem::CallVirtualFunction<oInvalidMaterial>(this, 88)(this);
	}

	IMaterial* GetMaterial(MaterialHandle_t h)
	{
		typedef IMaterial* (__thiscall *oGetMaterial)(void*, MaterialHandle_t);
		return Mem::CallVirtualFunction<oGetMaterial>(this, 89)(this, h);
	}
};
