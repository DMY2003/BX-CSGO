#include "SDK.h"
#include "Global.h"
#include "Chams.h"
#include "ESP.h"

#define RGB_TO_COLOR_ARRAY(_array, r, g, b) _array[0] = r / 255.0f; _array[1] = g / 255.0f; _array[2] = b / 255.0f;

namespace Chams
{
	bool clean = true;
	bool materialsCreated = false;
	bool enablePlayersDraw = true;

	IMaterial* ignorezLit = NULL;
	IMaterial* ignorezLitWireframe = NULL;
	IMaterial* lit = NULL;
	IMaterial* litWireframe = NULL;
	IMaterial* flat = NULL;
	IMaterial* ignorezFlat = NULL;
	IMaterial* cubeMap = NULL;
	IMaterial* cubeMapRed = NULL;
	
	IMaterial* matShowingEnemy = NULL;
	IMaterial* matHiddenEnemy = NULL;
	IMaterial* matShowingTeam = NULL;
	IMaterial* matHiddenTeam = NULL;
	IMaterial* matWeapon = NULL;
	IMaterial* matBlack = NULL;
	IMaterial* matWhite = NULL;

	float colorHiddenEnemy[3], colorHiddenTeam[3], colorShowingEnemy[3], colorShowingTeam[3], colorShowingLocalPlayer[3];
	float colorWhite[3], colorDoctor[3], colorBlack[3], colorGlow[3];

	bool execute = true;
	bool dmeClean = true;
	bool frsClean = true;

	void CreateMaterials()
	{
		if (materialsCreated)
			return;

		RGB_TO_COLOR_ARRAY(colorHiddenEnemy, 214, 255, 254);
		//	RGB_TO_COLOR_ARRAY(colorShowingEnemy, 0, 255, 242);
		RGB_TO_COLOR_ARRAY(colorShowingEnemy, 255, 67, 0);
		
		RGB_TO_COLOR_ARRAY(colorHiddenTeam, 255, 214, 246);
		RGB_TO_COLOR_ARRAY(colorShowingTeam, 175, 228, 255);
		RGB_TO_COLOR_ARRAY(colorShowingLocalPlayer, 255, 59, 0);

		RGB_TO_COLOR_ARRAY(colorWhite, 255, 255, 255);
		RGB_TO_COLOR_ARRAY(colorDoctor, 117, 164, 239);

		RGB_TO_COLOR_ARRAY(colorBlack, 0, 0, 0);

		RGB_TO_COLOR_ARRAY(colorGlow, 150, 0, 0);

		ignorezLit = I::pMaterialSystem->CreateMaterial(true, true, false);
		ignorezLitWireframe = I::pMaterialSystem->CreateMaterial(true, true, true);
		lit = I::pMaterialSystem->CreateMaterial(false, true, false);
		litWireframe = I::pMaterialSystem->CreateMaterial(false, true, true);
		flat = I::pMaterialSystem->CreateMaterial(false, false, false);
		ignorezFlat = I::pMaterialSystem->CreateMaterial(true, false, false);

		matShowingEnemy = I::pMaterialSystem->CreateMaterial(false, true, false);
		matHiddenEnemy = I::pMaterialSystem->CreateMaterial(true, true, false);
		matShowingTeam = I::pMaterialSystem->CreateMaterial(false, true, false);
		matHiddenTeam = I::pMaterialSystem->CreateMaterial(true, true, false);
		matWeapon = I::pMaterialSystem->CreateMaterial(false, true, false);
		matBlack = I::pMaterialSystem->CreateMaterial(false, true, false);
		matWhite = I::pMaterialSystem->CreateMaterial(false, true, false);

		{
			string matdata = "";
			matdata += xorstr("\"UnlitGeneric\"\n{\n");
			matdata += xorstr("\t\"$envmap\"     \"editor/cube_vertigo\"\n");
			matdata += xorstr("\t\"$envmaptint\" \"[1 1 1]\"\n");
			matdata += xorstr("\t\"$model\"      \"1\"\n");
			matdata += xorstr("\t\"$additive\"   \"1\"\n");
			matdata += xorstr("}");

			cubeMap = I::pMaterialSystem->CreateMaterial(false, true, false, matdata);
		}

		{
			string matdata = "";
			matdata += xorstr("\"UnlitGeneric\"\n{\n");
			matdata += xorstr("\t\"$envmap\"     \"editor/cube_vertigo\"\n");
			matdata += xorstr("\t\"$envmaptint\" \"[0.9 0.9 0.9]\"\n");
			matdata += xorstr("\t\"$model\"      \"1\"\n");
			matdata += xorstr("\t\"$additive\"   \"1\"\n");
			matdata += xorstr("}");

			cubeMapRed = I::pMaterialSystem->CreateMaterial(false, true, false, matdata);
		}

		materialsCreated = true;
	}

