//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: TF Sniper Rifle
//
//=============================================================================//
#ifndef TF_WEAPON_SNIPERRIFLE_H
#define TF_WEAPON_SNIPERRIFLE_H
#ifdef _WIN32
#pragma once
#endif

#include "tf_weaponbase_gun.h"

enum RifleTypes_t
{
	RIFLE_NORMAL = 0,
	RIFLE_JARATE,
	RIFLE_MACHINA,
	RIFLE_CLASSIC,
};

//=============================================================================
//
// Sniper Rifle class.
//
class C_TFSniperRifle : public C_TFWeaponBaseGun
{
public:
	virtual						~C_TFSniperRifle() = 0;
	
	virtual bool				OwnerCanJump(void) = 0;

	virtual void				HandleZooms(void) = 0;

	virtual bool				MustBeZoomedToFire(void) = 0;

	virtual bool				IsJarateRifle(void) const = 0;
	virtual float				GetJarateTime(void) const = 0;

	virtual float				SniperRifleChargeRateMod() = 0;

	virtual int					GetBuffType() = 0;

protected:
	virtual void				Zoom(void) = 0;

public:
	M_NETVAR(m_flChargedDamage, float, "CTFSniperRifle", "m_flChargedDamage");
};

#endif // TF_WEAPON_SNIPERRIFLE_H