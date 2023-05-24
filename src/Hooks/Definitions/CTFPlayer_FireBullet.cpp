#include "../Hooks.h"

#include "../../Features/Vars.h"

DEFINE_HOOK(CTFPlayer_FireBullet, void, __fastcall, C_TFPlayer* pThis, void* edx, C_TFWeaponBase* pWpn, FireBulletsInfo_t& info, bool bDoEffects, int nDamageType, int nCustomDamageType)
{
	if (Vars::Visual::Tracers.m_Var)
	{
		C_TFPlayer* pLocal = UTIL_TFPlayerByIndex(g_Globals.m_nLocalIndex);

		//Thanks to Spook for telling me this.
		if (pThis == pLocal)
		{
			//Remove crit type so tracers some tracers don't break.
			if (nDamageType & DMG_CRITICAL)
				nDamageType &= (~DMG_CRITICAL);

			//Draws tracer on each shot
			info.m_iTracerFreq = 1;
		}
	}

	Func.Original<FN>()(pThis, edx, pWpn, info, bDoEffects, nDamageType, nCustomDamageType);
}