	Color Rainbow()
	{
		static uint32_t cnt = 0;
		float freq = .001f;

		Color color = Color(
			sinf(freq*cnt + 0) * 127 + 128,
			sinf(freq*cnt + 2) * 127 + 128,
			sinf(freq*cnt + 4) * 127 + 128,
			255);

		// Probably redundant
		if (cnt++ >= (uint32_t)-1) cnt = 0;

		return color;
	}

	Color BlueTransition()
	{
		static uint32_t cnt = 0;
		float freq = .00001f;

		Color color = Color(
			70,//sinf(freq*cnt + 0) * 127 + 128,
			70,//sinf(freq*cnt + 2) * 127 + 128,
			fabsf(sinf(freq*cnt + 4)) * (200-110) + 110,
			255);

		// Probably redundant
		if (cnt++ >= (uint32_t)-1) cnt = 0;

		return color;
	}

	void DrawColoredModel(DrawModelParams p, IMaterial *pMaterialHidden, IMaterial *pMaterialShowing, Color colorHidden, Color colorShowing)
	{
		if (pMaterialHidden)
		{
			pMaterialHidden->ColorModulate(colorHidden.r() / 255.f, colorHidden.g() / 255.f, colorHidden.b() / 255.f);
			pMaterialHidden->AlphaModulate(colorHidden.a() / 255.f);

			I::pModelRender->ForcedMaterialOverride(pMaterialHidden);

			p.oDrawModel(p._this, p.results, p.info, p.pBoneToWorld, p.pFlexWeights, p.pFlexDelayedWeights, p.origin, p.drawFlags);
		}

		if (pMaterialShowing)
		{
			pMaterialShowing->ColorModulate(colorShowing.r() / 255.f, colorShowing.g() / 255.f, colorShowing.b() / 255.f);
			pMaterialShowing->AlphaModulate(colorShowing.a() / 255.f);

			I::pModelRender->ForcedMaterialOverride(pMaterialShowing);

			p.oDrawModel(p._this, p.results, p.info, p.pBoneToWorld, p.pFlexWeights, p.pFlexDelayedWeights, p.origin, p.drawFlags);
		}
	}

	void DrawColoredModel(CBaseEntity *entity, IMaterial *pMaterialHidden, IMaterial *pMaterialShowing, Color colorHidden, Color colorShowing)
	{
		if (pMaterialHidden)
		{
			pMaterialHidden->ColorModulate(colorHidden.r() / 255.f, colorHidden.g() / 255.f, colorHidden.b() / 255.f);
			pMaterialHidden->AlphaModulate(colorHidden.a() / 255.f);

			I::pModelRender->ForcedMaterialOverride(pMaterialHidden);

			entity->DrawModel(0x1, colorHidden.a());
		}

		if (pMaterialShowing)
		{
			pMaterialShowing->ColorModulate(colorShowing.r() / 255.f, colorShowing.g() / 255.f, colorShowing.b() / 255.f);
			pMaterialShowing->AlphaModulate(colorShowing.a() / 255.f);

			I::pModelRender->ForcedMaterialOverride(pMaterialShowing);

			entity->DrawModel(0x1, colorShowing.a());
		}
	}

