#pragma once

#include "EntityCache/EntityCache.h"

#define XASSERT(x) if (x) Error(#x)

#define FAILED_TO_INITIALIZE false
#define FAILED_TO_HOOK false

#define FONT_MENU EFonts::MENU_TAHOMA

namespace I { inline void* TFGameMovement = nullptr; }

class CUtil
{
public:
	void FixMovement(const QAngle& va, CUserCmd* cmd);

	bool IsVisible(const Vector& vStart, const Vector& vEnd);
	bool IsVisible(const Vector& vStart, const Vector& vEnd, const Vector& vMins, const Vector& vMaxs);

	Color GetTeamColor(const int nTeam);
	Color GetHealthColor(const int nHealth, const int nMaxHealth);

	float GetClientInterpAmount();

	Vector EstimateProjectileImpactPosition(C_TFPlayer* pPlayer, float pitch, float yaw, float initVel);
	Vector VelocityToAngles(const Vector direction);
};

namespace G { inline CUtil Util; }