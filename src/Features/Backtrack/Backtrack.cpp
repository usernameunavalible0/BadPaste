#include "Backtrack.h"

void CFeatures_Backtrack::FrameUpdatePostEntityThink()
{
	if ((g_Globals.m_nMaxClients <= 1) || !G::ConVars.sv_unlag->GetBool())
	{
		ClearHistory();
		return;
	}

	m_flTeleportDistanceSqr = G::ConVars.sv_lagcompensation_teleport_dist->GetFloat() * G::ConVars.sv_lagcompensation_teleport_dist->GetFloat();

	int flDeadTime = I::GlobalVars->curtime - G::ConVars.sv_maxunlag->GetFloat();

	for (int i = 1; i <= g_Globals.m_nMaxClients; i++)
	{
		C_BasePlayer* pPlayer = UTIL_PlayerByIndex(i);

		CUtlFixedLinkedList<LagRecord>* track = &m_PlayerTrack[i - 1];

		if (!pPlayer)
		{
			if (track->Count() > 0)
			{
				track->RemoveAll();
			}

			continue;
		}

		//remove tail records that are too old
		int tailIndex = track->Tail();
		while (track->IsValidIndex(tailIndex))
		{
			LagRecord& tail = track->Element(tailIndex);

			if (tail.m_flSimulationTime >= flDeadTime)
				break;

			track->Remove(tailIndex);
			tailIndex = track->Tail();
		}

		//check if head has same simulation time
		if (track->Count() > 0)
		{
			LagRecord& head = track->Element(track->Head());

			if (head.m_flSimulationTime >= pPlayer->m_flSimulationTime())
				continue;
		}

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

		/*int layerCount = pPlayer->GetNumAnimOverlays();
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
		}*/
		record.m_masterSequence = pPlayer->m_nSequence();
		record.m_masterCycle = pPlayer->m_flCycle();
	}
}

void CFeatures_Backtrack::BacktrackPlayer(C_BasePlayer* pPlayer, float flTargetTime)
{
	Vector org;
	Vector minsPreScaled;
	Vector maxsPreScaled;
	QAngle ang;

	int pl_index = pPlayer->entindex() - 1;

	// get track history of this player
	CUtlFixedLinkedList< LagRecord >* track = &m_PlayerTrack[pl_index];

	// check if we have at least one entry
	if (track->Count() <= 0)
		return;

	int curr = track->Head();

	LagRecord* prevRecord = NULL;
	LagRecord* record = NULL;

	Vector prevOrg = pPlayer->m_vecOrigin();

	while (track->IsValidIndex(curr))
	{
		// remember last record
		prevRecord = record;

		record = &track->Element(curr);

		if (!(record->m_fFlags & LC_ALIVE))
			return;

		Vector delta = record->m_vecOrigin - prevOrg;
		if (delta.Length2DSqr() > m_flTeleportDistanceSqr)
			return;

		if (record->m_flSimulationTime <= flTargetTime)
			break;

		prevOrg = record->m_vecOrigin;

		curr = track->Next(curr);
	}

	if (!record)
		return;

	float frac = 0.0f;
	if (prevRecord &&
		(record->m_flSimulationTime < flTargetTime) &&
		(record->m_flSimulationTime < prevRecord->m_flSimulationTime))
	{
		frac = (flTargetTime - record->m_flSimulationTime) /
			(prevRecord->m_flSimulationTime - record->m_flSimulationTime);

		ang = Lerp(frac, record->m_vecAngles, prevRecord->m_vecAngles);
		org = Lerp(frac, record->m_vecOrigin, prevRecord->m_vecOrigin);
		minsPreScaled = Lerp(frac, record->m_vecMinsPreScaled, prevRecord->m_vecMinsPreScaled);
		maxsPreScaled = Lerp(frac, record->m_vecMaxsPreScaled, prevRecord->m_vecMaxsPreScaled);
	}
	else
	{
		org = record->m_vecOrigin;
		ang = record->m_vecAngles;
		minsPreScaled = record->m_vecMinsPreScaled;
		maxsPreScaled = record->m_vecMaxsPreScaled;
	}
}

