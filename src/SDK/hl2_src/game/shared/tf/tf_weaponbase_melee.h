//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: Weapon Base Melee 
//
//=============================================================================

#ifndef TF_WEAPONBASE_MELEE_H
#define TF_WEAPONBASE_MELEE_H
#ifdef _WIN32
#pragma once
#endif

#include "tf_shareddefs.h"
#include "tf_weaponbase.h"

extern ConVar cl_crosshair_file;

//=============================================================================
//
// Weapon Base Melee Class
//
class C_TFWeaponBaseMelee : public C_TFWeaponBase
{
public:

	virtual						~C_TFWeaponBaseMelee() = 0;

	virtual int					GetSwingRange(void) = 0;

	virtual void				DoViewModelAnimation(void) = 0;

	virtual bool				DoSwingTrace(trace_t& trace) = 0;
	virtual void				Smack(void) = 0;
    virtual float               GetSmackTime(int iWeaponMode) = 0;
	virtual void				DoMeleeDamage(C_BaseEntity* ent, trace_t& trace) = 0;
	virtual void				DoMeleeDamage(C_BaseEntity* ent, trace_t& trace, float flDamageMod) = 0;

	virtual float				GetMeleeDamage(C_BaseEntity* pTarget, int* piDamageType, int* piCustomDamage) = 0;

	// Call when we hit an entity. Use for special weapon effects on hit.
	virtual void				OnEntityHit(C_BaseEntity* pEntity, CTakeDamageInfo* info) = 0;

	virtual void				SendPlayerAnimEvent(C_TFPlayer* pPlayer) = 0;

protected:

	virtual void				Swing(C_TFPlayer* pPlayer) = 0;
    
public:
    
    //im just guessing here lol
    virtual void                PlaySwingSound(void) = 0;

	bool DoSwingTraceInternal(trace_t& trace, CUtlVector<trace_t>* pTargetTraceVector, const QAngle vAngleOverride);
};

#endif // TF_WEAPONBASE_MELEE_H
