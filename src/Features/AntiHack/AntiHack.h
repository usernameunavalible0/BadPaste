#pragma once
#include "../../SDK/SDK.h"
#include "../../SDK/hl2_src/game/shared/Multiplayer/multiplayer_animstate.h"

class CFeatures_AntiHack
{
public:
	void Run(CUserCmd* cmd, bool* pbSendPacket);
	void Render(void* ecx, void* edx, const DrawModelState_t& state, const ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld);

private:
	class CFakeAngleMatrixHelper
	{
	public:
		bool Create(C_TFPlayer* pPlayer, float flPitch, float flYawOffset, matrix3x4_t* pMatrixOut);

		void Restore(C_TFPlayer* pPlayer, CMultiPlayerAnimState* pAnimState)
		{
			I::GlobalVars->frametime = m_flOldFrameTime;
			pPlayer->m_nSequence() = m_nOldSequence;
			pPlayer->m_flCycle() = m_flOldCycle;
			pPlayer->m_flPoseParameter() = m_pOldPoseParams;
			memcpy(pAnimState, m_pOldAnimState, sizeof(CMultiPlayerAnimState));
		}

	private:
		float m_flOldFrameTime;
		int m_nOldSequence;
		float m_flOldCycle;
		std::array<float, MAXSTUDIOPOSEPARAM> m_pOldPoseParams;
		char m_pOldAnimState[sizeof(CMultiPlayerAnimState)] = {};
	};
	CFakeAngleMatrixHelper m_MatrixHelper;

	QAngle m_vFakeViewAngles;
};

namespace F { inline CFeatures_AntiHack AntiHack; }