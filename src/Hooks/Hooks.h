#pragma once
#include "../SDK/SDK.h"

class INetworkStringTable;

CREATE_HOOK(CBaseEntity_BaseInterpolatePart1, U::Offsets.m_dwBaseInterpolatePart1, int, __fastcall, C_BaseEntity* pThis, void* edx, float& currentTime, Vector& oldOrigin, QAngle& oldAngles, Vector& oldVel, int& bNoMoreChanges);
CREATE_HOOK(CBasePlayer_CalcPlayerView, U::Offsets.m_dwCalcPlayerView, void, __fastcall, C_BasePlayer* pThis, void* edx, Vector& eyeOrigin, QAngle& eyeAngles, float& fov);
CREATE_HOOK(CL_CheckForPureServerWhitelist, U::Offsets.m_dwCLCheckForPureServerWhitelist, void, __cdecl, IFileList* &pFilesToReload);
CREATE_HOOK(CL_Move, U::Offsets.m_dwCLMove, void, __cdecl, float accumulated_extra_samples, bool bFinalTick);
CREATE_HOOK(IBaseClientDLL_LevelInitPreEntity, U::VFunc.Get<void*>(I::BaseClient, 5u), void, __fastcall, void* ecx, void* edx, char const* pMapName);
CREATE_HOOK(IBaseClientDLL_LevelInitPostEntity, U::VFunc.Get<void*>(I::BaseClient, 6u), void, __fastcall, void* ecx, void* edx);
CREATE_HOOK(IBaseClientDLL_LevelShutdown, U::VFunc.Get<void*>(I::BaseClient, 7u), void, __fastcall, void* ecx, void* edx);
CREATE_HOOK(IBaseClientDLL_FrameStageNotify, U::VFunc.Get<void*>(I::BaseClient, 35u), void, __fastcall, void* ecx, void* edx, ClientFrameStage_t curStage);
CREATE_HOOK(IBaseClientDLL_DispatchUserMessage, U::VFunc.Get<void*>(I::BaseClient, 36u), bool, __fastcall, void* ecx, void* edx, int msg_type, bf_read& msg_data);
CREATE_HOOK(IClientEntityList_OnAddEntity, U::Offsets.m_dwOnAddEntity, void, __fastcall, void* ecx, void* edx, IHandleEntity* pEnt, CBaseHandle handle);
CREATE_HOOK(IClientEntityList_OnRemoveEntity, U::Offsets.m_dwOnRemoveEntity, void, __fastcall, void* ecx, void* edx, IHandleEntity* pEnt, CBaseHandle handle);
CREATE_HOOK(ClientModeShared_OverrideView, U::VFunc.Get<void*>(I::ClientMode, 16u), void, __fastcall, void* ecx, void* edx, CViewSetup* pSetup);
CREATE_HOOK(ClientModeShared_CreateMove, U::VFunc.Get<void*>(I::ClientMode, 21u), bool, __fastcall, void* ecx, void* edx, float flInputSampleTime, CUserCmd* cmd);
CREATE_HOOK(ClientModeShared_ShouldDrawViewModel, U::VFunc.Get<void*>(I::ClientMode, 24u), bool, __fastcall, void* ecx, void* edx);
CREATE_HOOK(ClientModeShared_GetViewModelFOV, U::VFunc.Get<void*>(I::ClientMode, 32u), float, __fastcall, void* ecx, void* edx);
CREATE_HOOK(ClientModeShared_DoPostScreenSpaceEffects, U::VFunc.Get<void*>(I::ClientMode, 39u), bool, __fastcall, void* ecx, void* edx, const CViewSetup* pSetup);
CREATE_HOOK(ClientModeShared_ShouldBlackoutAroundHUD, U::VFunc.Get<void*>(I::ClientMode, 42u), bool, __fastcall, void* ecx, void* edx);
CREATE_HOOK(CPrediction_RunCommand, U::VFunc.Get<void*>(I::ClientPrediction, 17u), void, __fastcall, void* ecx, void* edx, C_BasePlayer* player, CUserCmd* ucmd, IMoveHelper* moveHelper);
CREATE_HOOK(IVEngineClient_IsPlayingTimeDemo, U::VFunc.Get<void*>(I::EngineClient, 78u), bool, __fastcall, void* ecx, void* edx);
CREATE_HOOK(IEngineVGui_Paint, U::VFunc.Get<void*>(I::EngineVGui, 14u), void, __fastcall, void* ecx, void* edx, int mode);
CREATE_HOOK(CInput_WriteUsercmdDeltaToBuffer, U::VFunc.Get<void*>(I::Input, 5u), bool, __fastcall, void* ecx, void* edx, bf_write* buf, int from, int to, bool isnewcommand);
CREATE_HOOK(CInput_EncodeUserCmdToBuffer, U::VFunc.Get<void*>(I::Input, 6u), void, __fastcall, void* ecx, void* edx, bf_write& buf, int slot);
CREATE_HOOK(CInput_DecodeUserCmdFromBuffer, U::VFunc.Get<void*>(I::Input, 7u), void, __fastcall, void* ecx, void* edx, bf_read& buf, int slot);
CREATE_HOOK(CInput_GetUserCmd, U::VFunc.Get<void*>(I::Input, 8u), CUserCmd*, __fastcall, void* ecx, void* edx, int sequence_number);
CREATE_HOOK(IVModelRender_ForcedMaterialOverride, U::VFunc.Get<void*>(I::ModelRender, 1u), void, __fastcall, void* ecx, void* edx, IMaterial* newMaterial, OverrideType_t nOverrideType);
CREATE_HOOK(IVModelRender_DrawModelExecute, U::VFunc.Get<void*>(I::ModelRender, 19u), void, __fastcall, void* ecx, void* edx, const DrawModelState_t& state, const ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld);
CREATE_HOOK(INetChannel_SendNetMsg, U::Offsets.m_dwSendNetMsg, bool, __fastcall, CNetChan* pThis, void* edx, INetMessage& msg, bool bForceReliable, bool bVoice);
CREATE_HOOK(INetChannel_SendDataGram, U::Offsets.m_dwSendDataGram, int, __fastcall, INetChannel* pThis, void* edx, bf_write* data);
CREATE_HOOK(INetChannel_Shutdown, U::Offsets.m_dwShutdown, void, __fastcall, INetChannel* pThis, void* edx, const char* pReason);
CREATE_HOOK(CSequenceTransitioner_CheckForSequenceChange, U::Offsets.m_dwCheckForSequenceChange, void, __fastcall, void* ecx, void* edx, CStudioHdr* hdr, int nCurSequence, bool bForceNewSequence, bool bInterpolate);
CREATE_HOOK(CTFPlayer_FireBullet, U::Offsets.m_dwFireBullet, void, __fastcall, C_TFPlayer* pThis, void* edx, C_TFWeaponBase* pWpn, FireBulletsInfo_t& info, bool bDoEffects, int nDamageType, int nCustomDamageType);
CREATE_HOOK(CTFWeaponBase_GetTracerType, U::Offsets.m_dwGetTracerType, const char*, __fastcall, C_TFWeaponBase* pThis, void* edx);
CREATE_HOOK(IPanel_PaintTraverse, U::VFunc.Get<void*>(I::VGuiPanel, 41u), void, __fastcall, void* ecx, void* edx, vgui::VPANEL vguiPanel, bool forceRepaint, bool allowForce);
CREATE_HOOK(ISurface_OnScreenSizeChanged, U::VFunc.Get<void*>(I::VGuiSurface, 111u), void, __fastcall, void* ecx, void* edx, int oldWidth, int oldHeight);
CREATE_HOOK(ISurface_LockCursor, U::VFunc.Get<void*>(I::VGuiSurface, 62u), void, __fastcall, void* ecx, void* edx);
CREATE_HOOK(CViewRender_PerformScreenSpaceEffects, *reinterpret_cast<DWORD*>(U::Offsets.m_dwPerformScreenSpaceEffects) + U::Offsets.m_dwPerformScreenSpaceEffects + 0x04, void, __fastcall, void* ecx, void* edx, int x, int y, int w, int h);
CREATE_HOOK(CTFPlayerShared_IsPlayerDominated, U::Offsets.m_dwIsPlayerDominated, bool, __fastcall, void* ecx, void* edx, int iPlayerIndex);
CREATE_HOOK(KeyValues_SetInt, U::Offsets.m_dwSetInt, void, __fastcall, void* ecx, void* edx, const char* keyName, int value);
CREATE_HOOK(CTFPlayerShared_InCond, U::Offsets.m_dwInCond, bool, __fastcall, void* ecx, void* edx, ETFCond eCond);
CREATE_HOOK(CTFViewModel_CalcViewModelView, U::Offsets.m_dwCalcViewModelView, void, __fastcall, void* ecx, void* edx, C_BasePlayer* owner, const Vector& eyePosition, const QAngle& eyeAngles);

class CGlobal_Hooks
{
public:
	bool Initialize();
	bool UnInitialize();
};

namespace G { inline CGlobal_Hooks Hooks; }