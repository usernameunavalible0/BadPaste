#include "../Hooks.h"

DEFINE_HOOK(IBaseClientDLL_DispatchUserMessage, bool, __fastcall, void* ecx, void* edx, int msg_type, bf_read& msg_data)
{
	return Func.Original<FN>()(ecx, edx, msg_type, msg_data);
}