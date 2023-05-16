#pragma once
#include "../../SDK/SDK.h"

class CFeatures_Crithack
{
public:
	void Run(C_TFWeaponBase* pWeapon, CUserCmd* cmd);

private:
	//NOTE: If you dont have CUtlVector avalible to you you can use std::vector instead
	CUtlVector<int> m_CritTicks;
};

namespace F { inline CFeatures_Crithack Crithack; }