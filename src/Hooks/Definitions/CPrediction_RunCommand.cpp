#include "../Hooks.h"

DEFINE_HOOK(CPrediction_RunCommand, void, __fastcall, void* ecx, void* edx, C_BasePlayer* player, CUserCmd* ucmd, IMoveHelper* moveHelper)
{
	if (!I::MoveHelper && moveHelper)
		I::MoveHelper = moveHelper;

	Func.Original<FN>()(ecx, edx, player, ucmd, moveHelper);
}