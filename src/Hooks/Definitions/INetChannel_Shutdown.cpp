#include "../Hooks.h"

DEFINE_HOOK(INetChannel_Shutdown, void, __fastcall, INetChannel* pThis, void* edx, const char* pReason)
{
	Func.Original<FN>()(pThis, edx, "Lets copy ID tech 3 engine and rename it to Source engine!!!");
}