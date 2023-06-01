#include "Chams.h"
#include "../Vars.h"
#include "../../Hooks/Hooks.h"

using namespace Hooks::IVModelRender_DrawModelExecute;

bool CFeatures_Chams::Initialize()
{
	KeyValues* pMatFresnel = new KeyValues("VertexLitGeneric");
	pMatFresnel->SetString("$basetexture", "vgui/white_additive");
	pMatFresnel->SetString("$bumpmap", "models/player/shared/shared_normal");
	pMatFresnel->SetString("$envmap", "skybox/sky_dustbowl_01");
	pMatFresnel->SetString("$envmapfresnel", "1");
	pMatFresnel->SetString("$phong", "1");
	pMatFresnel->SetString("$phongfresnelranges", "[0.0 1.0 6.0]");
	pMatFresnel->SetString("$selfillum", "1");
	pMatFresnel->SetString("$selfillumfresnel", "1");
	pMatFresnel->SetString("$selfillumfresnelminmaxexp", "[0.5 0.5 0]");
	pMatFresnel->SetString("$selfillumtint", "[1 1 1]");
	pMatFresnel->SetString("$envmaptint", "[1 1 1]");
	m_pMatFresnel = I::MaterialSystem->CreateMaterial("ChamsMaterialFresnel", pMatFresnel);

	KeyValues* pMatGlow = new KeyValues("VertexLitGeneric");
	pMatGlow->SetString("$basetexture", "vgui/white_additive");
	pMatGlow->SetString("$bumpmap", "vgui/white_additive");
	pMatGlow->SetString("$color2", "[1 100 1]");
	pMatGlow->SetString("$selfillum", "1");
	pMatGlow->SetString("$selfillumfresnel", "1");
	pMatGlow->SetString("$selfillumfresnelminmaxexp", "[0 0.1 0.1]");
	pMatGlow->SetString("$selfillumtint", "[10 0 2]");
	m_pMatGlow = I::MaterialSystem->CreateMaterial("ChamsMaterialGlow", pMatGlow);

	KeyValues* pMatTest = new KeyValues("VertexLitGeneric");
	pMatTest->SetString("$basetexture", "vgui/white_additive");
	pMatTest->SetString("$bumpmap", "vgui/white_additive");
	pMatTest->SetString("$color2", "[100 1 15]");
	pMatTest->SetString("$selfillum", "1");
	pMatTest->SetString("$selfillumfresnel", "1");
	pMatTest->SetString("$selfillumfresnelminmaxexp", "[0 0.1 1]");
	pMatTest->SetString("$selfillumtint", "[10 10 0.1]");
	m_pMatTest = I::MaterialSystem->CreateMaterial("ChamsMaterialTest", pMatTest);

	KeyValues* pMatShaded = new KeyValues("VertexLitGeneric");
	pMatShaded->SetString("$basetexture", "vgui/white_additive");
	pMatShaded->SetString("$bumpmap", "vgui/white_additive");
	pMatShaded->SetString("$selfillum", "1");
	pMatShaded->SetString("$selfillumfresnel", "1");
	pMatShaded->SetString("$selfillumfresnelminmaxexp", "[0.1 1 2]");
	m_pMatShaded = I::MaterialSystem->CreateMaterial("ChamsMaterialShaded", pMatShaded);

	KeyValues* pMatToxic = new KeyValues("VertexLitGeneric");
	pMatToxic->SetString("$basetexture", "vgui/white_additive");
	pMatToxic->SetString("$bumpmap", "vgui/white_additive");
	pMatToxic->SetString("$color2", "[1 100 1]");
	pMatToxic->SetString("$selfillum", "1");
	pMatToxic->SetString("$selfillumfresnel", "1");
	pMatToxic->SetString("$selfillumfresnelminmaxexp", "[0 0.18 0.1]");
	pMatToxic->SetString("$selfillumtint", "[0.3 0.001 0.1]");
	m_pMatToxic = I::MaterialSystem->CreateMaterial("ChamsMaterialToxic", pMatToxic);

	//Hopefully we get here with no problems!
	return true;
}

