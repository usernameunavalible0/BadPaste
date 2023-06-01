#include "../Hooks.h"

#include "../../Features/Visual/Visual.h"

DEFINE_HOOK(IBaseClientDLL_LevelInitPreEntity, void, __fastcall, void* ecx, void* edx, char const* pMapName)
{
	Func.Original<FN>()(ecx, edx, pMapName);

	F::Visual.ModulateWorld();
}