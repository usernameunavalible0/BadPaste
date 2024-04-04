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

float g_flFractionScale = 0.95;
void CFeatures_Backtrack::BacktrackPlayer(C_BasePlayer* pPlayer, float flTargetTime)
{
	Vector org;
	Vector minsPreScaled;
	Vector maxsPreScaled;
	QAngle ang;

	int pl_index = pPlayer->entindex() - 1;

	// get track history of this player
	CUtlFixedLinkedList< LagRecord >* track = &m_PlayerTrack[pl_index];

	// check if we have at leat one entry
	if (track->Count() <= 0)
		return;

	int curr = track->Head();

	LagRecord* prevRecord = NULL;
	LagRecord* record = NULL;

	Vector prevOrg = pPlayer->m_vecOrigin();

	// Walk context looking for any invalidating event
	while (track->IsValidIndex(curr))
	{
		// remember last record
		prevRecord = record;

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

		// did we find a context smaller than target time ?
		if (record->m_flSimulationTime <= flTargetTime)
			break; // hurra, stop

		prevOrg = record->m_vecOrigin;

		// go one step back
		curr = track->Next(curr);
	}

	if (!record)
		return; // that should never happen

	float frac = 0.0f;
	if (prevRecord &&
		(record->m_flSimulationTime < flTargetTime) &&
		(record->m_flSimulationTime < prevRecord->m_flSimulationTime))
	{
		// we didn't find the exact time but have a valid previous record
		// so interpolate between these two records

		// calc fraction between both records
		frac = (flTargetTime - record->m_flSimulationTime) /
			(prevRecord->m_flSimulationTime - record->m_flSimulationTime);

		ang = Lerp(frac, record->m_vecAngles, prevRecord->m_vecAngles);
		org = Lerp(frac, record->m_vecOrigin, prevRecord->m_vecOrigin);
		minsPreScaled = Lerp(frac, record->m_vecMinsPreScaled, prevRecord->m_vecMinsPreScaled);
		maxsPreScaled = Lerp(frac, record->m_vecMaxsPreScaled, prevRecord->m_vecMaxsPreScaled);
	}
	else
	{
		// we found the exact record or no other record to interpolate with
		// just copy these values since they are the best we have
		org = record->m_vecOrigin;
		ang = record->m_vecAngles;
		minsPreScaled = record->m_vecMinsPreScaled;
		maxsPreScaled = record->m_vecMaxsPreScaled;
	}

	// See if this is still a valid position for us to teleport to
	if (G::ConVars.sv_unlag_fixstuck->GetBool())
	{
		// Try to move to the wanted position from our current position.
		trace_t tr;
		UTIL_TraceEntity(pPlayer, org, org, MASK_PLAYERSOLID, &tr);
		if (tr.startsolid || tr.allsolid)
		{
			C_BasePlayer* pHitPlayer = dynamic_cast<C_BasePlayer*>(tr.m_pEnt);

			// don't lag compensate the local player
			if (pHitPlayer && (pHitPlayer->entindex() != g_Globals.m_nLocalIndex))
			{
				// If we haven't backtracked this player, do it now
				// this deliberately ignores WantsLagCompensationOnEntity.
				if (!m_RestorePlayer.Get(pHitPlayer->entindex() - 1))
				{
					// prevent recursion - save a copy of m_RestorePlayer,
					// pretend that this player is off-limits
					int pl_index = pPlayer->entindex() - 1;

					// Temp turn this flag on
					m_RestorePlayer.Set(pl_index);

					BacktrackPlayer(pHitPlayer, flTargetTime);

					// Remove the temp flag
					m_RestorePlayer.Clear(pl_index);
				}
			}

			// now trace us back as far as we can go
			UTIL_TraceEntity(pPlayer, pPlayer->m_vecOrigin(), org, MASK_PLAYERSOLID, &tr);

			if (tr.startsolid || tr.allsolid)
			{
				// Our starting position is bogus
			}
			else
			{
				// We can get to a valid place, but not all the way to the target
				Vector vPos;
				VectorLerp(pPlayer->m_vecOrigin(), org, tr.fraction * g_flFractionScale, vPos);

				// This is as close as we're going to get
				org = vPos;
			}
		}
	}

	// See if this represents a change for the player
	int flags = 0;
	LagRecord* restore = &m_RestoreData[pl_index];
	LagRecord* change = &m_ChangeData[pl_index];

	QAngle angdiff = pPlayer->m_angRotation() - ang;
	Vector orgdiff = pPlayer->m_vecOrigin() - org;

	// Always remember the pristine simulation time in case we need to restore it.
	restore->m_flSimulationTime = pPlayer->m_flSimulationTime();

	if (angdiff.LengthSqr() > LAG_COMPENSATION_EPS_SQR)
	{
		flags |= LC_ANGLES_CHANGED;
		restore->m_vecAngles = pPlayer->m_angRotation();
		pPlayer->SetLocalAngles(ang);
		change->m_vecAngles = ang;
	}

	// Use absolute equality here
	if (minsPreScaled != pPlayer->m_vecMinsPreScaled() || maxsPreScaled != pPlayer->m_vecMaxsPreScaled())
	{
		flags |= LC_SIZE_CHANGED;

		restore->m_vecMinsPreScaled = pPlayer->m_vecMinsPreScaled();
		restore->m_vecMaxsPreScaled = pPlayer->m_vecMaxsPreScaled();

		//pPlayer->SetSize(minsPreScaled, maxsPreScaled);
		pPlayer->SetCollisionBounds(minsPreScaled, maxsPreScaled);

		change->m_vecMinsPreScaled = minsPreScaled;
		change->m_vecMaxsPreScaled = maxsPreScaled;
	}

	// Note, do origin at end since it causes a relink into the k/d tree
	if (orgdiff.LengthSqr() > LAG_COMPENSATION_EPS_SQR)
	{
		flags |= LC_ORIGIN_CHANGED;
		restore->m_vecOrigin = pPlayer->m_vecOrigin();
		pPlayer->SetLocalOrigin(org);
		change->m_vecOrigin = org;
	}

	// Sorry for the loss of the optimization for the case of people
	// standing still, but you breathe even on the server.
	// This is quicker than actually comparing all bazillion floats.
	flags |= LC_ANIMATION_CHANGED;
	restore->m_masterSequence = pPlayer->m_nSequence();
	restore->m_masterCycle = pPlayer->m_flCycle();

	bool interpolationAllowed = false;
	if (prevRecord && (record->m_masterSequence == prevRecord->m_masterSequence))
	{
		// If the master state changes, all layers will be invalid too, so don't interp (ya know, interp barely ever happens anyway)
		interpolationAllowed = true;
	}

	////////////////////////
	// First do the master settings
	bool interpolatedMasters = false;
	if (frac > 0.0f && interpolationAllowed)
	{
		interpolatedMasters = true;
		pPlayer->SetSequence(Lerp(frac, record->m_masterSequence, prevRecord->m_masterSequence));
		pPlayer->SetCycle(Lerp(frac, record->m_masterCycle, prevRecord->m_masterCycle));

		if (record->m_masterCycle > prevRecord->m_masterCycle)
		{
			// the older record is higher in frame than the newer, it must have wrapped around from 1 back to 0
			// add one to the newer so it is lerping from .9 to 1.1 instead of .9 to .1, for example.
			float newCycle = Lerp(frac, record->m_masterCycle, prevRecord->m_masterCycle + 1);
			pPlayer->SetCycle(newCycle < 1 ? newCycle : newCycle - 1);// and make sure .9 to 1.2 does not end up 1.05
		}
		else
		{
			pPlayer->SetCycle(Lerp(frac, record->m_masterCycle, prevRecord->m_masterCycle));
		}
	}
	if (!interpolatedMasters)
	{
		pPlayer->SetSequence(record->m_masterSequence);
		pPlayer->SetCycle(record->m_masterCycle);
	}

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

			bool interpolated = false;
			if ((frac > 0.0f) && interpolationAllowed)
			{
				LayerRecord& recordsLayerRecord = record->m_layerRecords[layerIndex];
				LayerRecord& prevRecordsLayerRecord = prevRecord->m_layerRecords[layerIndex];
				if ((recordsLayerRecord.m_order == prevRecordsLayerRecord.m_order)
					&& (recordsLayerRecord.m_sequence == prevRecordsLayerRecord.m_sequence)
					)
				{
					// We can't interpolate across a sequence or order change
					interpolated = true;
					if (recordsLayerRecord.m_cycle > prevRecordsLayerRecord.m_cycle)
					{
						// the older record is higher in frame than the newer, it must have wrapped around from 1 back to 0
						// add one to the newer so it is lerping from .9 to 1.1 instead of .9 to .1, for example.
						float newCycle = Lerp(frac, recordsLayerRecord.m_cycle, prevRecordsLayerRecord.m_cycle + 1);
						currentLayer->m_flCycle = newCycle < 1 ? newCycle : newCycle - 1;// and make sure .9 to 1.2 does not end up 1.05
					}
					else
					{
						currentLayer->m_flCycle = Lerp(frac, recordsLayerRecord.m_cycle, prevRecordsLayerRecord.m_cycle);
					}
					currentLayer->m_nOrder = recordsLayerRecord.m_order;
					currentLayer->m_nSequence = recordsLayerRecord.m_sequence;
					currentLayer->m_flWeight = Lerp(frac, recordsLayerRecord.m_weight, prevRecordsLayerRecord.m_weight);
				}
			}
			if (!interpolated)
			{
				//Either no interp, or interp failed.  Just use record.
				currentLayer->m_flCycle = record->m_layerRecords[layerIndex].m_cycle;
				currentLayer->m_nOrder = record->m_layerRecords[layerIndex].m_order;
				currentLayer->m_nSequence = record->m_layerRecords[layerIndex].m_sequence;
				currentLayer->m_flWeight = record->m_layerRecords[layerIndex].m_weight;
			}
		}
	}

	if (!flags)
		return; // we didn't change anything

	// TODO: pPlayer->InvalidateBoneCache(); 
	// I already know how to do this but not everything we might want to do needs to call setupbones
	*reinterpret_cast<int*>(reinterpret_cast<DWORD>(pPlayer) + 0x59C) = -INT_MAX;
	*reinterpret_cast<float*>(reinterpret_cast<DWORD>(pPlayer) + 0x860) = -FLT_MAX;

	m_RestorePlayer.Set( pl_index ); //remember that we changed this player
	m_bNeedToRestore = true;  // we changed at least one player
	restore->m_fFlags = flags; // we need to restore these flags
	change->m_fFlags = flags; // we have changed these flags
}
