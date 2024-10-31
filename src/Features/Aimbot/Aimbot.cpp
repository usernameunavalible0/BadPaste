#include "Aimbot.h"
#include "AimbotHitscan/AimbotHitscan.h"
#include "AimbotProjectile/AimbotProjectile.h"
#include "AimbotMelee/AimbotMelee.h"
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
	case EWeaponType::PROJECTILE: {
		A::Projectile.Run(pLocal, pWeapon, cmd);
		break;
	}
	case EWeaponType::MELEE: {
		A::Melee.Run(pLocal, pWeapon, cmd);
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

	switch (pWeapon->GetWeaponID())
	{
	case TF_WEAPON_ROCKETLAUNCHER:
	case TF_WEAPON_GRENADELAUNCHER:
	case TF_WEAPON_FLAREGUN:
	case TF_WEAPON_COMPOUND_BOW:
	case TF_WEAPON_ROCKETLAUNCHER_DIRECTHIT:
	case TF_WEAPON_CROSSBOW:
	case TF_WEAPON_PARTICLE_CANNON:
	case TF_WEAPON_DRG_POMSON:
	case TF_WEAPON_FLAREGUN_REVENGE:
	case TF_WEAPON_CANNON:
	case TF_WEAPON_SYRINGEGUN_MEDIC:
	case TF_WEAPON_SHOTGUN_BUILDING_RESCUE:
	case TF_WEAPON_FLAMETHROWER:
	{
		return EWeaponType::PROJECTILE;
	}
	}

	if (nDamageType & DMG_BULLET || nDamageType & DMG_BUCKSHOT)
		return EWeaponType::HITSCAN;

	return EWeaponType::UNKNOWN;
}
