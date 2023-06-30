#pragma once
#include "../../SDK/SDK.h"

enum struct EWeaponType
{
	UNKNOWN, HITSCAN, PROJECTILE, MELEE
};

class CFeatures_Aimbot
{
public:
	void Run(C_TFPlayer* pLocal, C_TFWeaponBase* pWeapon, CUserCmd* cmd);

private:
	EWeaponType GetWeaponType(C_TFWeaponBase* pWeapon);
};

namespace F { inline CFeatures_Aimbot Aimbot; }