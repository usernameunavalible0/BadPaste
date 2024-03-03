#include "Backtrack.h"

void CFeatures_Backtrack::FrameUpdatePostEntityThink()
{
	if ((I::GlobalVars->maxClients <= 1) || !G::ConVars.sv_unlag->GetBool())
	{
		ClearHistory();
		return;
	}

	m_flTeleportDistanceSqr = G::ConVars.sv_lagcompensation_teleport_dist->GetFloat() * G::ConVars.sv_lagcompensation_teleport_dist->GetFloat();

	// remove all records before that time:
	int flDeadtime = I::GlobalVars->curtime - G::ConVars.sv_maxunlag->GetFloat();

	// Iterate all active players
	for (int i = 1; i <= I::GlobalVars->maxClients; i++)
	{
		C_BasePlayer* pPlayer = UTIL_PlayerByIndex(i);

		CUtlFixedLinkedList< LagRecord >* track = &m_PlayerTrack[i - 1];

		if (!pPlayer)
		{
			if (track->Count() > 0)
			{
				track->RemoveAll();
			}

			continue;
		}

		//Assert(track->Count() < 1000); // insanity check

		// remove tail records that are too old
		int tailIndex = track->Tail();
		while (track->IsValidIndex(tailIndex))
		{
			LagRecord& tail = track->Element(tailIndex);

			// if tail is within limits, stop
			if (tail.m_flSimulationTime >= flDeadtime)
				break;

			// remove tail, get new tail
			track->Remove(tailIndex);
			tailIndex = track->Tail();
		}

		// check if head has same simulation time
		if (track->Count() > 0)
		{
			LagRecord& head = track->Element(track->Head());

			// check if player changed simulation time since last time updated
			if (head.m_flSimulationTime >= pPlayer->m_flSimulationTime())
				continue; // don't add new entry for same or older time
		}

		// add new record to player track
		LagRecord& record = track->Element(track->AddToHead());

		record.m_fFlags = 0;
		if (pPlayer->IsAlive())
		{
			record.m_fFlags |= LC_ALIVE;
		}

		record.m_flSimulationTime = pPlayer->m_flSimulationTime();
		record.m_vecAngles = pPlayer->m_angRotation();
		record.m_vecOrigin = pPlayer->m_vecOrigin();
		record.m_vecMinsPreScaled = pPlayer->m_vecMinsPreScaled();
		record.m_vecMaxsPreScaled = pPlayer->m_vecMaxsPreScaled();

		int layerCount = pPlayer->GetNumAnimOverlays();
		for (int layerIndex = 0; layerIndex < layerCount; ++layerIndex)
		{
			C_AnimationLayer* currentLayer = pPlayer->GetAnimOverlay(layerIndex);
			if (currentLayer)
			{
				record.m_layerRecords[layerIndex].m_cycle = currentLayer->m_flCycle;
				record.m_layerRecords[layerIndex].m_order = currentLayer->m_nOrder;
				record.m_layerRecords[layerIndex].m_sequence = currentLayer->m_nSequence;
				record.m_layerRecords[layerIndex].m_weight = currentLayer->m_flWeight;
			}
		}
		record.m_masterSequence = pPlayer->m_nSequence();
		record.m_masterCycle = pPlayer->m_flCycle();
	}

	//Clear the current player.
	m_pCurrentPlayer = NULL;
}

void CFeatures_Backtrack::DrawLagRecordsBasic()
{
	for (int i = 1; i <= g_Globals.m_nMaxClients; i++)
	{
		C_BasePlayer* pPlayer = UTIL_PlayerByIndex(i);

		if (!pPlayer || (pPlayer->entindex() == g_Globals.m_nLocalIndex)) // || pPlayer->InLocalTeam()
			continue;

		// get track history of this player
		CUtlFixedLinkedList< LagRecord >* track = &m_PlayerTrack[i - 1];

		// check if we have at leat one entry
		if (track->Count() <= 0)
			continue;

		int curr = track->Head();

		LagRecord* record = NULL;

		Vector prevOrg = pPlayer->m_vecOrigin();

		// Walk context looking for any invalidating event
		while (track->IsValidIndex(curr))
		{
			// get next record
			record = &track->Element(curr);

			//We dont want to draw records on dead players
			if (!(record->m_fFlags & LC_ALIVE))
				break;

			Vector delta = record->m_vecOrigin - prevOrg;
			if (delta.Length2DSqr() > m_flTeleportDistanceSqr)
				break;

			//NOTE: This should be used for more accurate representation of useable records but we dont not have access to CUserCmd in IEngineVGui::Paint.
			// Maybe we can store that target time some other way...
			//if (record->m_flSimulationTime <= flTargetTime)
			//	break;

			prevOrg = record->m_vecOrigin;

			// go one step back
			curr = track->Next(curr);

			Vector2D screenPos;
			if (GetVectorInScreenSpace(record->m_vecOrigin, screenPos))
				G::Draw.Rect(screenPos.x, screenPos.y, 2, 2, COLOR_GREEN);
		}
	}
}