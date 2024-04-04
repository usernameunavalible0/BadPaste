#include "../Hooks.h"
#include "../../Features/Vars.h"

DEFINE_HOOK(CL_Move, void, __cdecl, float accumulated_extra_samples, bool bFinalTick)
{
	Func.Original<FN>()(accumulated_extra_samples, bFinalTick);
}