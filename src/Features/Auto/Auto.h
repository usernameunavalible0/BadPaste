#pragma once
#include "../../SDK/SDK.h"

class CFeatures_Auto
{
public:
	void Run(C_TFPlayer* pLocal, C_TFWeaponBase* pWeapon, CUserCmd* cmd);

private:
	void AutoBackstab(C_TFPlayer* pLocal, C_TFWeaponBase* pWeapon, CUserCmd* cmd);
};

namespace F { inline CFeatures_Auto Auto; }