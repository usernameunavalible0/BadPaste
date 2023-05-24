#include "../Hooks.h"

DEFINE_HOOK(CViewRender_PerformScreenSpaceEffects, void, __fastcall, void* ecx, void* edx, int x, int y, int w, int h)
{
	Func.Original<FN>()(ecx, edx, x, y, w, h);
}