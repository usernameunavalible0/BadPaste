#include "../Hooks.h"
#include "../../Features/Vars.h"

DEFINE_HOOK(CL_CheckForPureServerWhitelist, void, __cdecl, IFileList*& pFilesToReload)
{
	if (Vars::Misc::BypassPure.m_Var)
		return;

	Func.Original<FN>()(pFilesToReload);
}