#include "../Hooks.h"

DEFINE_HOOK(ClientModeShared_OverrideView, void, __fastcall, void* ecx, void* edx, CViewSetup* pSetup)
{
	Func.Original<FN>()(ecx, edx, pSetup);
}