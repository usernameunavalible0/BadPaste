#include "MovementSimulation.h"

bool CAimbot_MovementSimulation::Initialize(C_TFPlayer* pPlayer)
{
	if (!I::TFGameMovement)
		return false;

	C_TFPlayer* pLocal = UTIL_TFPlayerByIndex(g_Globals.m_nLocalIndex);

	if (!pLocal || !pPlayer || !pPlayer->IsAlive())
		return false;

	m_pPlayer = pPlayer;
	m_pPlayer->m_pCurrentCommand() = &m_NullCmd;

	g_PlayerDataBackup.Store(pPlayer);

	m_bOldInPrediction = I::ClientPrediction->m_bInPrediction;
	m_bOldFirstTimePredicted = I::ClientPrediction->m_bFirstTimePredicted;
	m_flOldFrametime = I::GlobalVars->frametime;

	//the hacks that make it work
	{
		if (pPlayer->m_fFlags().IsFlagSet(FL_DUCKING))
		{
			pPlayer->m_fFlags().ClearFlag(FL_DUCKING);
			pPlayer->m_bDucked() = true;
			pPlayer->m_flDucktime() = 0.0f;
			pPlayer->m_flDuckJumpTime() = 0.0f;
			pPlayer->m_bDucking() = false;
			pPlayer->m_bInDuckJump() = false;
		}

		if (pPlayer != pLocal)
			pPlayer->m_hGroundEntity() = nullptr;

		pPlayer->m_flModelScale() -= 0.03125f;

		if (pPlayer->m_fFlags().IsFlagSet(FL_ONGROUND))
			pPlayer->m_vecOrigin().z += 0.03125f;

		if (::fabsf(pPlayer->m_vecVelocity().x) < 0.01f)
			pPlayer->m_vecVelocity().x = 0.015f;

		if (::fabsf(pPlayer->m_vecVelocity().y) < 0.01f)
			pPlayer->m_vecVelocity().y = 0.015f;

		if (pPlayer->m_fFlags().IsFlagSet(FL_ONGROUND) || pPlayer->m_hGroundEntity().Get())
			pPlayer->m_vecVelocity().z = 0.0f;
	}

	SetupMoveData(m_pPlayer, &m_MoveData);

	return true;
}

void CAimbot_MovementSimulation::Restore()
{
	if (!m_pPlayer)
		return;

	m_pPlayer->m_pCurrentCommand() = nullptr;

	g_PlayerDataBackup.Restore(m_pPlayer);

	I::ClientPrediction->m_bInPrediction = m_bOldInPrediction;
	I::ClientPrediction->m_bFirstTimePredicted = m_bOldFirstTimePredicted;
	I::GlobalVars->frametime = m_flOldFrametime;

	m_pPlayer = nullptr;

	memset(&m_MoveData, 0, sizeof(CMoveData));
	memset(&g_PlayerDataBackup, 0, sizeof(CPlayerDataBackup));
}

void CAimbot_MovementSimulation::RunTick()
{
	if (!m_pPlayer || !I::TFGameMovement)
		return;

	I::ClientPrediction->m_bInPrediction = true;
	I::ClientPrediction->m_bFirstTimePredicted = false;
	I::GlobalVars->frametime = I::ClientPrediction->m_bEnginePaused ? 0.0f : TICK_INTERVAL;

	if (m_MoveData.m_vecVelocity.Length() < 15.0f && (m_pPlayer->m_fFlags().IsFlagSet(FL_ONGROUND)))
		return;

	m_bIsRunning = true;

	//CTFGameMovement::ProcessMovement
	reinterpret_cast<void(__thiscall*)(void*, C_BasePlayer*, CMoveData*)>(U::Offsets.m_dwProcessMovement)(I::TFGameMovement, m_pPlayer, &m_MoveData);

	m_bIsRunning = false;
}

void CAimbot_MovementSimulation::SetupMoveData(C_TFPlayer* pPlayer, CMoveData* pMoveData)
{
	if (!pPlayer || !pMoveData)
		return;

	const Vector vecVelocity = pPlayer->m_vecVelocity();

	pMoveData->m_bFirstRunOfFunctions = false;
	pMoveData->m_bGameCodeMovedPlayer = false;
	pMoveData->m_nPlayerHandle = pPlayer->GetRefEHandle();
	pMoveData->m_vecVelocity = vecVelocity;
	pMoveData->SetAbsOrigin(pPlayer->m_vecOrigin());
	pMoveData->m_flMaxSpeed = pPlayer->TeamFortress_CalculateMaxSpeed();

	if (g_PlayerDataBackup.m_fFlags.IsFlagSet(FL_DUCKING))
		pMoveData->m_flMaxSpeed *= 0.3333f;

	pMoveData->m_flClientMaxSpeed = pMoveData->m_flMaxSpeed;
	pMoveData->m_vecViewAngles = { 0.0f, G::Util.VelocityToAngles(pMoveData->m_vecVelocity).y, 0.0f };

	//Assume full accel
	pMoveData->m_flForwardMove = 450.0f;
	pMoveData->m_flSideMove = 0.0f;

	/*
	pMoveData->m_flForwardMove = (pMoveData->m_vecVelocity.y - vRight.y / vRight.x * pMoveData->m_vecVelocity.x) / (vForward.y - vRight.y / vRight.x * vForward.x);
	pMoveData->m_flSideMove = (pMoveData->m_vecVelocity.x - vForward.x * pMoveData->m_flForwardMove) / vRight.x;
	*/

	if (pPlayer->m_vecVelocity().Length2D() <= (pMoveData->m_flMaxSpeed * 0.1f))
		pMoveData->m_flForwardMove = pMoveData->m_flSideMove = 0.0f;

	pMoveData->m_vecAngles = pMoveData->m_vecViewAngles;
	pMoveData->m_vecOldAngles = pMoveData->m_vecAngles;

	C_BaseEntity* pConstraintEntity = pPlayer->m_hConstraintEntity().Get();

	if (pConstraintEntity)
		pMoveData->m_vecConstraintCenter = pConstraintEntity->GetAbsOrigin();
	else
		pMoveData->m_vecConstraintCenter = pPlayer->m_vecConstraintCenter();

	pMoveData->m_flConstraintRadius = pPlayer->m_flConstraintRadius();
	pMoveData->m_flConstraintWidth = pPlayer->m_flConstraintWidth();
	pMoveData->m_flConstraintSpeedFactor = pPlayer->m_flConstraintSpeedFactor();
}