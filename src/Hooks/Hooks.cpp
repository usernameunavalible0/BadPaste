#include "Hooks.h"
#include "Definitions/WndProc.h"

using namespace Hooks;

bool CGlobal_Hooks::Initialize()
{
	if (MH_Initialize() != MH_STATUS::MH_OK)
		return false;

	CBaseEntity_BaseInterpolatePart1::Initialize();
	CBasePlayer_CalcPlayerView::Initialize();
	CL_LoadWhitelist::Initialize();
	CL_Move::Initialize();
	CL_SendMove::Initialize();
	IBaseClientDLL_LevelInitPreEntity::Initialize();
	IBaseClientDLL_LevelInitPostEntity::Initialize();
	IBaseClientDLL_LevelShutdown::Initialize();
	IBaseClientDLL_FrameStageNotify::Initialize();
	IBaseClientDLL_DispatchUserMessage::Initialize();
	IClientEntityList_OnAddEntity::Initialize();
	IClientEntityList_OnRemoveEntity::Initialize();
	ClientModeShared_OverrideView::Initialize();
	ClientModeShared_CreateMove::Initialize();
	ClientModeShared_ShouldDrawViewModel::Initialize();
	ClientModeShared_GetViewModelFOV::Initialize();
	ClientModeShared_DoPostScreenSpaceEffects::Initialize();
	ClientModeShared_ShouldBlackoutAroundHUD::Initialize();
	CPrediction_RunCommand::Initialize();
	IVEngineClient_IsPlayingTimeDemo::Initialize();
	IEngineVGui_Paint::Initialize();
	CInput_WriteUsercmdDeltaToBuffer::Initialize();
	CInput_EncodeUserCmdToBuffer::Initialize();
	CInput_DecodeUserCmdFromBuffer::Initialize();
	CInput_GetUserCmd::Initialize();
	IVModelRender_ForcedMaterialOverride::Initialize();
	IVModelRender_DrawModelExecute::Initialize();
	INetChannel_SendNetMsg::Initialize();
	INetChannel_SendDataGram::Initialize();
	INetChannel_Shutdown::Initialize();
	CSequenceTransitioner_CheckForSequenceChange::Initialize();
	CTFPlayer_FireBullet::Initialize();
	CTFWeaponBase_GetTracerType::Initialize();
	IPanel_PaintTraverse::Initialize();
	ISurface_OnScreenSizeChanged::Initialize();
	ISurface_LockCursor::Initialize();
	CViewRender_PerformScreenSpaceEffects::Initialize();
	CTFPlayerShared_IsPlayerDominated::Initialize();
	KeyValues_SetInt::Initialize();
	CTFPlayerShared_InCond::Initialize();
	CTFViewModel_CalcViewModelView::Initialize();
	WndProc::Initialize();

	if (MH_EnableHook(MH_ALL_HOOKS) != MH_STATUS::MH_OK)
		return false;

	//If we arrive here, return success.
	return true;
}

bool CGlobal_Hooks::UnInitialize()
{
	WndProc::Uninitialize();

	if (MH_Uninitialize() != MH_STATUS::MH_OK)
		return false;

	//If we arrive here, return success.
	return true;
}
