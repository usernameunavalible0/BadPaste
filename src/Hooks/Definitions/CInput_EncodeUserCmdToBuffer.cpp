#include "../Hooks.h"

DEFINE_HOOK(CInput_EncodeUserCmdToBuffer, void, __fastcall, void* ecx, void* edx, bf_write& buf, int slot)
{
	Func.Original<FN>()(ecx, edx, buf, slot);
}