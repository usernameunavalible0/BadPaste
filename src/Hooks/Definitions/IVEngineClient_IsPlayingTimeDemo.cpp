#include "../Hooks.h"

DEFINE_HOOK(IVEngineClient_IsPlayingTimeDemo, bool, __fastcall, void* ecx, void* edx)
{
	return Func.Original<FN>()(ecx, edx);
}