#include "Backtrack.h"

void CFeatures_Backtrack::PostDataUpdate()
{
	if ((I::GlobalVars->maxClients <= 1) || !G::ConVars.sv_unlag->GetBool())
	{
		ClearHistory();
		return;
	}

	// NOTE: Do we *really* need to be setting this on every net update?
	m_flTeleportDistanceSqr = G::ConVars.sv_lagcompensation_teleport_dist->GetFloat() * G::ConVars.sv_lagcompensation_teleport_dist->GetFloat();

	// remove all records before that time:
	int flDeadtime = I::GlobalVars->curtime - G::ConVars.sv_maxunlag->GetFloat();

	// Iterate all active players
	for (int i = 1; i <= I::GlobalVars->maxClients; i++)
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
		record.m_vecMinsPreScaled = pPlayer->m_vecMinsPreScaled(); //pPlayer->m_Collision()->OBBMinsPreScaled();
		record.m_vecMaxsPreScaled = pPlayer->m_vecMaxsPreScaled(); //pPlayer->m_Collision()->OBBMaxsPreScaled();

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
}

float g_flFractionScale = 0.95;
static void RestorePlayerTo(C_BasePlayer* pPlayer, const Vector& vWantedPos)
{
	// Try to move to the wanted position from our current position.
	trace_t tr;
	UTIL_TraceEntity(pPlayer, vWantedPos, vWantedPos, MASK_PLAYERSOLID, pPlayer, COLLISION_GROUP_PLAYER_MOVEMENT, &tr);
	if (tr.startsolid || tr.allsolid)
	{
		UTIL_TraceEntity(pPlayer, pPlayer->m_vecOrigin(), vWantedPos, MASK_PLAYERSOLID, pPlayer, COLLISION_GROUP_PLAYER_MOVEMENT, &tr);
		if (tr.startsolid || tr.allsolid)
		{
			// In this case, the guy got stuck back wherever we lag compensated him to. Nasty.
		}
		else
		{
			// We can get to a valid place, but not all the way back to where we were.
			Vector vPos;
			VectorLerp(pPlayer->m_vecOrigin(), vWantedPos, tr.fraction * g_flFractionScale, vPos);
			//UTIL_SetOrigin(pPlayer, vPos, true);
			pPlayer->SetAbsOrigin(vPos);
		}
	}
	else
	{
		// Cool, the player can go back to whence he came.
		//UTIL_SetOrigin(pPlayer, tr.endpos, true);
		pPlayer->SetAbsOrigin(tr.endpos);
	}
}

// In my example we are only backtracking one player at a time,
// but this function can be easily modified to remove pPlayer
// and loop through player list. (Especially since FinishBacktrack is setup to handle multiple backtracked players)
void CFeatures_Backtrack::StartBacktracking(C_BasePlayer* pPlayer, CUserCmd* cmd, std::function<bool()> func)
{
	if (I::GlobalVars->maxClients <= 1 || !G::ConVars.sv_unlag->GetBool())
		return;

	memset(m_RestoreData, 0, sizeof(m_RestoreData));
	memset(m_ChangeData, 0, sizeof(m_ChangeData));

	// Tell anyone who needs to know that we are currently backtracking
	m_bIsCurrentlyBacktracking = true;

	// get track history of this player
	CUtlFixedLinkedList< LagRecord >* track = &m_PlayerTrack[pPlayer->entindex()-1];

	// check if we have at leat one entry
	if (track->Count() <= 0)
		return;

	int curr = track->Head();

	LagRecord* record = NULL;

	Vector prevOrg = pPlayer->m_vecOrigin();

	// Walk context looking for any invalidating event
	while (track->IsValidIndex(curr))
	{
		// get next record
		record = &track->Element(curr);

		if (!(record->m_fFlags & LC_ALIVE))
		{
			// player most be alive, lost track
			return;
		}

		Vector delta = record->m_vecOrigin - prevOrg;
		if (delta.Length2DSqr() > m_flTeleportDistanceSqr)
		{
			// lost track, too much difference
			return;
		}

		// NOTE: This stops looping through records when time diff
		// is bigger than 0.2f (200ms). If we artificially increase latency
		// this check must be modified to match.
		if (fabsf(pPlayer->m_flSimulationTime() - record->m_flSimulationTime) > 0.2f)
			break; // end of the line

		BacktrackPlayer(pPlayer, record);

		// Check for custom condition
		if (func())
		{
			cmd->tick_count = TIME_TO_TICKS(record->m_flSimulationTime + G::Util.GetClientInterpAmount());
			return;
		}

		prevOrg = record->m_vecOrigin;

		// go one step back
		curr = track->Next(curr);
	}
}

