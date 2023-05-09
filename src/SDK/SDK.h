#pragma once

#include "EntityCache/EntityCache.h"

#define XASSERT(x) if (x) Error(#x)

#define FAILED_TO_INITIALIZE false
#define FAILED_TO_HOOK false

#define FONT_MENU EFonts::MENU_TAHOMA

class CUtil
{
public:
	void FixMovement(const QAngle& va, CUserCmd* cmd);

	bool IsVisible(const Vector& vStart, const Vector& vEnd);

	Color GetTeamColor(const int nTeam);
	Color GetHealthColor(const int nHealth, const int nMaxHealth);
};

namespace G { inline CUtil Util; }