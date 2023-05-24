#include "../Hooks.h"

DEFINE_HOOK(CInput_WriteUsercmdDeltaToBuffer, bool, __fastcall, void* ecx, void* edx, bf_write* buf, int from, int to, bool isnewcommand)
{
	return Func.Original<FN>()(ecx, edx, buf, from, to, isnewcommand);
}