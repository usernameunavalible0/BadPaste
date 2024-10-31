#include "Backtrack.h"

void CFeatures_Backtrack::PostDataUpdate()
{
	if ((g_Globals.m_nMaxClients <= 1) || !G::ConVars.sv_unlag->GetBool())
	{
		ClearHistory();
		return;
	}

	// NOTE: Do we *really* need to be setting this on every net update?
	m_flTeleportDistanceSqr = G::ConVars.sv_lagcompensation_teleport_dist->GetFloat() * G::ConVars.sv_lagcompensation_teleport_dist->GetFloat();

	// remove all records before that time:
	int flDeadtime = I::GlobalVars->curtime - G::ConVars.sv_maxunlag->GetFloat();

	// Iterate all active players
	for (int i = 1; i < (g_Globals.m_nMaxClients + 1); i++)
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

		// Save data we want to use for 
		record.m_flSimulationTime = pPlayer->m_flSimulationTime();
		record.m_vecAngles = pPlayer->GetAbsAngles();
		record.m_vecOrigin = pPlayer->GetAbsOrigin();
		record.m_vecMinsPreScaled = pPlayer->m_vecMinsPreScaled(); //pPlayer->m_Collision()->OBBMinsPreScaled();
		record.m_vecMaxsPreScaled = pPlayer->m_vecMaxsPreScaled(); //pPlayer->m_Collision()->OBBMaxsPreScaled();
		// NOTE: Here I'm using BONE_USED_BY_HITBOX because when backtracking for aimbot you are most likely going to be looking for a hitbox bone.
		// But, if you want to use the bone matrix for anything other than that (eg. backtrack chams) you want to use BONE_USED_BY_ANYTHING instead.
		if (!pPlayer->SetupBones(record.m_CachedBoneData, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, I::GlobalVars->curtime))
		{
			//Whoops, we failed this one!
			//Lets remove it from the track.
			track->Remove(track->Head());
		}
	}
}

// In my example we are only backtracking one player at a time,
// but this function can be easily modified to remove pPlayer
// and loop through player list. (Especially since FinishBacktrack is setup to handle multiple backtracked players)
void CFeatures_Backtrack::StartBacktracking(C_BasePlayer* pPlayer, CUserCmd* cmd, std::function<bool()> func)
{
	if (g_Globals.m_nMaxClients <= 1 || !G::ConVars.sv_unlag->GetBool())
		return;

	memset(m_RestoreData, 0, sizeof(m_RestoreData));
	memset(m_ChangeData, 0, sizeof(m_ChangeData));

	// get track history of this player
	CUtlFixedLinkedList< LagRecord >* track = &m_PlayerTrack[pPlayer->entindex()-1];

	// check if we have at leat one entry
	if (track->Count() <= 0)
		return;

	int curr = track->Head();

	LagRecord* record = NULL;

	Vector prevOrg = pPlayer->GetAbsOrigin();

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
			//We have a winner!!! Set the tick count so the server knows whats up.
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
	if (!m_bNeedToRestore)
		return; // no player was changed at all

	// Iterate all active players
	for (int i = 1; i < (g_Globals.m_nMaxClients + 1); i++)
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

		// We always set bone matrix, so always restore bone matrix.
		{
			//memcpy(pPlayer->m_CachedBoneData().Base(), restore->m_CachedBoneData, sizeof(matrix3x4_t) * pPlayer->m_CachedBoneData().Count());
		}

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

			if (pPlayer->GetAbsAngles() == change->m_vecAngles)
				pPlayer->SetAbsAngles(restore->m_vecAngles);
		}

		if (restore->m_fFlags & LC_ORIGIN_CHANGED)
		{
			restoreSimulationTime = true;

			if (pPlayer->GetAbsOrigin() == change->m_vecOrigin)
				pPlayer->SetAbsOrigin(restore->m_vecOrigin);
		}

		if (restoreSimulationTime)
			pPlayer->m_flSimulationTime() = restore->m_flSimulationTime;
	}
}

void CFeatures_Backtrack::DrawLagRecordsBasic()
{
	for (int i = 1; i < (g_Globals.m_nMaxClients + 1); i++)
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

		Vector prevOrg = pPlayer->GetAbsOrigin();

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
	//This should never happen
	if (!pPlayer || !record)
		return;

	const int pl_index = pPlayer->entindex() - 1;

	// See if this represents a change for the player
	int flags = 0;
	LagRecord* restore = &m_RestoreData[pl_index];
	LagRecord* change = &m_ChangeData[pl_index];

	// remember simulation time in case we need to restore it
	restore->m_flSimulationTime = pPlayer->m_flSimulationTime();

	// Its inefficient to compare all the bones in the bone matrix, so instead just set the data
	{
		//const size_t nBoneDataSize = sizeof(matrix3x4_t) * pPlayer->m_CachedBoneData().Count();
		//memcpy(restore->m_CachedBoneData, pPlayer->m_CachedBoneData().Base(), nBoneDataSize);
		//memcpy(pPlayer->m_CachedBoneData().Base(), record->m_CachedBoneData, nBoneDataSize);
	}

	//if (ang != pPlayer->GetAbsAngles())
	{
		flags |= LC_ANGLES_CHANGED;
		restore->m_vecAngles = pPlayer->GetAbsAngles();
		// Must be SetAbsAngles and not SetLocalAngles like in the server code
		pPlayer->SetAbsAngles(record->m_vecAngles);
		change->m_vecAngles = record->m_vecAngles;
	}

	// FIXME!!! Calling SetCollisionBounds crashes the game.
	/*if (minsPreScaled != pPlayer->m_vecMinsPreScaled() || maxsPreScaled != pPlayer->m_vecMaxsPreScaled())
	{
		flags |= LC_SIZE_CHANGED;

		restore->m_vecMinsPreScaled = pPlayer->m_vecMinsPreScaled();
		restore->m_vecMaxsPreScaled = pPlayer->m_vecMaxsPreScaled();

		pPlayer->SetCollisionBounds(minsPreScaled, maxsPreScaled);

		change->m_vecMinsPreScaled = minsPreScaled;
		change->m_vecMaxsPreScaled = maxsPreScaled;
	}*/

	//if (org != pPlayer->m_vecOrigin())
	{
		flags |= LC_ORIGIN_CHANGED;
		restore->m_vecOrigin = pPlayer->GetAbsOrigin();
		// Must be SetAbsOrigin and not SetLocalOrigin like in the server code
		pPlayer->SetAbsOrigin(record->m_vecOrigin);
		change->m_vecOrigin = record->m_vecOrigin;
	}

	if (!flags)
		return; // we didnt change anything

	m_RestorePlayer.Set(pl_index); // remember that we changed this player
	m_bNeedToRestore = true; // we have changed at least one player
	restore->m_fFlags = flags; // we need to restore these flags
	change->m_fFlags = flags; // we have changed these flags
}