	bool DrawModel(DrawModel_t oDrawModel, void *_this, void* results, DrawModelInfo_t& info, matrix3x4_t* pBoneToWorld, float* pFlexWeights, float* pFlexDelayedWeights, Vector& origin, int drawFlags)
	{
		DrawModelParams p = DrawModelParams{ oDrawModel, (DWORD*)_this, (DWORD*)results, info, pBoneToWorld, pFlexWeights, pFlexDelayedWeights, origin, drawFlags };

		if (!execute)
		{
			dmeClean = true;
			return false;
		}

		dmeClean = false;

		CreateMaterials();

		CBaseEntity *entity = (CBaseEntity*)((DWORD)info.m_pClientEntity - 4);
		CBaseEntity *pLocal = G::pLocal;
		studiohdr_t* hdr = info.m_pStudioHdr;

		if (!entity || !hdr || !pLocal)
			return false;

		string modelName = hdr->name;

		if (modelName.find(xorstr("shadow")) != -1)
			return false;

		if (modelName.find(xorstr("player")) != -1 && entity->GetClientClass()->m_ClassID == _CCSPlayer)
		{
			if (entity == pLocal)
			{				
				if (I::pInput->m_fCameraInThirdPerson && pLocal->IsAlive())
				{					
					if (G::scoped)
					{
						DrawColoredModel(p, NULL, lit, Color(0, 0, 0, 0), Color(255, 255, 255, 50));
					}
					else if (cfg.Visual.localPlayerVisuals)
					{
						DrawColoredModel(p, NULL, lit, Color(0, 0, 0, 0), Color(77, 94, 239));
					}

					if (cfg.Rage.AntiAim.viewLowerBodyYawTarget)
					{
						I::pModelRender->ForcedMaterialOverride(lit);
						lit->AlphaModulate(1);
						lit->ColorModulate(1, 1, 1);
						p.oDrawModel(p._this, p.results, p.info, G::localPlayerLbyBones, p.pFlexWeights, p.pFlexDelayedWeights, p.origin, p.drawFlags);
					}

					return G::scoped || cfg.Visual.localPlayerVisuals || cfg.Rage.AntiAim.viewLowerBodyYawTarget;
				}
			}
			else
			{
				bool enemy = entity->GetTeam() != G::pLocal->GetTeam() || cfg.Aimbot.friendlyFire;

				if (enemy && entity != G::pLocal)
				{
					if (!ESP::Filter(entity))
					{
						if (cfg.Visual.wallChams || cfg.Visual.chams)
						{
							// doing sceneend drawing
							if (!enablePlayersDraw)
								return true;
						}
					}
				}
			}
		}
		else if (modelName.find(xorstr("v_glove")) != -1)
		{
			if (cfg.Visual.handChams)
			{
				if (cfg.Visual.glassHands)
				{
					DrawColoredModel(p, NULL, cubeMapRed, Color(0, 0, 0, 0), Color(255, 255, 255, 255));
					return true;
				}
				else if (cfg.Visual.darkHandChams)
				{
					DrawColoredModel(p, NULL, lit, Color(0, 0, 0, 0), Color(10, 10, 10, 255));
					return true;
				}
				else
				{
					DrawColoredModel(p, lit, cubeMapRed, Color(0, 0, 0, 255), Color(255, 255, 255, 255));
					return true;
				}
			}
		}
		else if (modelName.find(xorstr("arms")) != -1)
		{
			if (cfg.Visual.handChams)
			{
				//DrawColoredModel(p, NULL, litWireframe, Color(0, 0, 0, 0), Color(0, 255, 0, 255));
				//return true;
			}
		}
		else if (modelName.find(xorstr("_dropped")) != -1)
		{
			if (cfg.Visual.chams)
			{
				DrawColoredModel(p, NULL, lit, Color(0, 0, 0, 0), Color(255, 255, 255, 255));
				return true;
			}
		}
		else if (modelName.find(xorstr("weapon")) != -1)
		{
			if (cfg.Visual.handChams)
			{
				if (entity != I::pClientEntityList->GetClientEntityFromHandle(G::pLocal->GetViewModel()))
				{
					DrawColoredModel(p, NULL, lit, Color(0, 0, 0, 0), Color(255, 255, 255, 255));
					return true;
				}				
			}
		}

		return false;
	}

	bool DoPlayersChams(CBaseEntity *entity)
	{
		CreateMaterials();

		bool enemy = entity->GetTeam() != G::pLocal->GetTeam() || cfg.Aimbot.friendlyFire;

		if (enemy && entity != G::pLocal)
		{
			if (!ESP::Filter(entity))
			{
				if (cfg.Visual.wallChams && cfg.Visual.chams)
				{
					if (cfg.Visual.hvhChams)
					{
						DrawColoredModel(entity, ignorezLit, lit, cfg.Visual.colorChamsHidden, enemy ? cfg.Visual.colorChamsVisible : Color(10, 50, 255, 255));
						return true;
					}
					else
					{
						DrawColoredModel(entity, ignorezLit, lit, Color(0, 0, 0, 170), Color(30, 30, 210, 255));
						return true;
					}
				}
				else if (cfg.Visual.chams)
				{
					if (cfg.Visual.hvhChams)
					{
						DrawColoredModel(entity, NULL, lit, Color(0, 0, 0, 0), enemy ? cfg.Visual.colorChamsVisible : Color(10, 50, 255, 255));
						return true;
					}
					else
					{
						DrawColoredModel(entity, NULL, lit, Color(0, 0, 0, 0), Color(30, 30, 210, 255));
						return true;
					}
				}
			}
		}

		return false;
	}

	bool DrawModelExecute(DrawModelExecute_t oDrawModelExecute, void* context, void* state, ModelRenderInfo_t& info, matrix3x4_t* pCustomBoneToWorld)
	{
		return false;
	}
};