bool CFeatures_Chams::Render(void* ecx, void* edx, const DrawModelState_t& state, const ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld)
{
	if (!Vars::Chams::Enabled.m_Var || g_Globals.m_bIsGameUIVisible)
		return false;

	C_TFPlayer* pLocal = G::EntityCache.GetLocal();

	if (!pLocal)
		return false;

	IClientEntity* pEntity = UTIL_EntityByIndex(pInfo.entity_index);

	if (!pEntity || pEntity->IsDormant())
		return false;

	ClientClass* pCC = pEntity->GetClientClass();

	if (!pCC)
		return false;

	switch (pCC->GetTFClientClass())
	{
	case ETFClientClass::CTFViewModel:
	{
		I::ModelRender->ForcedMaterialOverride(m_pMatShaded);

		//I feel this is somewhat inefficient.
		const Color clrTeam = G::Util.GetTeamColor(pLocal->GetTeamNumber());
		const float fColor[3] = { clrTeam.r() / 255.f, clrTeam.g() / 255.f, clrTeam.b() / 255.f };
		I::RenderView->SetColorModulation(fColor);

		Func.Original<FN>()(ecx, edx, state, pInfo, pCustomBoneToWorld);

		I::ModelRender->ForcedMaterialOverride(nullptr);

		const float fClear[3] = { 1.f, 1.f, 1.f };
		I::RenderView->SetColorModulation(fClear);

		return true;
	}

	case ETFClientClass::CTFPlayer:
	{
		C_TFPlayer* pPlayer = pEntity->As<C_TFPlayer*>();

		if (Vars::Chams::Players::IgnoreTeam.m_Var && pPlayer->InLocalTeam())
			return false;

		I::ModelRender->ForcedMaterialOverride([&]() -> IMaterial*
			{
				switch (Vars::Chams::Players::Material.m_Var)
				{
				case 1: return m_pMatFresnel;
				case 2: return m_pMatGlow;
				case 3: return m_pMatTest;
				case 4: return m_pMatShaded;
				case 5: return m_pMatToxic;
				default: return nullptr;
				}
			}());

		if (Vars::Chams::Players::Material.m_Var == 1)
		{
			IMaterialVar* pFresnelRanges = m_pMatFresnel->FindVar("$phongfresnelranges", NULL, false);
			pFresnelRanges->SetVecValue(Vars::Chams::Players::FresnelVars::FresnelX.m_Var, Vars::Chams::Players::FresnelVars::FresnelY.m_Var, Vars::Chams::Players::FresnelVars::FresnelZ.m_Var);

			float fBaseColor[3]; Vars::Chams::Players::FresnelVars::BaseColor.AsFloat(fBaseColor);
			IMaterialVar* pBaseColor = m_pMatFresnel->FindVar("$selfillumtint", NULL, false);
			pBaseColor->SetVecValue(fBaseColor[0], fBaseColor[1], fBaseColor[2]);

			float fGlowColor[3]; Vars::Chams::Players::FresnelVars::GlowColor.AsFloat(fGlowColor);
			IMaterialVar* pGlowColor = m_pMatFresnel->FindVar("$envmaptint", NULL, false);
			pGlowColor->SetVecValue(fGlowColor[0], fGlowColor[1], fGlowColor[2]);
		}

		Func.Original<FN>()(ecx, edx, state, pInfo, pCustomBoneToWorld);

		I::ModelRender->ForcedMaterialOverride(nullptr);

		return true;
	}

	case ETFClientClass::CTFWearable:
	{
		C_TFWearable* pWearable = pEntity->As<C_TFWearable*>();

		if (!pWearable->moveparent())
			return false;

		if (Vars::Chams::Players::IgnoreTeam.m_Var && pWearable->InLocalTeam())
			return false;

		I::ModelRender->ForcedMaterialOverride([&]() -> IMaterial*
			{
				switch (Vars::Chams::Players::Material.m_Var)
				{
				case 1: return m_pMatFresnel;
				case 2: return m_pMatGlow;
				case 3: return m_pMatTest;
				case 4: return m_pMatShaded;
				case 5: return m_pMatToxic;
				default: return nullptr;
				}
			}());

		if (Vars::Chams::Players::Material.m_Var == 1)
		{
			IMaterialVar* pFresnelRanges = m_pMatFresnel->FindVar("$phongfresnelranges", NULL, false);
			pFresnelRanges->SetVecValue(Vars::Chams::Players::FresnelVars::FresnelX.m_Var, Vars::Chams::Players::FresnelVars::FresnelY.m_Var, Vars::Chams::Players::FresnelVars::FresnelZ.m_Var);

			float fBaseColor[3]; Vars::Chams::Players::FresnelVars::BaseColor.AsFloat(fBaseColor);
			IMaterialVar* pBaseColor = m_pMatFresnel->FindVar("$selfillumtint", NULL, false);
			pBaseColor->SetVecValue(fBaseColor[0], fBaseColor[1], fBaseColor[2]);

			float fGlowColor[3]; Vars::Chams::Players::FresnelVars::GlowColor.AsFloat(fGlowColor);
			IMaterialVar* pGlowColor = m_pMatFresnel->FindVar("$envmaptint", NULL, false);
			pGlowColor->SetVecValue(fGlowColor[0], fGlowColor[1], fGlowColor[2]);
		}

		Func.Original<FN>()(ecx, edx, state, pInfo, pCustomBoneToWorld);

		I::ModelRender->ForcedMaterialOverride(nullptr);

		return true;
	}

	default: break;
	}

	return false;
}
