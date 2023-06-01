#include "../Hooks.h"
#include "../../Features/Visual/Visual.h"

DEFINE_HOOK(IBaseClientDLL_FrameStageNotify, void, __fastcall, void* ecx, void* edx, ClientFrameStage_t curStage)
{
	Func.Original<FN>()(ecx, edx, curStage);

	if (curStage == ClientFrameStage_t::FRAME_NET_UPDATE_START)
	{
		G::EntityCache.Clear();
	}

	if (curStage == ClientFrameStage_t::FRAME_NET_UPDATE_END)
	{
		g_Globals.m_bIsInGame = I::EngineClient->IsInGame();
		g_Globals.m_bIsGameUIVisible = I::EngineVGui->IsGameUIVisible();

		if (g_Globals.m_bIsInGame)
		{
			g_Globals.m_nMaxClients = I::EngineClient->GetMaxClients();
			g_Globals.m_nMaxEntities = I::ClientEntityList->GetMaxEntities();
			G::EntityCache.Fill();
		}
	}

	if (curStage == ClientFrameStage_t::FRAME_RENDER_START)
	{
		F::Visual.UpdateWorldModulation();
	}
}