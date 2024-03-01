//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: Weapon Knife Class
//
//=============================================================================
#ifndef TF_WEAPON_KNIFE_H
#define TF_WEAPON_KNIFE_H
#ifdef _WIN32
#pragma once
#endif

#include "tf_weaponbase_melee.h"

class C_TFPlayer;

// Knives use the "set_weapon_mode" attribute to define which type of knife they are
// Keep this enum in sync with the values used for set_weapon_mode.
enum knife_weapontypes_t
{
	KNIFE_STANDARD = 0,
	KNIFE_DISGUISE_ONKILL = 1,
	KNIFE_MOVEMENT_CLOAK = 2,		// The Cloak and Dagger
	KNIFE_ICICLE = 3,
};

//=============================================================================
//
// Knife class.
//
class C_TFKnife : public C_TFWeaponBaseMelee
{
public:
	
	virtual						~C_TFKnife() = 0;

public:
	M_NETVAR(m_bReadyToBackstab, bool, "CTFKnife", "m_bReadyToBackstab");
	M_NETVAR(m_bKnifeExists, bool, "CTFKnife", "m_bKnifeExists");
	M_NETVAR(m_flKnifeRegenerateDuration, float, "CTFKnife", "m_flKnifeRegenerateDuration");
	M_NETVAR(m_flKnifeMeltTimestamp, float, "CTFKnife", "m_flKnifeMeltTimestamp");

public:

	bool DoSwingTraceKnife(trace_t& trace, QAngle vAngleOverride = QAngle(VEC_T_NAN, VEC_T_NAN, VEC_T_NAN));

	bool CanPerformBackstabAgainstTarget(C_TFPlayer* pTarget, QAngle vAngleTo);
	bool IsBehindAndFacingTarget(C_TFPlayer* pTarget, QAngle vAngleTo);

	inline int GetKnifeType()
	{
		int iMode = 0;
		CALL_ATTRIB_HOOK_INT(iMode, set_weapon_mode);
		return iMode;
	}

	inline float GetProgress(void)
	{
		if (m_bKnifeExists())
		{
			return 1.0f;
		}

		float meltedTime = I::GlobalVars->curtime - m_flKnifeMeltTimestamp();

		return meltedTime / m_flKnifeRegenerateDuration();
	}
};


#endif // TF_WEAPON_KNIFE_H