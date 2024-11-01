#pragma once

#include "../Pattern/Pattern.h"

class CUtil_Offsets
{
public:
	bool Initialize();

public:
	DWORD m_dwGlobalVars = 0x0;
	DWORD m_dwPredictionRandomSeed = 0x0;
	DWORD m_dwStartDrawing = 0x0;
	DWORD m_dwFinishDrawing = 0x0;
	DWORD m_dwEntitySphereQuery = 0x0;
	DWORD m_dwClientModeShared = 0x0;
	DWORD m_dwIsPlayerOnSteamFriendsList = 0x0;
	DWORD m_dwPhysicsRunThink = 0x0;
	DWORD m_dwSaveData = 0x0;
	DWORD m_dwOnAddEntity = 0x0;
	DWORD m_dwOnRemoveEntity = 0x0;
	DWORD m_dwCheckForSequenceChange = 0x0;
	DWORD m_dwLoadFromBuffer = 0x0;
	DWORD m_dwInitializeKeyValue = 0x0;
	DWORD m_dwFindKey = 0x0;
	DWORD m_dwPerformScreenSpaceEffects = 0x0;
	DWORD m_dwCLMove = 0x0;
	DWORD m_dwCLSendMove = 0x0;
	DWORD m_dwCLCheckForPureServerWhitelist = 0x0;
	DWORD m_dwClientState = 0x0;
	DWORD m_dwGetTracerType = 0x0;
	DWORD m_dwFireBullet = 0x0;
	DWORD m_dwBaseInterpolatePart1 = 0x0;
	DWORD m_dwSendDataGram = 0x0;
	DWORD m_dwShutdown = 0x0;
	DWORD m_dwSendNetMsg = 0x0;
	DWORD m_dwCalcPlayerView = 0x0;
	DWORD m_dwBaseAnimatingSetupBones = 0x0;
	DWORD m_dwPredictionPlayer = 0x0;
	DWORD m_dwUsingStandardWeaponsInVehicle = 0x0;
	DWORD m_dwUpdateButtonState = 0x0;
	DWORD m_dwIsPlayerDominated = 0x0;
	DWORD m_dwSetInt = 0x0;
	DWORD m_dwInCond = 0x0;
	DWORD m_dwCalcViewModelView = 0x0;
	DWORD m_dwAttribHookValue = 0x0;
	DWORD m_dwGetNumAnimOverlays = 0x0;
	DWORD m_dwGetAnimOverlay = 0x0;
	DWORD m_dwSetCollisionBounds = 0x0;
	DWORD m_dwInvalidatePhysicsRecursive = 0x0;
	DWORD m_dwInvalidateBoneCache = 0x0;
	DWORD m_dwSetAbsOrigin = 0x0;
	DWORD m_dwSetAbsAngles = 0x0;
	DWORD m_dwHost_ShouldRun = 0x0;
	DWORD m_dwnet_time = 0x0;
	DWORD m_dwIsPaused = 0x0;
	DWORD m_dwAvoidPlayers = 0x0;
	DWORD m_dwEstimateAbsVelocity = 0x0;
	DWORD m_dwProcessFixAngle = 0x0;
	DWORD m_dwProcessMovement = 0x0;
	DWORD m_dwCalculateMaxSpeed = 0x0;
	DWORD m_dwTFPlayerMove = 0x0;
	DWORD m_dwPlayerMove = 0x0;
	DWORD m_dwFXFireBullets = 0x0;
	DWORD m_dwhost_frametime_unbounded = 0x0;
	DWORD m_dwhost_frametime_stddeviation = 0x0;
	DWORD m_dwDrawViewModels = 0x0;
};

namespace U { inline CUtil_Offsets Offsets; }