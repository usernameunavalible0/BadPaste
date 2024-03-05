#include "../Hooks.h"
#include "../../Features/Glow/Glow.h"

DEFINE_HOOK(ClientModeShared_DoPostScreenSpaceEffects, bool, __fastcall, void* ecx, void* edx, const CViewSetup* pSetup)
{
	F::Glow.Render();
	return Func.Original<FN>()(ecx, edx, pSetup);
}