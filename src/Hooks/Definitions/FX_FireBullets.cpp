#include "../Hooks.h"
#include "../../Features/Vars.h"
#include "../../Features/AntiHack/AntiHack.h"

DEFINE_HOOK(FX_FireBullets, void, __cdecl, C_TFWeaponBase* pWpn, int iPlayer, const Vector& vecOrigin, const QAngle& vecAngles, int iWeapon, int iMode, int iSeed, float flSpread, float flDamage, bool bCritical)
{
	if (Vars::AntiHack::Resolver::Enabled.m_Var && !Vars::AntiHack::Resolver::Mode.m_Var && iPlayer != g_Globals.m_nLocalIndex)
	{
		F::AntiHack.AddOnShotAngles(iPlayer, vecAngles);
	}

	Func.Original<FN>()(pWpn, iPlayer, vecOrigin, vecAngles, iWeapon, iMode, iSeed, flSpread, flDamage, bCritical);
}