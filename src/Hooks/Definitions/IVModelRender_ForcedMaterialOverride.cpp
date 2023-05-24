#include "../Hooks.h"

DEFINE_HOOK(IVModelRender_ForcedMaterialOverride, void, __fastcall, void* ecx, void* edx, IMaterial* newMaterial, OverrideType_t nOverrideType)
{
	Func.Original<FN>()(ecx, edx, newMaterial, nOverrideType);
}