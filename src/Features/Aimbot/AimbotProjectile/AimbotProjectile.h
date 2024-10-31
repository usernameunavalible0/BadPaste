#pragma once
#include "../AimbotGlobal/AimbotGlobal.h"

class CAimbot_Projectile
{
private:
	struct ProjectileInfo_t
	{
		float m_flVelocity = 0.0f;
		float m_flGravity = 0.0f;
		float m_flMaxTime = 2.0f;
	};

	struct Predictor_t
	{
		C_BaseEntity* m_pEntity = nullptr;
		Vector m_vPosition = Vector();
		Vector m_vVelocity = Vector();
		Vector m_vAcceleration = Vector();

		Vector Extrapolate(float time);
	};

	struct Solution_t
	{
		float m_flPitch = 0.0f;
		float m_flYaw = 0.0f;
		float m_flTime = 0.0f;
	};

	bool GetProjectileInfo(C_TFWeaponBase* pWeapon, ProjectileInfo_t& out);
	bool CalcProjAngle(const Vector& vLocalPos, const Vector& vTargetPos, const ProjectileInfo_t& ProjInfo, Solution_t& out);
	bool SolveProjectile(C_TFPlayer* pLocal, C_TFWeaponBase* pWeapon, CUserCmd* cmd, Predictor_t& Predictor, const ProjectileInfo_t& ProjInfo, Solution_t& out);

	inline bool GetWeaponProjectileMinsMaxs(C_TFWeaponBase* pWeapon, Vector& mins, Vector& maxs)
	{
		switch (pWeapon->GetWeaponID())
		{
		case TF_WEAPON_CROSSBOW:
		{
			mins = Vector(-3.0f, -3.0f, -3.0f);
			maxs = Vector(3.0f, 3.0f, 3.0f);
			return true;
		}
		case TF_WEAPON_COMPOUND_BOW:
		case TF_WEAPON_JAR:
		case TF_WEAPON_JAR_MILK:
		case TF_WEAPON_SHOTGUN_BUILDING_RESCUE:
		case TF_WEAPON_SPELLBOOK:
		case TF_WEAPON_MECHANICAL_ARM:
		//case TF_WEPON_FLAME_BALL:
		{
			mins = Vector(-1.0f, -1.0f, -1.0f);
			maxs = Vector(1.0f, 1.0f, 1.0f);
			return true;
		}
		default:
		{
			mins = Vector();
			maxs = Vector();
			return false;
		}
		}
	}

private:
	Vector GetAimPos(C_TFPlayer* pLocal, C_BaseEntity* pEntity);
	ESortMethod GetSortMethod();
	bool GetTargets(C_TFPlayer* pLocal, C_TFWeaponBase* pWeapon);
	bool VerifyTarget(C_TFPlayer* pLocal, C_TFWeaponBase* pWeapon, CUserCmd* cmd, Target_t& Target);
	bool GetTarget(C_TFPlayer* pLocal, C_TFWeaponBase* pWeapon, CUserCmd* cmd, Target_t& Out);
	void Aim(C_TFPlayer* pLocal, CUserCmd* cmd, C_TFWeaponBase* pWeapon, QAngle& vAngle);
	bool ShouldFire(C_TFWeaponBase* pWeapon, CUserCmd* cmd);
	bool IsAttacking(CUserCmd* cmd, C_TFWeaponBase* pWeapon);

	bool m_bIsFlameThrower;

public:
	void Run(C_TFPlayer* pLocal, C_TFWeaponBase* pWeapon, CUserCmd* cmd);
};

namespace A { inline CAimbot_Projectile Projectile; }