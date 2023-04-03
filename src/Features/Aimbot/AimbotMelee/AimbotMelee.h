#pragma once
#include "../AimbotGlobal/AimbotGlobal.h"

class CAimbot_Melee
{
private:
	bool CanMeleeHit(C_TFPlayer* pLocal, C_TFWeaponBaseMelee* pWeapon, C_BaseEntity* pEntity);

private:
	ESortMethod GetSortMethod();
	bool GetTargets(C_TFPlayer* pLocal, C_TFWeaponBaseMelee* pWeapon);
	bool VerifyTarget(C_TFPlayer* pLocal, C_TFWeaponBaseMelee* pWeapon, Target_t& Target);
	bool GetTarget(C_TFPlayer* pLocal, C_TFWeaponBaseMelee* pWeapon, Target_t& Out);
	void Aim(CUserCmd* pCmd, QAngle& vAngle);
	bool ShouldSwing(C_TFPlayer* pLocal, C_TFWeaponBaseMelee* pWeapon, CUserCmd* pCmd, const Target_t& Target);
	bool IsAttacking(CUserCmd* pCmd, C_TFWeaponBaseMelee* pWeapon);

public:
	void Run(C_TFPlayer* pLocal, C_TFWeaponBase* pWeapon, CUserCmd* pCmd);
};

namespace A { inline CAimbot_Melee Melee; }