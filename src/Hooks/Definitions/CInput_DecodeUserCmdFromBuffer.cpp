#include "../Hooks.h"

DEFINE_HOOK(CInput_DecodeUserCmdFromBuffer, void, __fastcall, void* ecx, void* edx, bf_read& buf, int slot)
{
	Func.Original<FN>()(ecx, edx, buf, slot);
}