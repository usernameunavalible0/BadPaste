#include "../Hooks.h"
#include "../../Features/Vars.h"

DEFINE_HOOK(CClientState_ProcessFixAngle, bool, __fastcall, void* ecx, void* edx, SVC_FixAngle* msg)
{
	if (Vars::Misc::NoServerChangeAngle.m_Var)
		return true;

	return Func.Original<FN>()(ecx, edx, msg);
}