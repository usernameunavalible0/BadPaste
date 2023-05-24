#include "../Hooks.h"

DEFINE_HOOK(ClientModeShared_GetViewModelFOV, float, __fastcall, void* ecx, void* edx)
{
	return Func.Original<FN>()(ecx, edx);
}