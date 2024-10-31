#include "../Hooks.h"

DEFINE_HOOK(CTFGameMovement_ProcessMovement, void, __fastcall, void* ecx, void* edx, C_BasePlayer* pBasePlayer, CMoveData* pMove)
{
	if (!I::TFGameMovement)
		I::TFGameMovement = ecx;

	Func.Original<FN>()(ecx, edx, pBasePlayer, pMove);
}