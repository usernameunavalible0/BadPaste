#include "../Hooks.h"

DEFINE_HOOK(INetChannel_SendDataGram, int, __fastcall, INetChannel* pThis, void* edx, bf_write* data)
{
	return Func.Original<FN>()(pThis, edx, data);
}