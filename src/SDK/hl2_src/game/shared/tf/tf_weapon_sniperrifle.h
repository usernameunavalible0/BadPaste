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
// Sniper Rifle Laser Dot class.
//
class C_SniperDot : public C_BaseEntity
{
public:
	virtual						~C_SniperDot() = 0;

	M_NETVAR(m_flChargeStartTime, float, "CSniperDot", "m_flChargeStartTime");
};

#define TF_WEAPON_SNIPERRIFLE_CHARGE_PER_SEC	50.0
#define TF_WEAPON_SNIPERRIFLE_UNCHARGE_PER_SEC	75.0
#define	TF_WEAPON_SNIPERRIFLE_DAMAGE_MIN		50
#define TF_WEAPON_SNIPERRIFLE_DAMAGE_MAX		150
#define TF_WEAPON_SNIPERRIFLE_RELOAD_TIME		1.5f
#define TF_WEAPON_SNIPERRIFLE_ZOOM_TIME			0.3f
#define TF_WEAPON_SNIPERRIFLE_NO_CRIT_AFTER_ZOOM_TIME	0.2f

//=============================================================================
//
// Sniper Rifle class.
//
class C_TFSniperRifle : public C_TFWeaponBaseGun
{
public:
	virtual						~C_TFSniperRifle() = 0;

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

	inline bool IsFullyCharged()
	{
		return m_flChargedDamage() >= TF_WEAPON_SNIPERRIFLE_DAMAGE_MAX;
	}

	inline int GetRifleType()
	{
		int iMode = 0;
		CALL_ATTRIB_HOOK_INT(iMode, set_weapon_mode);
		return iMode;
	}
};

#endif // TF_WEAPON_SNIPERRIFLE_H
