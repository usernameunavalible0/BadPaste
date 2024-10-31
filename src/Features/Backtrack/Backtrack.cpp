#include "Backtrack.h"

// Called once every network update to update the lag record list
void CFeatures_Backtrack::OnNetUpdate()
{
	if ((g_Globals.m_nMaxClients <= 1) || G::ConVars.sv_unlag->GetBool())
	{
		//TODO: Fix this? Clears history even if track already empty.
		ClearHistory();
		return;
	}

	// Remove all records before this time:
	const float flDeadTime = I::GlobalVars->curtime - G::ConVars.sv_maxunlag->GetFloat();

	// Iterate all active players
	for (int i = 1; i < (g_Globals.m_nMaxClients + 1); i++)
	{
		C_TFPlayer* pPlayer = UTIL_TFPlayerByIndex(i);

		CUtlFixedLinkedList<LagRecord>* track = &m_PlayerTrack[i - 1];

		if (!pPlayer)
		{
			if (track->Count() > 0)
				track->RemoveAll();

			continue;
		}

		// Remove tail records that are too old
		int tailIndex = track->Tail();
		while (track->IsValidIndex(tailIndex))
		{
			LagRecord& tail = track->Element(tailIndex);

			// If tail is within limits, stop
			if (tail.m_flSimulationTime >= flDeadTime)
				break;

			// Remove tail, get new tail
			track->Remove(tailIndex);
			tailIndex = track->Tail();
		}

		// Check if head has same simulation time
		if (track->Count() > 0)
		{
			LagRecord& head = track->Element(track->Head());

			// Check if player changed simulation time since last time updated
			if (head.m_flSimulationTime >= pPlayer->m_flSimulationTime())
				continue; // Don't add new entry for same or older time
		}

		// Add new record to player track
		LagRecord& record = track->Element(track->AddToHead());

		record.m_flSimulationTime = pPlayer->m_flSimulationTime();
		record.m_vecAngles = pPlayer->GetAbsAngles();
		record.m_vecOrigin = pPlayer->GetAbsOrigin();
	}
}

void CFeatures_Backtrack::DrawSimple()
{
	for (int i = 1; i < (g_Globals.m_nMaxClients + 1); i++)
	{
		if (i == g_Globals.m_nLocalIndex)
			continue;

		C_TFPlayer* pPlayer = UTIL_TFPlayerByIndex(i);

		if (!pPlayer)
			continue;

		CUtlFixedLinkedList< LagRecord >* track = &m_PlayerTrack[i - 1];

		if (track->Count() <= 0)
			continue;

		int curr = track->Head();

		LagRecord* record = NULL;

		while (track->IsValidIndex(curr))
		{
			record = &track->Element(curr);

			//Vector delta = record->ve
		}
	}
}
