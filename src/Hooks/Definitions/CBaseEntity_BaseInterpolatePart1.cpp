#include "../Hooks.h"
#include "../../Features/Vars.h"

DEFINE_HOOK(CBaseEntity_BaseInterpolatePart1, int, __fastcall, C_BaseEntity* pThis, void* edx, float& currentTime, Vector& oldOrigin, QAngle& oldAngles, Vector& oldVel, int& bNoMoreChanges)
{
	return Func.Original<FN>()(pThis, edx, currentTime, oldOrigin, oldAngles, oldVel, bNoMoreChanges);
}