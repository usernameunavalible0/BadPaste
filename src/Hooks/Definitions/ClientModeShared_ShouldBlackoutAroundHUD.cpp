#include "../Hooks.h"

DEFINE_HOOK(ClientModeShared_ShouldBlackoutAroundHUD, bool, __fastcall, void* ecx, void* edx)
{
	return Func.Original<FN>()(ecx, edx);
}