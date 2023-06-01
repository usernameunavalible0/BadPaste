#include "../Hooks.h"

#include "../../Features/Vars.h"

DEFINE_HOOK(ClientModeShared_ShouldDrawViewModel, bool, __fastcall, void* ecx, void* edx)
{
	if (C_TFPlayer* pLocal = G::EntityCache.GetLocal())
	{
		if (pLocal->IsZoomed() && Vars::Visual::RemoveScope.m_Var && Vars::Visual::RemoveZoom.m_Var && !I::Input->CAM_IsThirdPerson())
			return true;
	}

	return Func.Original<FN>()(ecx, edx);
}