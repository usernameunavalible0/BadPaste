//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: Weapon Base Gun 
//
//=============================================================================

#ifndef TF_WEAPONBASE_GUN_H
#define TF_WEAPONBASE_GUN_H
#ifdef _WIN32
#pragma once
#endif

#include "tf_shareddefs.h"
#include "tf_weaponbase.h"

#define ZOOM_CONTEXT		"ZoomContext"
#define ZOOM_REZOOM_TIME	1.4f

//=============================================================================
//
// Weapon Base Melee Gun
//
class C_TFWeaponBaseGun : public C_TFWeaponBase
{
public:
	virtual						~C_TFWeaponBaseGun() = 0;

	virtual void				DoFireEffects() = 0;
	virtual bool				ShouldDoMuzzleFlash(void) = 0;
	virtual int					GetWeaponProjectileType(void) const = 0;
	virtual C_BaseEntity*		FireProjectile(C_TFPlayer* pPlayer) = 0;
	virtual void				RemoveProjectileAmmo(C_TFPlayer* pPlayer) = 0;
	virtual void				ModifyProjectile(C_BaseEntity* pProj) = 0;
	virtual void				FireBullet(C_TFPlayer* pPlayer) = 0;
	virtual C_BaseEntity*		FirePipeBomb(C_TFPlayer* pPlayer, int iPipeBombType) = 0;
	virtual C_BaseEntity*		FireArrow(C_TFPlayer* pPlayer, ProjectileType_t projectileType) = 0;
	virtual C_BaseEntity*		FireJar(C_TFPlayer* pPlayer) = 0;
	virtual C_BaseEntity*		FireFlameRocket(C_TFPlayer* pPlayer) = 0;
	virtual C_BaseEntity*		FireEnergyBall(C_TFPlayer* pPlayer, bool bRing = false) = 0;
	virtual float				GetWeaponSpread(void) = 0;
	virtual void				GetCustomProjectileModel(void* attrCustomProjModel) = 0;
	virtual float				GetProjectileSpeed(void) = 0;
	virtual float				GetProjectileGravity(void) = 0;
	virtual float				GetProjectileSpread(void) = 0;
	virtual int					GetAmmoPerShot(void) = 0;
	virtual float				GetProjectileDamage(void) = 0;
	virtual bool				ShouldPlayFireAnim(void) = 0;
	virtual void				ZoomIn(void) = 0;
	virtual void				ZoomOut(void) = 0;
	virtual void				PlayWeaponShootSound(void) = 0;
	virtual bool				ShouldRemoveDisguiseOnPrimaryAttack() const = 0;
};

#endif // TF_WEAPONBASE_GUN_H