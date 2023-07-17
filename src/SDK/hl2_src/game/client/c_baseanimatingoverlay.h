//========= Copyright (c) Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
// $NoKeywords: $
//
//=============================================================================//
#ifndef C_BASEANIMATINGOVERLAY_H
#define C_BASEANIMATINGOVERLAY_H
#pragma once

#include "c_baseanimating.h"
#include "animationlayer.h"

class C_BaseAnimatingOverlay : public C_BaseAnimating
{
public:
	virtual ~C_BaseAnimatingOverlay() = 0;

public:
	//I hope this works
	M_NETVAR(m_AnimOverlay, CUtlVector<C_AnimationLayer>, "CBaseAnimatingOverlay", "m_AnimOverlay");

	M_NETVAR(m_flPrevCycle, float, "CBaseAnimatingOverlay", "m_flPrevCycle");
	M_NETVAR(m_flWeight, float, "CBaseAnimatingOverlay", "m_flWeight");
	M_NETVAR(m_nOrder, int, "CBaseAnimatingOverlay", "m_nOrder");

	inline int GetNumAnimOverlays()
	{
		return m_AnimOverlay().Count();
	}

	C_AnimationLayer* GetAnimOverlay(int i)
	{
		return &m_AnimOverlay()[i];
	}
};

#endif //C_BASEANIMATINGOVERLAY_H