void CFeatures_Backtrack::FinishBacktracking()
{
	// We are done here
	m_bIsCurrentlyBacktracking = false;

	if (!m_bNeedToRestore)
		return; // no player was changed at all

	// Iterate all active players
	for (int i = 1; i <= I::GlobalVars->maxClients; i++)
	{
		int pl_index = i - 1;

		if (!m_RestorePlayer.Get(pl_index))
			continue; // player wasn't changed by backtrack

		C_BasePlayer* pPlayer = UTIL_PlayerByIndex(i);

		if (!pPlayer)
			continue;

		LagRecord* restore = &m_RestoreData[pl_index];
		LagRecord* change = &m_ChangeData[pl_index];

		bool restoreSimulationTime = false;

		// TODO: FIXME!! Setting collision bounds crashes the game.
		if (restore->m_fFlags & LC_SIZE_CHANGED)
		{
			restoreSimulationTime = true;

			// see if simulation made any changes, if no, then do the restore, otherwise,
			//  leave new values in
			if (pPlayer->m_vecMinsPreScaled() == change->m_vecMinsPreScaled && //pPlayer->m_Collision()->OBBMinsPreScaled()
				pPlayer->m_vecMaxsPreScaled() == change->m_vecMaxsPreScaled)	//pPlayer->m_Collision()->OBBMaxsPreScaled()
			{
				// Restore it
				pPlayer->SetCollisionBounds(restore->m_vecMinsPreScaled, restore->m_vecMaxsPreScaled);
			}
		}

		if (restore->m_fFlags & LC_ANGLES_CHANGED)
		{
			restoreSimulationTime = true;

			if (pPlayer->m_angRotation() == change->m_vecAngles)
				pPlayer->SetAbsAngles(restore->m_vecAngles);
		}

		if (restore->m_fFlags & LC_ORIGIN_CHANGED)
		{
			restoreSimulationTime = true;

			Vector delta = pPlayer->m_vecOrigin() - change->m_vecOrigin;

			if (delta.Length2DSqr() < m_flTeleportDistanceSqr)
				RestorePlayerTo(pPlayer, restore->m_vecOrigin + delta);
		}

		if (restore->m_fFlags & LC_ANIMATION_CHANGED)
		{
			restoreSimulationTime = true;

			pPlayer->SetSequence(restore->m_masterSequence);
			pPlayer->SetCycle(restore->m_masterCycle);

			int layerCount = pPlayer->GetNumAnimOverlays();
			for (int layerIndex = 0; layerIndex < layerCount; ++layerIndex)
			{
				C_AnimationLayer* currentLayer = pPlayer->GetAnimOverlay(layerIndex);
				if (currentLayer)
				{
					currentLayer->m_flCycle = restore->m_layerRecords[layerIndex].m_cycle;
					currentLayer->m_nOrder = restore->m_layerRecords[layerIndex].m_order;
					currentLayer->m_nSequence = restore->m_layerRecords[layerIndex].m_sequence;
					currentLayer->m_flWeight = restore->m_layerRecords[layerIndex].m_weight;
				}
			}
		}

		if (restoreSimulationTime)
			pPlayer->m_flSimulationTime() = restore->m_flSimulationTime;
	}
}

void CFeatures_Backtrack::DrawLagRecordsBasic()
{
	for (int i = 1; i <= I::GlobalVars->maxClients; i++)
	{
		C_BasePlayer* pPlayer = UTIL_PlayerByIndex(i);

		if (!pPlayer || pPlayer->entindex() == g_Globals.m_nLocalIndex)
			continue;

		int pl_index = pPlayer->entindex() - 1;

		// get track history of this player
		CUtlFixedLinkedList< LagRecord >* track = &m_PlayerTrack[pl_index];

		// check if we have at least one entry
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

			if (!(record->m_fFlags & LC_ALIVE))
			{
				// player most be alive, lost track
				break;
			}

			Vector delta = record->m_vecOrigin - prevOrg;
			if (delta.Length2DSqr() > m_flTeleportDistanceSqr)
			{
				// lost track, too much difference
				break;
			}

			// NOTE: In this example we dont extend latency so this check is done using 0.2f (200ms) as the cap
			// Interpolation is subtracted to get the client time because according to valve
			// While rendering, this is the exact client clock [client_current_tick * tick_interval + interpolation_amount]
			if (fabs((I::GlobalVars->curtime - G::Util.GetClientInterpAmount()) - record->m_flSimulationTime) > 0.2f)
				break; // end of the line

			prevOrg = record->m_vecOrigin;

			// go one step back
			curr = track->Next(curr);

			Vector2D screenPos;
			if (GetVectorInScreenSpace(record->m_vecOrigin, screenPos))
				G::Draw.Rect(screenPos.x, screenPos.y, 2, 2, COLOR_GREEN);
		}
	}
}

