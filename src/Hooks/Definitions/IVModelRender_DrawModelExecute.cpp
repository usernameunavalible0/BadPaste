#include "../Hooks.h"

#include "../../Features/Chams/Chams.h"

DEFINE_HOOK(IVModelRender_DrawModelExecute, void, __fastcall, void* ecx, void* edx, const DrawModelState_t& state, const ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld)
{
	if (F::Chams.Render(ecx, edx, state, pInfo, pCustomBoneToWorld))
		return;

	Func.Original<FN>()(ecx, edx, state, pInfo, pCustomBoneToWorld);
}