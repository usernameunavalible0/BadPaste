#include "../Hooks.h"

DEFINE_HOOK(IClientEntityList_OnRemoveEntity, void, __fastcall, void* ecx, void* edx, IHandleEntity* pEnt, CBaseHandle handle)
{
	Func.Original<FN>()(ecx, edx, pEnt, handle);
}