#include "../Hooks.h"
#include "../../Features/Vars.h"

DEFINE_HOOK(CTFPlayer_AvoidPlayers, void, __fastcall, C_TFPlayer* pThis, void* edx, CUserCmd* pCmd)
{
	if (Vars::Misc::NoPush.m_Var)
		return;

	Func.Original<FN>()(pThis, edx, pCmd);
}