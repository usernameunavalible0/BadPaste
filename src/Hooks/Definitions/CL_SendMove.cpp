#include "../Hooks.h"

DEFINE_HOOK(CL_SendMove, void, __fastcall, void* ecx, void* edx)
{
	Func.Original<FN>()(ecx, edx);
}