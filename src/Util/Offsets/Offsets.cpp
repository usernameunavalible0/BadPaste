#include "Offsets.h"

#include "../../SDK/hl2_src/public/tier0/platform.h"

#define _CHECKPATT(x) if (x) { Error(#x); return false; }
#define GETPATT(x, mod, patt) x = g_Pattern.Find(mod, patt); _CHECKPATT(x == 0x0)

bool CUtil_Offsets::Initialize()
{
	GETPATT(m_dwEntitySphereQuery, "client.dll", "55 8B EC 83 EC 14 D9 45 0C");
	GETPATT(m_dwIsPlayerOnSteamFriendsList, "client.dll", "55 8B EC 81 EC ? ? ? ? E8 ? ? ? ? 85 C0 74 7E 8B 4D 08 85 C9 74 77 A1 ? ? ? ? 83 78 08 00 74 6C 83 78 0C 00 74 66 A1 ? ? ? ? 83 C1 08 56 8B 30 8D 85 ? ? ? ? 50 8B 01");
	GETPATT(m_dwPhysicsRunThink, "client.dll", "55 8B EC 53 8B D9 56 57 8B 83 ? ? ? ? C1 E8 16 A8 01 B0 01 0F 85 ? ? ? ?");
	GETPATT(m_dwSaveData, "client.dll", "55 8B EC 81 EC ? ? ? ? 53 8B D9 57 8B 7D");
	GETPATT(m_dwOnAddEntity, "client.dll", "55 8B EC 8B 45 0C 83 EC 0C 53");
	GETPATT(m_dwOnRemoveEntity, "client.dll", "55 8B EC 51 8B 45 0C 53 8B D9 56 57");
	GETPATT(m_dwCheckForSequenceChange, "client.dll", "55 8B EC 53 8B 5D 08 57 8B F9 85 DB 0F 84 ? ? ? ? 83");
	GETPATT(m_dwGetTracerType, "client.dll", "55 8B EC 51 53 56 57 8B F1 E8 ? ? ? ? 6A");
	GETPATT(m_dwFireBullet, "client.dll", "53 8B DC 83 EC 08 83 E4 F0 83 C4 04 55 8B 6B 04 89 6C 24 04 8B EC 81 EC C8 01 00 00 56 57 8B F9 8B");
	GETPATT(m_dwBaseInterpolatePart1, "client.dll", "55 8B EC 8B 45 18 56 8B F1");
	GETPATT(m_dwCalcPlayerView, "client.dll", "55 8B EC 83 EC 18 53 8B 5D 0C 56 8B");
	GETPATT(m_dwBaseAnimatingSetupBones, "client.dll", "55 8B EC 81 EC 8C 0E 00 00 53");
	GETPATT(m_dwFindKey, "client.dll", "55 8B EC 81 EC ? ? ? ? 56 8B 75 08 57 8B F9 85 F6 0F");
	GETPATT(m_dwUsingStandardWeaponsInVehicle, "client.dll", "56 57 8B F9 8B 97 ? ? ? ? 85 D2 74 52");
	GETPATT(m_dwUpdateButtonState, "client.dll", "55 8B EC 8B 55 08 56 8B B1");
	GETPATT(m_dwIsPlayerDominated, "client.dll", "55 8B EC 56 57 8B F1 E8 ? ? ? ? 8B F8 85 FF 75 08");
	GETPATT(m_dwSetInt, "client.dll", "55 8B EC 6A 01 FF 75 08 E8 ? ? ? ? 85 C0 74 0A 8B 4D 0C 89 48 0C C6 40 10 02");
	GETPATT(m_dwInCond, "client.dll", "55 8B EC 83 EC 08 56 57 8B 7D 08 8B F1 83 FF 20");
	GETPATT(m_dwCalcViewModelView, "client.dll", "55 8B EC 81 EC ? ? ? ? 8B 55 0C");
	GETPATT(m_dwGetAnimOverlay, "client.dll", "55 8B EC 6B 45 08 2C");
	GETPATT(m_dwGetNumAnimOverlays, "client.dll", "8B 81 B4 08 ? ? C3");
	GETPATT(m_dwSetCollisionBounds, "client.dll", "55 8B EC 83 EC 34 53 8B 5D 0C 56 57");
	GETPATT(m_dwInvalidatePhysicsRecursive, "client.dll", "55 8B EC 83 EC 08 53 8B 5D 08 56 57 8B F1 8B");
	GETPATT(m_dwInvalidateBoneCache, "client.dll", "A1 ? ? ? ? 48 C7 81");
	GETPATT(m_dwSetAbsOrigin, "client.dll", "55 8B EC 56 57 8B 7D 08 8B F1 E8");
	GETPATT(m_dwSetAbsAngles, "client.dll", "55 8B EC 83 EC 60 56 57 8B 7D 08 8B");
	GETPATT(m_dwAvoidPlayers, "client.dll", "55 8B EC 81 EC ? ? ? ? A1 ? ? ? ? 57 8B F9 89 7D CC");
	GETPATT(m_dwEstimateAbsVelocity, "client.dll", "55 8B EC 83 EC 0C 56 8B F1 E8 ? ? ? ? 3B");
	GETPATT(m_dwProcessMovement, "client.dll", "55 8B EC 56 57 8B 7D 08 8B F1 85 FF 0F 84 ? ? ? ? 83");
	GETPATT(m_dwCalculateMaxSpeed, "client.dll", "55 8B EC 83 EC 18 83 3D ? ? ? ? ? 56");
	GETPATT(m_dwTFPlayerMove, "client.dll", "55 8B EC 83 EC 6C 53 56 8B F1 57");
	GETPATT(m_dwPlayerMove, "client.dll", "55 8B EC 51 56 57 8B F9 8B 07 FF 90 ? ? ? ? 8B 47");
	GETPATT(m_dwFXFireBullets, "client.dll", "53 8B DC 83 EC 08 83 E4 F0 83 C4 04 55 8B 6B 04 89 6C 24 04 8B EC 81 EC C8 00 00 00 56 57 8B 7B");
	GETPATT(m_dwDrawViewModels, "client.dll", "55 8B EC 81 EC ? ? ? ? 8B 15 ? ? ? ? 53 8B D9 33 C9 89 4D ? 89 4D ? 8B 02 89 5D ? 89 4D ? 89 4D ? 56 57 85 C0 74 ? 68 ? ? ? ? 68 ? ? ? ? 68 ? ? ? ? 68 ? ? ? ? 68 ? ? ? ? 51 51 51 51 8D 4D ? 51 50 8B 40 ? FF D0 8B 45 ? 83 C4 ? 8B 15 ? ? ? ? 89 45 ? 8B 45 ? 89 45 ? 80 7D");

	GETPATT(m_dwLoadFromBuffer, "engine.dll", "55 8B EC 83 EC 38 53 8B 5D 0C");
	GETPATT(m_dwCLMove, "engine.dll", "55 8B EC B8 2C 10 00 00 E8 ? ? ? ? 83");
	GETPATT(m_dwCLCheckForPureServerWhitelist, "engine.dll", "55 8B EC 83 EC 34 83");
	GETPATT(m_dwSendDataGram, "engine.dll", "55 8B EC B8 ? ? ? ? E8 ? ? ? ? A1 ? ? ? ? 53 56 8B");
	GETPATT(m_dwShutdown, "engine.dll", "55 8B EC 83 EC 10 56 8B F1 83 BE ? ? ? ? ? 0F 8C ? ? ? ?");
	GETPATT(m_dwSendNetMsg, "engine.dll", "55 8B EC 57 8B F9 8D 8F 94 00 00 00 E8 ? ? ? ? 85 C0 75 07 B0 01 5F 5D C2 0C");
	GETPATT(m_dwHost_ShouldRun, "engine.dll", "A1 ? ? ? ? 83 78 30 00 74 30 A1 ? ? ? ? 83 78 30 00 A1");
	GETPATT(m_dwIsPaused, "engine.dll", "80 B9 ? ? ? ? ? 75 42");
	GETPATT(m_dwProcessFixAngle, "engine.dll", "55 8B EC F3 0F 10 1D ? ? ? ? 83");

	GETPATT(m_dwStartDrawing, "vguimatsurface.dll", "55 8B EC 64 A1 ? ? ? ? 6A FF 68 ? ? ? ? 50 64 89 25 ? ? ? ? 83 EC 14");
	GETPATT(m_dwFinishDrawing, "vguimatsurface.dll", "55 8B EC 6A FF 68 ? ? ? ? 64 A1 ? ? ? ? 50 64 89 25 ? ? ? ? 51 56 6A 00");

	if (const DWORD dwClientMode = g_Pattern.Find("client.dll", "8B 0D ? ? ? ? 8B 02 D9 05"))
		m_dwClientModeShared = (dwClientMode + 0x02);

	if (const DWORD dwScreenSpaceFx = g_Pattern.Find("client.dll", "E8 ? ? ? ? 8B 0D ? ? ? ? 8B 01 FF 90 ? ? ? ? 83 F8 01"))
		m_dwPerformScreenSpaceEffects = (dwScreenSpaceFx + 0x01);

	if (const DWORD dwGlobalVars = g_Pattern.Find("engine.dll", "68 ? ? ? ? 50 50 FF 12"))
		m_dwGlobalVars = (dwGlobalVars + 0x01);

	if (const DWORD dwClientState = g_Pattern.Find("engine.dll", "68 ? ? ? ? E8 ? ? ? ? 83 C4 08 5F 5E 5B 5D C3"))
		m_dwClientState = (dwClientState + 0x01);

	if (const DWORD dwInitKVal = g_Pattern.Find("engine.dll", "FF 15 ? ? ? ? 83 C4 08 89 06 8B C6"))
		m_dwInitializeKeyValue = (dwInitKVal - 0x42);

	if (const DWORD dwRandomSeed = g_Pattern.Find("client.dll", "C7 05 ? ? ? ? ? ? ? ? 5D C3 8B 40 34"))
		m_dwPredictionRandomSeed = (dwRandomSeed + 0x02);

	if (const DWORD dwPredictionPlayer = g_Pattern.Find("client.dll", "89 3D ? ? ? ? F3"))
		m_dwPredictionPlayer = (dwPredictionPlayer + 0x02);

	if (const DWORD dwAttribHookValue = g_Pattern.Find("client.dll", "E8 ? ? ? ? 83 C4 14 D9 E8"))
		m_dwAttribHookValue = (dwAttribHookValue + 0x1);

	if (const DWORD dwnet_time = g_Pattern.Find("engine.dll", "DD 1D ? ? ? ? 5D"))
		m_dwnet_time = (dwnet_time + 0x02);

	if (const DWORD dwhost_frametime_unbounded = g_Pattern.Find("engine.dll", "D9 05 ? ? ? ? DE C2"))
		m_dwhost_frametime_unbounded = (dwhost_frametime_unbounded + 0x02);

	if (const DWORD dwhost_frametime_stddeviation = g_Pattern.Find("engine.dll", "D9 1D ? ? ? ? 0F 85"))
		m_dwhost_frametime_stddeviation = (dwhost_frametime_stddeviation + 0x02);

	_CHECKPATT(m_dwGlobalVars == 0x0);
	_CHECKPATT(m_dwPredictionRandomSeed == 0x0);
	_CHECKPATT(m_dwInitializeKeyValue == 0x0);
	_CHECKPATT(m_dwClientModeShared == 0x0);
	_CHECKPATT(m_dwClientState == 0x0);
	_CHECKPATT(m_dwPredictionPlayer == 0x0);

	//If we arrive here, return success.
	return true;
}