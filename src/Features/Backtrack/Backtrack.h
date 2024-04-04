#pragma once
#include "../../SDK/SDK.h"
#include "../../SDK/hl2_src/public/bitvec.h"

#define LC_NONE				0
#define LC_ALIVE			(1<<0)

#define LC_ORIGIN_CHANGED	(1<<8)
#define LC_ANGLES_CHANGED	(1<<9)
#define LC_SIZE_CHANGED		(1<<10)
#define LC_ANIMATION_CHANGED (1<<11)

#define LAG_COMPENSATION_EPS_SQR ( 0.1f * 0.1f )

#define MAX_LAYER_RECORDS (15) // CBaseAnimatingOverlay::MAX_OVERLAYS

struct LayerRecord
{
	int m_sequence;
	float m_cycle;
	float m_weight;
	int m_order;

	LayerRecord()
	{
		m_sequence = 0;
		m_cycle = 0;
		m_weight = 0;
		m_order = 0;
	}

	LayerRecord(const LayerRecord& src)
	{
		m_sequence = src.m_sequence;
		m_cycle = src.m_cycle;
		m_weight = src.m_weight;
		m_order = src.m_order;
	}
};

struct LagRecord
{
public:
	LagRecord()
	{
		m_fFlags = 0;
		m_vecOrigin.Init();
		m_vecAngles.Init();
		m_vecMinsPreScaled.Init();
		m_vecMaxsPreScaled.Init();
		m_flSimulationTime = -1;
		m_masterSequence = 0;
		m_masterCycle = 0;
	}

	LagRecord(const LagRecord& src)
	{
		m_fFlags = src.m_fFlags;
		m_vecOrigin = src.m_vecOrigin;
		m_vecAngles = src.m_vecAngles;
		m_vecMinsPreScaled = src.m_vecMinsPreScaled;
		m_vecMaxsPreScaled = src.m_vecMaxsPreScaled;
		m_flSimulationTime = src.m_flSimulationTime;
		for (int layerIndex = 0; layerIndex < MAX_LAYER_RECORDS; ++layerIndex)
		{
			m_layerRecords[layerIndex] = src.m_layerRecords[layerIndex];
		}
		m_masterSequence = src.m_masterSequence;
		m_masterCycle = src.m_masterCycle;
	}

	// Did player die this frame
	int						m_fFlags;

	// Player position, orientation and bbox
	Vector					m_vecOrigin;
	QAngle					m_vecAngles;
	Vector					m_vecMinsPreScaled;
	Vector					m_vecMaxsPreScaled;

	float					m_flSimulationTime;

	// Player animation details, so we can get the legs in the right spot.
	LayerRecord				m_layerRecords[MAX_LAYER_RECORDS];
	int						m_masterSequence;
	float					m_masterCycle;
};

class CFeatures_Backtrack
{
public:
	void FrameUpdatePostEntityThink();
	void DrawLagRecordsBasic();

	void BacktrackPlayer(C_BasePlayer* player, float flTargetTime);

private:

	void ClearHistory()
	{
		for (int i = 0; i < 100; i++) // MAX_PLAYERS
			m_PlayerTrack[i].Purge();
	}

	// keep a list of lag records for each player
	CUtlFixedLinkedList< LagRecord >	m_PlayerTrack[100]; // MAX_PLAYERS

	CBitVec<100>			m_RestorePlayer; // MAX_PLAYERS
	bool					m_bNeedToRestore;

	LagRecord				m_RestoreData[100];	// MAX_PLAYERS; player data before we moved him back
	LagRecord				m_ChangeData[100];	// MAX_PLAYERS; player data where we moved him back

	// This is used by server to tell what player to use for tickcount. For client backtrack we are only going to use local player.
	//C_BasePlayer				*m_pCurrentPlayer;	// The player we are doing lag compensation for

	float					m_flTeleportDistanceSqr;
};

namespace F { inline CFeatures_Backtrack Backtrack; }