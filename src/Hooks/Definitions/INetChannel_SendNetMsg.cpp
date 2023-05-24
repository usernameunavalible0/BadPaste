#include "../Hooks.h"

DEFINE_HOOK(INetChannel_SendNetMsg, bool, __fastcall, CNetChan* pThis, void* edx, INetMessage& msg, bool bForceReliable, bool bVoice)
{
	return Func.Original<FN>()(pThis, edx, msg, bForceReliable, bVoice);
}