#include "../Hooks.h"
#include "../../Features/Vars.h"

DEFINE_HOOK(CL_Move, void, __cdecl, float accumulated_extra_samples, bool bFinalTick)
{
	if (Vars::Misc::CL_Move::Enabled.m_Var)
	{
		if (Vars::Misc::CL_Move::TeleportKey.m_Var && (GetAsyncKeyState(Vars::Misc::CL_Move::TeleportKey.m_Var)) && !g_Globals.m_nShifted)
		{
			while (g_Globals.m_nShifted < MAX_NEW_COMMANDS)
			{
				Func.Original<FN>()(accumulated_extra_samples, (g_Globals.m_nShifted == (MAX_NEW_COMMANDS - 1)));
				g_Globals.m_nShifted++;
			}

			return;
		}

		if (GetAsyncKeyState(Vars::Misc::CL_Move::RechargeKey.m_Var))
			g_Globals.m_bRecharging = true;
	}

	if (g_Globals.m_bRecharging && g_Globals.m_nShifted)
	{
		g_Globals.m_nShifted--;
		g_Globals.m_nWaitForShift = DT_WAIT_CALLS;
		return;
	}
	else
		g_Globals.m_bRecharging = false;

	Func.Original<FN>()(accumulated_extra_samples, bFinalTick);

	if (g_Globals.m_nWaitForShift)
	{
		g_Globals.m_nWaitForShift--;
		return;
	}

	if (g_Globals.m_bShouldShift)
	{
		while (g_Globals.m_nShifted < MAX_NEW_COMMANDS)
		{
			Func.Original<FN>()(accumulated_extra_samples, (g_Globals.m_nShifted == (MAX_NEW_COMMANDS - 1)));
			g_Globals.m_nShifted++;
		}

		g_Globals.m_bShouldShift = false;
	}
}