//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//
#ifndef MULTIPLAYERANIMSTATE_H
#define MULTIPLAYERANIMSTATE_H
#ifdef _WIN32
#pragma once
#endif

#include "../../../public/tier1/convar.h"
#include "../basecombatweapon_shared.h"
#include "../iplayeranimstate.h"

enum PlayerAnimEvent_t
{
	PLAYERANIMEVENT_ATTACK_PRIMARY,
	PLAYERANIMEVENT_ATTACK_SECONDARY,
	PLAYERANIMEVENT_ATTACK_GRENADE,
	PLAYERANIMEVENT_RELOAD,
	PLAYERANIMEVENT_RELOAD_LOOP,
	PLAYERANIMEVENT_RELOAD_END,
	PLAYERANIMEVENT_JUMP,
	PLAYERANIMEVENT_SWIM,
	PLAYERANIMEVENT_DIE,
	PLAYERANIMEVENT_FLINCH_CHEST,
	PLAYERANIMEVENT_FLINCH_HEAD,
	PLAYERANIMEVENT_FLINCH_LEFTARM,
	PLAYERANIMEVENT_FLINCH_RIGHTARM,
	PLAYERANIMEVENT_FLINCH_LEFTLEG,
	PLAYERANIMEVENT_FLINCH_RIGHTLEG,
	PLAYERANIMEVENT_DOUBLEJUMP,

	// Cancel.
	PLAYERANIMEVENT_CANCEL,
	PLAYERANIMEVENT_SPAWN,

	// Snap to current yaw exactly
	PLAYERANIMEVENT_SNAP_YAW,

	PLAYERANIMEVENT_CUSTOM,				// Used to play specific activities
	PLAYERANIMEVENT_CUSTOM_GESTURE,
	PLAYERANIMEVENT_CUSTOM_SEQUENCE,	// Used to play specific sequences
	PLAYERANIMEVENT_CUSTOM_GESTURE_SEQUENCE,

	// TF Specific. Here until there's a derived game solution to this.
	PLAYERANIMEVENT_ATTACK_PRE,
	PLAYERANIMEVENT_ATTACK_POST,
	PLAYERANIMEVENT_GRENADE1_DRAW,
	PLAYERANIMEVENT_GRENADE2_DRAW,
	PLAYERANIMEVENT_GRENADE1_THROW,
	PLAYERANIMEVENT_GRENADE2_THROW,
	PLAYERANIMEVENT_VOICE_COMMAND_GESTURE,
	PLAYERANIMEVENT_DOUBLEJUMP_CROUCH,
	PLAYERANIMEVENT_STUN_BEGIN,
	PLAYERANIMEVENT_STUN_MIDDLE,
	PLAYERANIMEVENT_STUN_END,
	PLAYERANIMEVENT_PASSTIME_THROW_BEGIN,
	PLAYERANIMEVENT_PASSTIME_THROW_MIDDLE,
	PLAYERANIMEVENT_PASSTIME_THROW_END,
	PLAYERANIMEVENT_PASSTIME_THROW_CANCEL,

	PLAYERANIMEVENT_ATTACK_PRIMARY_SUPER,

	PLAYERANIMEVENT_COUNT
};

// Gesture Slots.
enum
{
	GESTURE_SLOT_ATTACK_AND_RELOAD,
	GESTURE_SLOT_GRENADE,
	GESTURE_SLOT_JUMP,
	GESTURE_SLOT_SWIM,
	GESTURE_SLOT_FLINCH,
	GESTURE_SLOT_VCD,
	GESTURE_SLOT_CUSTOM,

	GESTURE_SLOT_COUNT,
};

#define GESTURE_SLOT_INVALID	-1

struct GestureSlot_t
{
	int					m_iGestureSlot;
	Activity			m_iActivity;
	bool				m_bAutoKill;
	bool				m_bActive;
	C_AnimationLayer* m_pAnimLayer;
};

inline bool IsCustomPlayerAnimEvent(PlayerAnimEvent_t event)
{
	return (event == PLAYERANIMEVENT_CUSTOM) || (event == PLAYERANIMEVENT_CUSTOM_GESTURE) ||
		(event == PLAYERANIMEVENT_CUSTOM_SEQUENCE) || (event == PLAYERANIMEVENT_CUSTOM_GESTURE_SEQUENCE);
}

struct MultiPlayerPoseData_t
{
	int			m_iMoveX;
	int			m_iMoveY;
	int			m_iAimYaw;
	int			m_iAimPitch;
	int			m_iBodyHeight;
	int			m_iMoveYaw;
	int			m_iMoveScale;

	float		m_flEstimateYaw;
	float		m_flLastAimTurnTime;

	void Init()
	{
		m_iMoveX = 0;
		m_iMoveY = 0;
		m_iAimYaw = 0;
		m_iAimPitch = 0;
		m_iBodyHeight = 0;
		m_iMoveYaw = 0;
		m_iMoveScale = 0;
		m_flEstimateYaw = 0.0f;
		m_flLastAimTurnTime = 0.0f;
	}
};

struct DebugPlayerAnimData_t
{
	float		m_flSpeed;
	float		m_flAimPitch;
	float		m_flAimYaw;
	float		m_flBodyHeight;
	Vector2D	m_vecMoveYaw;

