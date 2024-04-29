#include "../Hooks.h"
#include "../../Features/Visual/Visual.h"
#include "../../Features/Backtrack/Backtrack.h"

DEFINE_HOOK(IBaseClientDLL_FrameStageNotify, void, __fastcall, void* ecx, void* edx, ClientFrameStage_t curStage)
{
	if (curStage == ClientFrameStage_t::FRAME_RENDER_START)
	{
		C_TFPlayer* pLocal = G::EntityCache.GetLocal();
		if (pLocal)
			F::Visual.Thirdperson(pLocal);
	}

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

			F::Backtrack.PostDataUpdate();

			g_Globals.m_bIsBeingSpectated = false;
			if (C_TFPlayer* pLocal = G::EntityCache.GetLocal())
			{
				for (auto pEntity : G::EntityCache.GetGroup(EEntGroup::PLAYERS_TEAMMATES))
				{
					C_TFPlayer* pTeammate = pEntity->As<C_TFPlayer*>();

					if (pTeammate->IsAlive() || pTeammate->IsPlayerOnSteamFriendsList())
						continue;

					C_BaseEntity* pObservedPlayer = pTeammate->GetObserverTarget();

					if (pObservedPlayer == pLocal)
					{
						switch (pTeammate->GetObserverMode())
						{
						case OBS_MODE_IN_EYE:
						case OBS_MODE_CHASE:
							break;
						default: continue;
						}

						g_Globals.m_bIsBeingSpectated = true;
						break;
					}
				}
			}
		}
	}

	if (curStage == ClientFrameStage_t::FRAME_RENDER_START)
	{
		F::Visual.UpdateWorldModulation();
	}
}