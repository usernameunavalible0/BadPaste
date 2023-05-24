#include "../Hooks.h"

DEFINE_HOOK(ClientModeShared_DoPostScreenSpaceEffects, bool, __fastcall, void* ecx, void* edx, const CViewSetup* pSetup)
{
	return Func.Original<FN>()(ecx, edx, pSetup);
}