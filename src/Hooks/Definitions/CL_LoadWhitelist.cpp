#include "../Hooks.h"
#include "../../Features/Vars.h"

DEFINE_HOOK(CL_LoadWhitelist, CPureServerWhitelist*, __cdecl, INetworkStringTable* pTable, const char* pName)
{
	return Vars::Misc::BypassPure.m_Var ? NULL : Func.Original<FN>()(pTable, pName);
}