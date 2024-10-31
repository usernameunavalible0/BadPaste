#pragma once
#include "../../SDK/SDK.h"

class CFeatures_Auto
{
public:
	void Run(C_TFPlayer* pLocal, C_TFWeaponBase* pWeapon, CUserCmd* cmd);

private:
	void AutoBackstab(C_TFPlayer* pLocal, C_TFWeaponBase* pWeapon, CUserCmd* cmd);
	void AutoHeal(C_TFPlayer* pLocal, C_TFWeaponBase* pWeapon, CUserCmd* cmd);
	void AutoDisguise(C_TFPlayer* pLocal, C_TFWeaponBase* pWeapon, CUserCmd* cmd);

private:
	CUtlVector<int> m_vecHealablePlayers;
};

namespace F { inline CFeatures_Auto Auto; }