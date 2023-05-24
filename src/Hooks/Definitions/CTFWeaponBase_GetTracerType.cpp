#include "../Hooks.h"

#include "../../Features/Vars.h"

DEFINE_HOOK(CTFWeaponBase_GetTracerType, const char*, __fastcall, C_TFWeaponBase* pThis, void* edx)
{
	return Vars::Visual::Tracers.m_Var ? "merasmus_zap" : Func.Original<FN>()(pThis, edx);
}