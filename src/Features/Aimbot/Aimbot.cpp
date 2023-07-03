#include "Aimbot.h"
#include "AimbotHitscan/AimbotHitscan.h"
#include "../Vars.h"

void CFeatures_Aimbot::Run(C_TFPlayer* pLocal, C_TFWeaponBase* pWeapon, CUserCmd* cmd)
{
	g_Globals.m_flCurAimFOV = 0.0f;

	if (!Vars::Aimbot::Global::Enabled.m_Var)
		return;

	if (g_Globals.m_bIsGameUIVisible || I::MatSystemSurface->IsCursorVisible())
		return;

	if (pLocal->deadflag()
		|| pLocal->IsTaunting()
		|| pLocal->IsBonked()
		|| pLocal->m_bFeignDeathReady()
		|| pLocal->IsCloaked()
		|| pLocal->IsHalloweenKart()
		|| pLocal->IsHalloweenGhostMode())
		return;

	switch (pWeapon->m_iItemDefinitionIndex())
	{
	case Soldier_m_RocketJumper:
	case Demoman_s_StickyJumper:
		return;
	default: break;
	}

	switch (GetWeaponType(pWeapon))
	{
	case EWeaponType::HITSCAN: {
		A::Hitscan.Run(pLocal, pWeapon, cmd);
		break;
	}
	default: break;
	}
}

EWeaponType CFeatures_Aimbot::GetWeaponType(C_TFWeaponBase* pWeapon)
{
	if (pWeapon->GetSlot() == 2/*MELEE WEAPON SLOT*/)
		return EWeaponType::MELEE;

	const int nDamageType = pWeapon->GetDamageType();

	if (nDamageType & DMG_BULLET || nDamageType & DMG_BUCKSHOT)
		return EWeaponType::HITSCAN;

	return EWeaponType::UNKNOWN;
}
