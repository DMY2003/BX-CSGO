#include "SDK.h"
#include "Global.h"
#include "NightMode.h"
#include "ConvarSpoofer.h"
#include "AutoWall.h"

namespace NightMode
{	
	bool done = false;

	class MaterialBackup
	{
	public:
		MaterialHandle_t handle;
		IMaterial* pMaterial;
		float color[3];
		float alpha;
		bool translucent;
		bool nodraw;

		MaterialBackup()
		{

		}

		MaterialBackup(MaterialHandle_t h, IMaterial* p)
		{
			handle = handle;
			pMaterial = p;

			pMaterial->GetColorModulation(&color[0], &color[1], &color[2]);
			alpha = pMaterial->GetAlphaModulation();

			translucent = pMaterial->GetMaterialVarFlag(MATERIAL_VAR_TRANSLUCENT);
			nodraw = pMaterial->GetMaterialVarFlag(MATERIAL_VAR_NO_DRAW);
		}
	
		void Restore()
		{
			pMaterial->ColorModulate(color[0], color[1], color[2]);
			pMaterial->AlphaModulate(alpha);
			pMaterial->SetMaterialVarFlag(MATERIAL_VAR_TRANSLUCENT, translucent);
			pMaterial->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, nodraw);
		}
	};

	std::vector<MaterialBackup> materials;
	std::vector<MaterialBackup> skyboxes;

	void DoNightMode(MaterialHandle_t i, IMaterial *pMaterial, bool saveBackup = false)
	{
		if (strstr(pMaterial->GetTextureGroupName(), xorstr("Model")))
		{
			if (saveBackup)
				materials.push_back(MaterialBackup(i, pMaterial));

			pMaterial->ColorModulate(0.6f, 0.6f, 0.6f);
		}
		else if (strstr(pMaterial->GetTextureGroupName(), xorstr("World")))
		{
			if (saveBackup)
				materials.push_back(MaterialBackup(i, pMaterial));

			pMaterial->ColorModulate(0.15f, 0.15f, 0.15f);
			//pMaterial->ColorModulate(0.07f, 0.07f, 0.07f);
		}
		else if (strstr(pMaterial->GetTextureGroupName(), xorstr("Static")))
		{
			if (saveBackup)
				materials.push_back(MaterialBackup(i, pMaterial));

			//pMaterial->ColorModulate(0.35f, 0.35f, 0.35f);
			pMaterial->ColorModulate(0.30f, 0.30f, 0.30f);
			//pMaterial->ColorModulate(0.17f, 0.17f, 0.17f);

			if (strstr(pMaterial->GetName(), xorstr("barrel")) || strstr(pMaterial->GetName(), xorstr("dome")))
			{
				pMaterial->SetMaterialVarFlag(MATERIAL_VAR_TRANSLUCENT, true);
				pMaterial->AlphaModulate(0.7f);
			}
			else if (strstr(pMaterial->GetName(), xorstr("door")))
			{
				if (!strstr(pMaterial->GetName(), xorstr("temple")))
				{
					pMaterial->AlphaModulate(0.83f);					
				}
			}
			else if (strstr(pMaterial->GetName(), xorstr("box")) || pMaterial->GetName(), xorstr("crate"))
			{
				pMaterial->AlphaModulate(0.93f);
			}			
		}
		else if (strstr(pMaterial->GetTextureGroupName(), xorstr("SkyBox")))
		{
			if (saveBackup)
				skyboxes.push_back(MaterialBackup(i, pMaterial));

			//pMaterial->ColorModulate(0.09f, 0.11f, 0.13f);
			//pMaterial->ColorModulate(0.29f, 0.31f, 0.33f);
			//pMaterial->ColorModulate(0.18f, 0.18f, 0.18f);
			pMaterial->ColorModulate(0.23f, 0.22f, 0.22f);
		}
	}

	void Apply()
	{
		if (done)
			return;

		if (!ConvarSpoofer::IsReady())
			return;

		done = true;

		//ConvarSpoofer::Get(xorstr("sv_skyname"))->SetString(xorstr("sky_csgo_night02"));
		ConvarSpoofer::Get(xorstr("sv_skyname"))->SetString(xorstr("vertigo"));
		ConvarSpoofer::Get(xorstr("r_drawspecificstaticprop"))->SetValue(0);
		ConvarSpoofer::Get(xorstr("r_dlightsenable"))->SetValue(1);		

		if (materials.size() == 0)
		{
			materials.clear();
			skyboxes.clear();

			for (MaterialHandle_t i = I::pMaterialSystem->FirstMaterial(); i != I::pMaterialSystem->InvalidMaterial(); i = I::pMaterialSystem->NextMaterial(i))
			{
				IMaterial* pMaterial = I::pMaterialSystem->GetMaterial(i);

				if (!pMaterial || pMaterial->IsErrorMaterial())
					continue;

				if (pMaterial->GetReferenceCount() <= 0)
					continue;

				DoNightMode(i, pMaterial, true);				
			}
		}
		else
		{
			for (int i = 0; i < (int)materials.size(); i++)
			{
				DoNightMode(materials[i].handle, materials[i].pMaterial);
			}

			for (int i = 0; i < (int)skyboxes.size(); i++)
			{
				DoNightMode(skyboxes[i].handle, skyboxes[i].pMaterial);
			}
		}
	}

	void Remove()
	{
		if (!done || !ConvarSpoofer::IsReady())
			return;

		done = false;

		for (int i = 0; i < (int)materials.size(); i++)
		{
			if (materials[i].pMaterial->GetReferenceCount() <= 0)
				continue;
			
			materials[i].Restore();
			materials[i].pMaterial->Refresh();
		}

		for (int i = 0; i < (int)skyboxes.size(); i++)
		{
			if (materials[i].pMaterial->GetReferenceCount() <= 0)
				continue;

			skyboxes[i].Restore();
			skyboxes[i].pMaterial->Refresh();
		}

		materials.clear();
		skyboxes.clear();

		cfg.Visual.nightMode = false;

		ConvarSpoofer::Get(xorstr("sv_skyname"))->SetString(I::pCVar->FindVar(xorstr("sv_skyname"))->GetString());
		ConvarSpoofer::Get(xorstr("r_drawspecificstaticprop"))->SetValue(I::pCVar->FindVar(xorstr("r_DrawSpecificStaticProp"))->GetInt());
		ConvarSpoofer::Get(xorstr("fog_override"))->SetValue(I::pCVar->FindVar(xorstr("fog_override"))->GetInt());
		ConvarSpoofer::Get(xorstr("fog_enable"))->SetValue(I::pCVar->FindVar(xorstr("fog_enable"))->GetInt());
		ConvarSpoofer::Get(xorstr("fog_enableskybox"))->SetValue(I::pCVar->FindVar(xorstr("fog_enableskybox"))->GetInt());
		ConvarSpoofer::Get(xorstr("fog_colorskybox"))->SetString(I::pCVar->FindVar(xorstr("fog_colorskybox"))->GetString());
		ConvarSpoofer::Get(xorstr("r_dlightsenable"))->SetValue(I::pCVar->FindVar(xorstr("r_dlightsenable"))->GetInt());
	}
	
	void FrameStageNotify(ClientFrameStage_t stage)
	{
		CBaseEntity *pLocal = G::pLocal;

		if (!(I::pEngineClient->IsInGame() && I::pEngineClient->IsConnected()))
		{
			materials.clear();
			skyboxes.clear();
			return;
		}

		if (stage == FRAME_RENDER_END)
		{
			if (cfg.Visual.nightMode)
				Apply();
			else
				Remove();
		}

		if (stage == FRAME_RENDER_START)
		{			
			if (pLocal && cfg.Visual.flashLight && pLocal->IsAlive())
			{

				Vector f, r, u;
				Vector viewAngles;

				I::pEngineClient->GetViewAngles(viewAngles);
				Math::AngleVectors(viewAngles, &f, &r, &u);

				Vector origin = pLocal->GetAbsOrigin() + pLocal->GetViewOffset();
			}
			else
			{
				
			}
		}
	}

	void HandleGameEvent(IGameEvent *pEvent)
	{
		const char *name = pEvent->GetName();

		if (!strcmp(name, xorstr("round_start")))
		{
			if (cfg.Visual.nightMode)
			{
				Remove();
				Apply();
				cfg.Visual.nightMode = true;
			}
		}
	}
};