	void Init()
	{
		m_flSpeed = 0.0f;
		m_flAimPitch = 0.0f;
		m_flAimYaw = 0.0f;
		m_flBodyHeight = 0.0f;
		m_vecMoveYaw.Init();
	}
};

struct MultiPlayerMovementData_t
{
	// Set speeds to -1 if they are not used.
	float		m_flWalkSpeed;
	float		m_flRunSpeed;
	float		m_flSprintSpeed;
	float		m_flBodyYawRate;
};


//=============================================================================
//
// Multi-Player Animation State
//
class CMultiPlayerAnimState
{
public:
	virtual ~CMultiPlayerAnimState() = 0;

	// This is called by both the client and the server in the same way to trigger events for
	// players firing, jumping, throwing grenades, etc.
	virtual void ClearAnimationState() = 0;
	virtual void DoAnimationEvent(PlayerAnimEvent_t event, int nData = 0) = 0;
	virtual Activity CalcMainActivity() = 0;
	virtual void Update(float eyeYaw, float eyePitch) = 0;
	virtual void Release(void) = 0;

	virtual Activity TranslateActivity(Activity actDesired) = 0;

	virtual void SetRunSpeed(float flSpeed) = 0;
	virtual void SetWalkSpeed(float flSpeed) = 0;
	virtual void SetSprintSpeed(float flSpeed) = 0;

	// Debug
	virtual void ShowDebugInfo(void) = 0;
	virtual void DebugShowAnimState(int iStartLine) = 0;

	// Feet.
	// If you are forcing aim yaw, your code is almost definitely broken if you don't include a delay between 
	// teleporting and forcing yaw. This is due to an unfortunate interaction between the command lookback window,
	// and the fact that m_flEyeYaw is never propogated from the server to the client.
	// TODO: Fix this after Halloween 2014.
	bool	m_bForceAimYaw;

	virtual void Init(C_BasePlayer* pPlayer, MultiPlayerMovementData_t& movementData) = 0;

	// Allow inheriting classes to override SelectWeightedSequence
	virtual int SelectWeightedSequence(Activity activity) = 0;
	virtual void RestartMainSequence() = 0;

	virtual void GetOuterAbsVelocity(Vector& vel) = 0;

	virtual bool HandleJumping(Activity& idealActivity) = 0;
	virtual bool HandleDucking(Activity& idealActivity) = 0;
	virtual bool HandleMoving(Activity& idealActivity) = 0;
	virtual bool HandleSwimming(Activity& idealActivity) = 0;
	virtual bool HandleDying(Activity& idealActivity) = 0;

	// Gesture Slots
	CUtlVector<GestureSlot_t>		m_aGestureSlots;
	virtual void RestartGesture(int iGestureSlot, Activity iGestureActivity, bool bAutoKill = true) = 0;
	virtual float	GetGesturePlaybackRate(void) = 0;

	virtual void PlayFlinchGesture(Activity iActivity) = 0;

	virtual float CalcMovementSpeed(bool* bIsMoving) = 0;
	virtual float CalcMovementPlaybackRate(bool* bIsMoving) = 0;

	// Pose parameters.
	virtual void		ComputePoseParam_MoveYaw(CStudioHdr* pStudioHdr) = 0;
	virtual void		ComputePoseParam_AimPitch(CStudioHdr* pStudioHdr) = 0;
	virtual void		ComputePoseParam_AimYaw(CStudioHdr* pStudioHdr) = 0;
	virtual void		EstimateYaw(void) = 0;

	virtual float GetCurrentMaxGroundSpeed() = 0;
	virtual void ComputeSequences(CStudioHdr* pStudioHdr) = 0;

	virtual bool ShouldUpdateAnimState() = 0;

	C_BasePlayer* m_pPlayer;

	QAngle				m_angRender;

	// Pose parameters.
	bool						m_bPoseParameterInit;
	MultiPlayerPoseData_t		m_PoseParameterData;
	DebugPlayerAnimData_t		m_DebugAnimData;

	bool						m_bCurrentFeetYawInitialized;
	float						m_flLastAnimationStateClearTime;

	float m_flEyeYaw;
	float m_flEyePitch;
	float m_flGoalFeetYaw;
	float m_flCurrentFeetYaw;
	float m_flLastAimTurnTime;

	MultiPlayerMovementData_t	m_MovementData;

	// Jumping.
	bool	m_bJumping;
	float	m_flJumpStartTime;
	bool	m_bFirstJumpFrame;

	// Swimming.
	bool	m_bInSwim;
	bool	m_bFirstSwimFrame;

	// Dying
	bool	m_bDying;
	bool	m_bFirstDyingFrame;

	// Last activity we've used on the lower body. Used to determine if animations should restart.
	Activity m_eCurrentMainSequenceActivity;

	// Specific full-body sequence to play
	int		m_nSpecificMainSequence;

	// Weapon data.
	CHandle<C_BaseCombatWeapon>	m_hActiveWeapon;

	// Ground speed interpolators.
	float m_flLastGroundSpeedUpdateTime;
	// I dont feel like implementing interpolatedvar rn so this is being padded out
	//CInterpolatedVar<float> m_iv_flMaxGroundSpeed;
	char pad_00[48];
	float m_flMaxGroundSpeed;

	// movement playback options
	int m_nMovementSequence;
	LegAnimType_t m_LegAnimType;
};

#endif // DOD_PLAYERANIMSTATE_H