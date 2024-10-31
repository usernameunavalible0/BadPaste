#pragma once
#include "../../SDK/SDK.h"
#include "../../SDK/hl2_src/public/bitvec.h"
#include <functional>

#define LC_NONE				0
#define LC_ALIVE			(1<<0)
#define LC_ORIGIN_CHANGED	(1<<8)
#define LC_ANGLES_CHANGED	(1<<9)
#define LC_SIZE_CHANGED		(1<<10)

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
	}

	LagRecord(const LagRecord& src)
	{
		m_fFlags = src.m_fFlags;
		m_vecOrigin = src.m_vecOrigin;
		m_vecAngles = src.m_vecAngles;
		m_vecMinsPreScaled = src.m_vecMinsPreScaled;
		m_vecMaxsPreScaled = src.m_vecMaxsPreScaled;
		m_flSimulationTime = src.m_flSimulationTime;
		for (int boneIndex = 0; boneIndex < MAXSTUDIOBONES; ++boneIndex)
		{
			m_CachedBoneData[boneIndex] = src.m_CachedBoneData[boneIndex];
		}
	}

	// Did player die this frame
	int						m_fFlags;

	// Player position, orientation and bbox
	Vector					m_vecOrigin;
	QAngle					m_vecAngles;
	Vector					m_vecMinsPreScaled;
	Vector					m_vecMaxsPreScaled;

	// Player bone cache
	matrix3x4_t				m_CachedBoneData[MAXSTUDIOBONES];

	float					m_flSimulationTime;
};

class CFeatures_Backtrack
{
public:
	void PostDataUpdate();

	void StartBacktracking(C_BasePlayer* pPlayer, CUserCmd* cmd, std::function<bool()>);
	void FinishBacktracking();

	void DrawLagRecordsBasic();

	void ClearHistory()
	{
		for (int i = 0; i < MAX_PLAYERS; i++)
			m_PlayerTrack[i].Purge();
	}

private:
	void BacktrackPlayer(C_BasePlayer* pPlayer, LagRecord* record);

	// keep a list of lag records for each player
	CUtlFixedLinkedList<LagRecord> m_PlayerTrack[MAX_PLAYERS];

	CBitVec<MAX_PLAYERS> m_RestorePlayer;
	bool m_bNeedToRestore;

	LagRecord m_RestoreData[MAX_PLAYERS]; // player data before we moved him back
	LagRecord m_ChangeData[MAX_PLAYERS]; // player data where we moved him back

	float m_flTeleportDistanceSqr;
};

namespace F { inline CFeatures_Backtrack Backtrack; }