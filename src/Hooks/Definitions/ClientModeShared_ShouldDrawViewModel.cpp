#include "../Hooks.h"

DEFINE_HOOK(ClientModeShared_ShouldDrawViewModel, bool, __fastcall, void* ecx, void* edx)
{
	return Func.Original<FN>()(ecx, edx);
}