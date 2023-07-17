#pragma once
#include "../AimbotGlobal/AimbotGlobal.h"

class CAimbot_Melee
{
public:
	void Run(C_TFPlayer* pLocal, C_TFWeaponBase* pWeapon, CUserCmd* cmd);

private:
	const ESortMethod GetSortMethod();
	bool IsAttacking(CUserCmd* cmd, C_TFWeaponBaseMelee* pWeapon);
	void Aim(C_TFPlayer* pLocal, CUserCmd* cmd, QAngle& vAngle);
	bool GetTargets(C_TFPlayer* pLocal, C_TFWeaponBaseMelee* pWeapon);
	bool GetTarget(C_TFPlayer* pLocal, C_TFWeaponBaseMelee* pWeapon, Target_t& Out);
};

namespace A { inline CAimbot_Melee Melee; }