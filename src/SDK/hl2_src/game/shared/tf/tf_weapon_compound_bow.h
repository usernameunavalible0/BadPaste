//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================
#ifndef TF_WEAPON_COMPOUND_BOW_H
#define TF_WEAPON_COMPOUND_BOW_H
#ifdef _WIN32
#pragma once
#endif

#include "tf_weaponbase_gun.h"
#include "tf_weapon_pipebomblauncher.h"

//=============================================================================
//
// TF Weapon Bow
//
class C_TFCompoundBow : public C_TFPipebombLauncher
{
public:
	virtual						~C_TFCompoundBow() = 0;

	virtual void				LowerBow(void) = 0;

	virtual bool				OwnerCanJump(void) = 0;

private:

	virtual void				StartBurningEffect(void) = 0;
	virtual void				StopBurningEffect(void) = 0;

private:

	M_NETVAR(m_bNoFire, bool, "CTFCompoundBow", "m_bNoFire");
	M_NETVAR(m_bArrowAlight, bool, "CTFCompoundBow", "m_bArrowAlight");

};

#endif // TF_WEAPON_COMPOUND_BOW_H