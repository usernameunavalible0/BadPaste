#include "../Hooks.h"
#include "../../Features/ESP/ESP.h"

DEFINE_HOOK(IBaseClientDLL_LevelInitPostEntity, void, __fastcall, void* ecx, void* edx)
{
	Func.Original<FN>()(ecx, edx);

	g_Globals.m_nLocalIndex = I::EngineClient->GetLocalPlayer();
	F::ESP.LevelInitPostEntity();
}