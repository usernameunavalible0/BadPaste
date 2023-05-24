#include "../Hooks.h"
#include "../../Features/Vars.h"

DEFINE_HOOK(CBasePlayer_CalcPlayerView, void, __fastcall, C_BasePlayer* pThis, void* edx, Vector& eyeOrigin, QAngle& eyeAngles, float& fov)
{
	if (Vars::Visual::RemoveVisualRecoil.m_Var && pThis) //Did this in l4d2 when Spookman told me to, so doing it here comes from him.
	{
		const QAngle vOldPunch = pThis->m_vecPunchAngle();

		pThis->m_vecPunchAngle().Init();
		Func.Original<FN>()(pThis, edx, eyeOrigin, eyeAngles, fov);
		pThis->m_vecPunchAngle() = vOldPunch;
	}
	else
	{
		Func.Original<FN>()(pThis, edx, eyeOrigin, eyeAngles, fov);
	}
}