#include "../Hooks.h"

DEFINE_HOOK(IClientEntityList_OnAddEntity, void, __fastcall, void* ecx, void* edx, IHandleEntity* pEnt, CBaseHandle handle)
{
	Func.Original<FN>()(ecx, edx, pEnt, handle);
}