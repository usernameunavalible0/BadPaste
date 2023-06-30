#include "../Hooks.h"

#include "../../Features/Vars.h"

DEFINE_HOOK(CTFWeaponBase_GetTracerType, const char*, __fastcall, C_TFWeaponBase* pThis, void* edx)
{
	if (Vars::Visual::Tracers.m_Var)
	{
		C_TFWeaponBase* pWeapon = G::EntityCache.GetWeapon();

		if (pThis == pWeapon)
		{
			return "merasmus_zap";
		}
	}

	return Func.Original<FN>()(pThis, edx);
}