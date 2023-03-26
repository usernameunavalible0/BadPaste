//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================

#ifndef TF_WEAPON_PIPEBOMBLAUNCHER_H
#define TF_WEAPON_PIPEBOMBLAUNCHER_H
#ifdef _WIN32
#pragma once
#endif

#include "tf_weaponbase_gun.h"

class CTFGrenadePipebombProjectile;

#define TF_PIPEBOMB_MAX_CHARGE_TIME	 4.0f

#define TF_DETONATE_MODE_STANDARD	0
#define TF_DETONATE_MODE_DOT		1
#define TF_DETONATE_MODE_AIR		2

// hard code these eventually
#define TF_PIPEBOMB_MIN_CHARGE_VEL 900
#define TF_PIPEBOMB_MAX_CHARGE_VEL 2400


//=============================================================================
//
// TF Weapon Pipebomb Launcher.
//
class C_TFPipebombLauncher : public C_TFWeaponBaseGun, public ITFChargeUpWeapon
{
public:
	virtual						~C_TFPipebombLauncher() = 0;

	virtual float				GetChargeForceReleaseTime(void) = 0;

	virtual void				LaunchGrenade(void) = 0;
	virtual void				ForceLaunchGrenade(void) = 0;
	virtual bool				DetonateRemotePipebombs(bool bFizzle) = 0;
	virtual bool				ModifyPipebombsInView(int iEffect) = 0;
	virtual void				AddPipeBomb(CTFGrenadePipebombProjectile* pBomb) = 0;

	M_NETVAR(m_flChargeBeginTime, float, "CTFPipebombLauncher", "m_flChargeBeginTime");
};


#endif // TF_WEAPON_PIPEBOMBLAUNCHER_H