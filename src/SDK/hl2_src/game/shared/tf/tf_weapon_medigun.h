//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================

#ifndef TF_WEAPON_MEDIGUN_H
#define TF_WEAPON_MEDIGUN_H
#ifdef _WIN32
#pragma once
#endif

#include "tf_weaponbase_gun.h"

class CTFMedigunShield;
class CTFReviveMarker;

enum medigun_weapontypes_t
{
	MEDIGUN_STANDARD = 0,
	MEDIGUN_UBER,
	MEDIGUN_QUICKFIX,
	MEDIGUN_RESIST,
};

enum medigun_resist_types_t
{
	MEDIGUN_BULLET_RESIST = 0,
	MEDIGUN_BLAST_RESIST,
	MEDIGUN_FIRE_RESIST,
	MEDIGUN_NUM_RESISTS
};

struct MedigunEffects_t
{
	ETFCond eCondition;
	ETFCond eWearingOffCondition;
	const char* pszChargeOnSound;
	const char* pszChargeOffSound;
};

extern MedigunEffects_t g_MedigunEffects[MEDIGUN_NUM_CHARGE_TYPES];

#define MAX_HEALING_TARGETS			1	//6

#define CLEAR_ALL_TARGETS			-1


//=========================================================
// Beam healing gun
//=========================================================
class C_WeaponMedigun : public C_TFWeaponBaseGun
{
public:
	virtual						~C_WeaponMedigun() = 0;

	virtual float				GetTargetRange(void) = 0;
	virtual float				GetStickRange(void) = 0;
	virtual float				GetHealRate(void) = 0;
	virtual bool				AppliesModifier(void) = 0;

public:
	M_NETVAR(m_hHealingTarget, EHANDLE, "CWeaponMedigun", "m_hHealingTarget");
	M_NETVAR(m_bHealing, bool, "CWeaponMedigun", "m_bHealing");
	M_NETVAR(m_bAttacking, bool, "CWeaponMedigun", "m_bAttacking");
	M_NETVAR(m_bHolstered, bool, "CWeaponMedigun", "m_bHolstered");
	M_NETVAR(m_bChargeRelease, bool, "CWeaponMedigun", "m_bChargeRelease");
	M_NETVAR(m_flChargeLevel, float, "CWeaponMedigun", "m_flChargeLevel");
	M_NETVAR(m_nChargeResistType, int, "CWeaponMedigun", "m_nChargeResistType");

	bool AllowedToHealTarget(C_BaseEntity* pTarget);

	inline bool HealingTarget(C_BaseEntity* pTarget)
	{
		if (pTarget == m_hHealingTarget())
			return true;
		
		return false;
	}

	struct targetdetachtimes_t
	{
		float	flTime;
		EHANDLE	hTarget;
	};

	struct healingtargeteffects_t
	{
		C_BaseEntity* pOwner;
		C_BaseEntity* pTarget;
		CNewParticleEffect* pEffect;
		CNewParticleEffect* pCustomEffect;
	};
};

#endif // TF_WEAPON_MEDIGUN_H