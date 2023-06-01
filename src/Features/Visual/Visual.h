#pragma once
#include "../../SDK/SDK.h"

class CFeatures_Visual
{
public:
	void FOV(CViewSetup* pSetup);
	void Thirdperson(C_TFPlayer* pLocal);
	void WeaponSway();
	void ModulateWorld();
	void UpdateWorldModulation();
};

namespace F { inline CFeatures_Visual Visual; }