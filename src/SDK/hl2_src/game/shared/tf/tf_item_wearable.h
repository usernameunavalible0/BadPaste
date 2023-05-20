//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================

#ifndef TF_WEARABLE_H
#define TF_WEARABLE_H
#ifdef _WIN32
#pragma once
#endif

#include "../econ/econ_wearable.h"

class C_TFWearable : public C_EconWearable, public CGameEventListener
{
public:
	virtual				~C_TFWearable() = 0;

	virtual int			GetWorldModelIndex(void) = 0;

public:
	M_NETVAR(m_bDisguiseWearable, bool, "CTFWearable", "m_bDisguiseWearable");
	M_NETVAR(m_hWeaponAssociatedWith, EHANDLE, "CTFWearable", "m_hWeaponAssociatedWith");

	enum eParticleSystemVisibility
	{
		kParticleSystemVisibility_Undetermined,
		kParticleSystemVisibility_Shown,
		kParticleSystemVisibility_Hidden,
	};
	eParticleSystemVisibility m_eParticleSystemVisibility;
};

#endif // TF_WEARABLE_H