void CFeatures_Backtrack::StartLagCompensation(C_BasePlayer* player, CUserCmd* cmd)
{
	Vector org;
	Vector minsPreScaled;
	Vector maxsPreScaled;
	QAngle ang;

	int targettick = cmd->tick_count;
	{
		m_RestorePlayer.ClearAll();
		m_bNeedToRestore = false;

		Q_memset(m_RestoreData, 0, sizeof(m_RestoreData));
		Q_memset(m_ChangeData, 0, sizeof(m_ChangeData));

		float correct = 0.0f;

		INetChannelInfo* nci = I::EngineClient->GetNetChannelInfo();

		if (nci)
		{
			correct += nci->GetLatency(FLOW_OUTGOING);
			correct += nci->GetLatency(FLOW_INCOMING);
		}

		//int lerpTicks = TIME_TO_TICKS(player->m_fLerpTime);

		//correct += TICKS_TO_TIME(lerpTicks);

		correct = clamp(correct, 0.0f, G::ConVars.sv_maxunlag->GetFloat());

		targettick = cmd->tick_count; //- lerpTicks

		float deltaTime = correct - TICKS_TO_TIME(I::GlobalVars->tickcount - targettick);

		if (fabsf(deltaTime) > 0.2f)
		{
			targettick = I::GlobalVars->tickcount - TIME_TO_TICKS(correct);
		}
	}

	/*float flTargetTime = TICKS_TO_TIME(targettick);
	{
		int pl_index = player->entindex() - 1;
		CUtlFixedLinkedList<LagRecord>* track = &m_PlayerTrack[pl_index];

		if (track->Count() <= 0)
			return;

		int curr = track->Head();

		LagRecord* prevRecord = NULL;
		LagRecord* record = NULL;

		Vector prevOrg = player->m_vecOrigin();

		while (track->IsValidIndex(curr))
		{
			prevRecord = record;

			record = &track->Element(curr);

			if (!(record->m_fFlags & LC_ALIVE))
				return;

			Vector delta = record->m_vecOrigin - prevOrg;
			if (delta.Length2DSqr() > m_flTeleportDistanceSqr)
				return;

			if (record->m_flSimulationTime <= flTargetTime)
				break;

			prevOrg = record->m_vecOrigin;

			curr = track->Next(curr);
		}

		if (!record)
			return;

		float frac = 0.0f;
		if (prevRecord &&
			(record->m_flSimulationTime < flTargetTime) &&
			(record->m_flSimulationTime < prevRecord->m_flSimulationTime))
		{
			frac = (flTargetTime - record->m_flSimulationTime) /
				(prevRecord->m_flSimulationTime - record->m_flSimulationTime);

			ang = Lerp(frac, record->m_vecAngles, prevRecord->m_vecAngles);
			org = Lerp(frac, record->m_vecOrigin, prevRecord->m_vecOrigin);
			minsPreScaled = Lerp(frac, record->m_vecMinsPreScaled, prevRecord->m_vecMinsPreScaled);
			maxsPreScaled = Lerp(frac, record->m_vecMaxsPreScaled, prevRecord->m_vecMaxsPreScaled);
		}
		else
		{
			org = record->m_vecOrigin;
			ang = record->m_vecAngles;
			minsPreScaled = record->m_vecMinsPreScaled;
			maxsPreScaled = record->m_vecMaxsPreScaled;
		}

		int flags = 0;
		LagRecord* restore = &m_RestoreData[pl_index];
		LagRecord* change = &m_ChangeData[pl_index];

		QAngle angdiff = player->m_angRotation() - ang;
		Vector orgdiff = player->m_vecOrigin() - org;

		restore->m_flSimulationTime = player->m_flSimulationTime();

		if (angdiff.LengthSqr() > LAG_COMPENSATION_EPS_SQR)
		{
			flags |= LC_ANGLES_CHANGED;
			restore->m_vecAngles = player->m_angRotation();
			player->m_angRotation() = ang;
			change->m_vecAngles = ang;
		}

		if (minsPreScaled != player->m_vecMinsPreScaled() || maxsPreScaled != player->m_vecMaxsPreScaled())
		{
			flags |= LC_SIZE_CHANGED;

			restore->m_vecMinsPreScaled = player->m_vecMinsPreScaled();
			restore->m_vecMaxsPreScaled = player->m_vecMaxsPreScaled();

			player->m_vecMinsPreScaled() = minsPreScaled;
			player->m_vecMaxsPreScaled() = maxsPreScaled;

			change->m_vecMinsPreScaled = minsPreScaled;
			change->m_vecMaxsPreScaled = maxsPreScaled;
		}

		if (orgdiff.LengthSqr() > LAG_COMPENSATION_EPS_SQR)
		{
			flags |= LC_ORIGIN_CHANGED;
			restore->m_vecOrigin = player->m_vecOrigin();
			player->m_vecOrigin() = org;
			change->m_vecOrigin = org;
		}

		//TODO: animations

		if (!flags)
			return;

		m_RestorePlayer.Set(pl_index);
		m_bNeedToRestore = true;
		restore->m_fFlags = flags;
		change->m_fFlags = flags;
	}*/
}

void CFeatures_Backtrack::FinishLagCompensation(C_BasePlayer* player)
{
	/*if (!m_bNeedToRestore)
		return;

	for (int i = 1; i <= g_Globals.m_nMaxClients; i++)
	{
		int pl_index = i - 1;

		if (!m_RestorePlayer.Get(pl_index))
			continue;

		C_BasePlayer* pPlayer = UTIL_PlayerByIndex(i);
		if (!pPlayer)
			continue;

		LagRecord* restore = &m_RestoreData[pl_index];
		LagRecord* change = &m_ChangeData[pl_index];

		bool restoreSimulationTime = false;

		if (restore->m_fFlags & LC_SIZE_CHANGED)
		{
			restoreSimulationTime = true;

			if (pPlayer->m_vecMinsPreScaled() == change->m_vecMinsPreScaled &&
				pPlayer->m_vecMaxsPreScaled() == change->m_vecMaxsPreScaled)
			{
				pPlayer->m_vecMinsPreScaled() = restore->m_vecMinsPreScaled;
				pPlayer->m_vecMaxsPreScaled() = restore->m_vecMaxsPreScaled;
			}
		}

		if (restore->m_fFlags & LC_ANGLES_CHANGED)
		{
			restoreSimulationTime = true;

			if (pPlayer->m_angRotation() == change->m_vecAngles)
			{
				pPlayer->m_angRotation() = restore->m_vecAngles;
			}
		}

		if (restore->m_fFlags & LC_ORIGIN_CHANGED)
		{
			restoreSimulationTime = true;

			Vector delta = pPlayer->m_vecOrigin() - change->m_vecOrigin;

			if (delta.Length2DSqr() < m_flTeleportDistanceSqr)
				pPlayer->m_vecOrigin() = restore->m_vecOrigin;
		}

		//TODO: Animations

		if (restoreSimulationTime)
			pPlayer->m_flSimulationTime() = restore->m_flSimulationTime;
	}*/
}
