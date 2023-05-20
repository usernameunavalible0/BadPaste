//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================

#ifndef ECON_WEARABLE_H
#define ECON_WEARABLE_H
#ifdef _WIN32
#pragma once
#endif

#include "econ_entity.h"

enum
{
	MAX_WEARABLES_SENT_FROM_SERVER = 8
};

enum
{
	ITEM_DROP_TYPE_NULL,
	ITEM_DROP_TYPE_NONE,
	ITEM_DROP_TYPE_DROP,
	ITEM_DROP_TYPE_BREAK,
};

class C_EconWearable : public C_EconEntity
{
public:
	virtual					~C_EconWearable() = 0;

	virtual bool			IsWearable(void) const = 0;
	virtual void			GiveTo(C_BaseEntity* pOther) = 0;
	virtual void			RemoveFrom(C_BaseEntity* pOther) = 0;
	virtual bool			CanEquip(C_BaseEntity* pOther) = 0;
	virtual void			Equip(C_BasePlayer* pOwner) = 0;
	virtual void			UnEquip(C_BasePlayer* pOwner) = 0;
	virtual void			OnWearerDeath(void) = 0;
	virtual int				GetDropType(void) = 0;
	virtual bool			IsViewModelWearable(void) = 0;
	virtual bool			ShouldDrawWhenPlayerIsDead() = 0;

protected:
	virtual void			InternalSetPlayerDisplayModel(void) = 0;
};

#endif // ECON_WEARABLE_H