#pragma once
#include "../../SDK/SDK.h"

struct LagRecord
{
public:
	LagRecord()
	{
		m_vecOrigin.Init();
		m_vecAngles.Init();
		m_flSimulationTime = -1;
	}

	// Player position, orientation
	Vector m_vecOrigin;
	QAngle m_vecAngles;

	float m_flSimulationTime;
};

class CFeatures_Backtrack
{
public:
	void OnNetUpdate();
	void DrawSimple();

private:
	CUtlFixedLinkedList<LagRecord> m_PlayerTrack[MAX_PLAYERS];

public:
	void ClearHistory()
	{
		for (int i = 0; i < MAX_PLAYERS; i++)
			m_PlayerTrack[i].Purge();
	}

	inline CUtlFixedLinkedList<LagRecord>* GetTrackForPlayer(C_TFPlayer* pPlayer) { return &m_PlayerTrack[pPlayer->entindex() - 1]; }
};

namespace F { inline CFeatures_Backtrack Backtrack; }