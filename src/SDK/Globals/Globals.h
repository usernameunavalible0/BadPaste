#pragma once

#define DT_WAIT_CALLS 26

class CGlobals
{
public:
	int m_nLocalIndex = 0;
	int m_nMaxClients = 0;
	int m_nMaxEntities = 0;
	int m_nScreenHeight = 0;
	int m_nScreenWidht = 0;
	int m_nShifted = 22;
	int m_nWaitForShift = 0;

	float m_flCurAimFOV = 0.0f;

	bool m_bIsInGame = false;
	bool m_bIsGameUIVisible = false;
	bool m_bIsBeingSpectated = false;
	bool m_bChoking = false;
	bool m_bRecharging = false;
	bool m_bShouldShift = false;

	QAngle m_vViewAngles;
	QAngle m_vRealViewAngles;
};

inline CGlobals g_Globals;