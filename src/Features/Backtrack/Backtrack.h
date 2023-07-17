#pragma once
#include "../../SDK/SDK.h"

#include "../../SDK/hl2_src/public/bitvec.h"


#define LAG_COMPENSATION_EPS_SQR ( 0.1f * 0.1f )
// Allow 4 units of error ( about 1 / 8 bbox width )
#define LAG_COMPENSATION_ERROR_EPS_SQR ( 4.0f * 4.0f )
#define LC_NONE				0
#define LC_ALIVE			(1<<0)

#define LC_ORIGIN_CHANGED	(1<<8)
#define LC_ANGLES_CHANGED	(1<<9)
#define LC_SIZE_CHANGED		(1<<10)
#define LC_ANIMATION_CHANGED (1<<11)
#define MAX_LAYER_RECORDS 15 //CBaseAnimatingOverlay::MAX_OVERLAYS

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

	int						m_fFlags;
	Vector					m_vecOrigin;
	QAngle					m_vecAngles;
	Vector					m_vecMinsPreScaled;
	Vector					m_vecMaxsPreScaled;
	float					m_flSimulationTime;
	LayerRecord				m_layerRecords[MAX_LAYER_RECORDS];
	int						m_masterSequence;
	float					m_masterCycle;
};

class CFeatures_Backtrack
{
public:
	void FrameUpdatePostEntityThink();

	void BacktrackPlayer(C_BasePlayer* pPlayer, float flTargetTime);
	
	void StartLagCompensation(C_BasePlayer* player, CUserCmd* cmd);
	void FinishLagCompensation(C_BasePlayer* player);

	inline void ClearHistory()
	{
		for (int i = 0; i < MAX_PLAYERS; i++)
			m_PlayerTrack[i].Purge();
	}

	// keep a list of lag records for each player
	CUtlFixedLinkedList<LagRecord> m_PlayerTrack[MAX_PLAYERS];

	LagRecord m_RestoreData[MAX_PLAYERS];
	LagRecord m_ChangeData[MAX_PLAYERS];

	CBitVec<MAX_PLAYERS> m_RestorePlayer;

	float m_flTeleportDistanceSqr;
	bool m_bNeedToRestore;
};

namespace F { inline CFeatures_Backtrack Backtrack; }