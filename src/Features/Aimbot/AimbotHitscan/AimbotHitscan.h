#pragma once
#include "../AimbotGlobal/AimbotGlobal.h"

class CAimbot_Hitscan
{
public:
	void Run(C_TFPlayer* pLocal, C_TFWeaponBase* pWeapon, CUserCmd* cmd);

private:
	int GetBestHitbox(C_TFPlayer* pLocal, C_TFWeaponBase* pWeapon);
	void Aim(C_TFPlayer* pLocal, CUserCmd* cmd, QAngle& vAngle);
	const ESortMethod GetSortMethod();
	bool ShouldFire(C_TFPlayer* pLocal, C_TFWeaponBase* pWeapon, CUserCmd* cmd, const Target_t& Target);
	bool GetTargets(C_TFPlayer* pLocal, C_TFWeaponBase* pWeapon, CUserCmd* cmd);
	bool GetTarget(C_TFPlayer* pLocal, C_TFWeaponBase* pWeapon, CUserCmd* cmd, Target_t& Out);
};

namespace A { inline CAimbot_Hitscan Hitscan; }