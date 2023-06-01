#include "Visual.h"
#include "../Vars.h"
#include "../../SDK/hl2_src/materialsystem/imaterialinternal.h"

void CFeatures_Visual::FOV(CViewSetup* pSetup)
{
	C_TFPlayer* pLocal = G::EntityCache.GetLocal();

	if (pLocal && pSetup)
	{
		if (pLocal->GetObserverMode() == OBS_MODE_IN_EYE || (pLocal->IsZoomed() && !Vars::Visual::RemoveZoom.m_Var))
			return;

		pSetup->fov = Vars::Visual::FOV.m_Var;

		if (pLocal->IsAlive())
			pLocal->m_iFOV() = Vars::Visual::FOV.m_Var;
	}
}

void CFeatures_Visual::Thirdperson(C_TFPlayer* pLocal)
{
	static bool bToggle = false;
	static float flLastHit = I::EngineClient->Time();

	if (!Vars::Visual::Thirdperson.m_Var)
		return;

	if (pLocal->deadflag())
		return;

	if (GetAsyncKeyState(Vars::Visual::ThirdpersonKey.m_Var) & 0x8000)
	{
		if ((I::EngineClient->Time() - flLastHit) > 0.25f) {
			pLocal->m_nForceTauntCam() = static_cast<int>(bToggle = !bToggle);
			flLastHit = I::EngineClient->Time();
		}
	}
}

void CFeatures_Visual::WeaponSway()
{
	if (Vars::Visual::ViewModelSway.m_Var)
	{
		if (C_TFWeaponBase* pWeapon = G::EntityCache.GetWeapon())
			G::ConVars.cl_wpn_sway_interp->SetValue(pWeapon->GetWeaponID() == TF_WEAPON_COMPOUND_BOW ? 0.02f : 0.05f);
	}
	else
	{
		if (G::ConVars.cl_wpn_sway_interp->GetFloat())
			G::ConVars.cl_wpn_sway_interp->SetValue(0.0f);
	}
}

bool bWorldIsModulated = false;

void CFeatures_Visual::ModulateWorld()
{
	if (!Vars::Visual::ModulateWorld.m_Var)
		return;

	static KeyValues* kv = nullptr;
	if (!kv)
	{
		kv = new KeyValues("LightmappedGeneric");
		kv->SetString("$basetexture", "dev/dev_measuregeneric01b");
		kv->SetString("$basetexturetransform", "center .5 .5 scale 1 1 rotate 0 translate 0 0");
	}

	for (MaterialHandle_t h = I::MaterialSystem->FirstMaterial(); h != I::MaterialSystem->InvalidMaterial(); h = I::MaterialSystem->NextMaterial(h))
	{
		if (IMaterial* pMat = I::MaterialSystem->GetMaterial(h))
		{
			if (pMat->IsErrorMaterial() || !pMat->IsPrecached() || pMat->IsTranslucent() || pMat->IsSpriteCard())
				continue;

			if (!strcmp(pMat->GetTextureGroupName(), TEXTURE_GROUP_WORLD))
			{
				pMat->SetShaderAndParams(kv);
			}
		}
	}

	//This makes the time it takes to load a map longer but I cannot find another solution to fix the scaling
	I::MaterialSystem->ReloadMaterials(NULL);

	bWorldIsModulated = true;
}

void CFeatures_Visual::UpdateWorldModulation()
{
	if (!bWorldIsModulated && Vars::Visual::ModulateWorld.m_Var)
		ModulateWorld();
}