void CFeatures_Backtrack::BacktrackPlayer(C_BasePlayer* pPlayer, LagRecord* record)
{
	//This sould never happen
	if (!pPlayer || !record)
		return;

	int pl_index = pPlayer->entindex() - 1;

	Vector org = record->m_vecOrigin;
	QAngle ang = record->m_vecAngles;
	Vector minsPreScaled = record->m_vecMinsPreScaled;
	Vector maxsPreScaled = record->m_vecMaxsPreScaled;

	// TODO: sv_unlag_fixstuck

	// See if this represents a change for the player
	int flags = 0;
	LagRecord* restore = &m_RestoreData[pl_index];
	LagRecord* change = &m_ChangeData[pl_index];

	//QAngle angdiff = pPlayer->m_angRotation() - ang;
	//Vector orgdiff = pPlayer->m_vecOrigin() - org;

	// remember simulation time in case we need to restore it
	restore->m_flSimulationTime = pPlayer->m_flSimulationTime();

	//if (angdiff.LengthSqr() > LAG_COMPENSATION_EPS_SQR)
	{
		flags |= LC_ANGLES_CHANGED;
		restore->m_vecAngles = pPlayer->m_angRotation();
		// Must be SetAbsAngles and not SetLocalAngles like in the server code
		pPlayer->SetAbsAngles(ang);
		change->m_vecAngles = ang;
	}

	// Use absolute equality here
	/*
	if (minsPreScaled != pPlayer->m_Collision()->OBBMinsPreScaled() || maxsPreScaled != pPlayer->m_Collision()->OBBMaxsPreScaled())
	{
		flags |= LC_SIZE_CHANGED;

		restore->m_vecMinsPreScaled = pPlayer->m_Collision()->OBBMinsPreScaled();
		restore->m_vecMaxsPreScaled = pPlayer->m_Collision()->OBBMaxsPreScaled();

		pPlayer->SetCollisionBounds(minsPreScaled, maxsPreScaled);

		change->m_vecMinsPreScaled = minsPreScaled;
		change->m_vecMaxsPreScaled = maxsPreScaled;
	}*/

	// Note, do origin at end since it causes a relink into the k/d tree
	//if (orgdiff.LengthSqr() > LAG_COMPENSATION_EPS_SQR)
	{
		flags |= LC_ORIGIN_CHANGED;
		restore->m_vecOrigin = pPlayer->m_vecOrigin();
		// Must be SetAbsOrigin and not SetLocalOrigin like in the server code
		pPlayer->SetAbsOrigin(org);
		change->m_vecOrigin = org;
	}

	/* TODO: FIXME!! Setting animations causes frozen animations on players
	// Sorry for the loss of the optimization for the case of people
	// standing still, but you breathe even on the server.
	// This is quicker than actually comparing all bazillion floats.
	flags |= LC_ANIMATION_CHANGED;
	restore->m_masterSequence = pPlayer->m_nSequence();
	restore->m_masterCycle = pPlayer->m_flCycle();
	pPlayer->SetSequence(record->m_masterSequence);
	pPlayer->SetCycle(record->m_masterCycle);

	////////////////////////
	// Now do all the layers
	int layerCount = pPlayer->GetNumAnimOverlays();
	for (int layerIndex = 0; layerIndex < layerCount; ++layerIndex)
	{
		C_AnimationLayer* currentLayer = pPlayer->GetAnimOverlay(layerIndex);
		if (currentLayer)
		{
			restore->m_layerRecords[layerIndex].m_cycle = currentLayer->m_flCycle;
			restore->m_layerRecords[layerIndex].m_order = currentLayer->m_nOrder;
			restore->m_layerRecords[layerIndex].m_sequence = currentLayer->m_nSequence;
			restore->m_layerRecords[layerIndex].m_weight = currentLayer->m_flWeight;

			currentLayer->m_flCycle = record->m_layerRecords[layerIndex].m_cycle;
			currentLayer->m_nOrder = record->m_layerRecords[layerIndex].m_order;
			currentLayer->m_nSequence = record->m_layerRecords[layerIndex].m_sequence;
			currentLayer->m_flWeight = record->m_layerRecords[layerIndex].m_weight;
		}
	}
	*/

	if (!flags)
		return; // we didnt change anything

	//if (G::ConVars.sv_lagflushbonecache->GetBool())
	pPlayer->InvalidateBoneCache();

	m_RestorePlayer.Set(pl_index); // remember that we changed this player
	m_bNeedToRestore = true; // we have changed at least one player
	restore->m_fFlags = flags; // we need to restore these flags
	change->m_fFlags = flags; // we have changed these flags
}
