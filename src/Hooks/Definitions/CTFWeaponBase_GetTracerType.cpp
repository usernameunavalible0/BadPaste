#include "../Hooks.h"

#include "../../Features/Vars.h"

DEFINE_HOOK(CTFWeaponBase_GetTracerType, const char*, __fastcall, C_TFWeaponBase* pThis, void* edx)
{
	if (Vars::Visual::Tracers.m_Var == 2)
	{
		C_TFWeaponBase* pWeapon = G::EntityCache.GetWeapon();

		if (pThis == pWeapon)
			return "merasmus_zap";
	}
	else if (Vars::Visual::Tracers.m_Var == 1)
	{
		C_TFWeaponBase* pWeapon = G::EntityCache.GetWeapon();

		if (pThis == pWeapon)
			return NULL;
	}

	return Func.Original<FN>()(pThis, edx);
}