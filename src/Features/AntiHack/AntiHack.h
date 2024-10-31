#pragma once
#include "../../SDK/SDK.h"
#include "../Vars.h"
#include "../../SDK/hl2_src/game/shared/Multiplayer/multiplayer_animstate.h"

class CFeatures_AntiHack
{
public:
	void Run(CUserCmd* cmd, bool *pbSendPacket);
	void ResolvePlayer(C_TFPlayer* pTarget, CUserCmd* cmd);
	void Render(void* ecx, void* edx, const DrawModelState_t& state, const ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld);
	void ResolverSpew();

	inline void AddOnShotAngles(const int ent_idx, const QAngle vecAngles)
	{
		C_TFPlayer* pPlayer = UTIL_TFPlayerByIndex(ent_idx);

		if (!pPlayer)
			return;

		OnShotData_t OnShotData = {};
		OnShotData.m_angRotation = vecAngles;
		OnShotData.m_flSimulationTime = pPlayer->m_flSimulationTime();
		m_OnShotData.emplace(ent_idx, OnShotData);

		player_info_t pi;
		if (Vars::AntiHack::Resolver::ResolverSpew.m_Var && I::EngineClient->GetPlayerInfo(ent_idx, &pi))
		{
			uchar16 szName[MAX_PLAYER_NAME_LENGTH];
			Q_UTF8ToUTF16(UTIL_SafeName(pi.name), szName, MAX_PLAYER_NAME_LENGTH);
			const wchar_t* szMsg = L"Resolver: Gathered shot angles from player ";
			wchar_t* szFinalMsg = new wchar_t[wcslen(szMsg) + wcslen(szName) + 1];
			wcscpy(szFinalMsg, szMsg);
			wcscat(szFinalMsg, szName);
			m_ResolverOutput.AddToTail({ szFinalMsg, I::EngineClient->Time() });
		}
	}

	inline bool GetPitchFromSniperDot(C_TFPlayer* pPlayer, float& flPitch)
	{
		for (int n = 0; n < (g_Globals.m_nMaxEntities + 1); n++)
		{
			C_SniperDot* pSniperDot = UTIL_EntityAs<C_SniperDot*>(n);

			if (!pSniperDot || pSniperDot->IsDormant() || pSniperDot->m_hOwnerEntity().Get() != pPlayer)
				continue;

			Vector vecDelta = pSniperDot->m_vecOrigin() - pPlayer->Weapon_ShootPosition();
			QAngle vecAngle;
			VectorAngles(vecDelta, vecAngle);
			flPitch = vecAngle.x;
			return true;
		}

		return false;
	}

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

	struct ResolverSpew_t
	{
		const wchar_t* m_szMsg;
		float m_flTimeAdded;
		float m_flDuration = 5.f;
	};
	CUtlVector<ResolverSpew_t> m_ResolverOutput;
	struct OnShotData_t
	{
		QAngle m_angRotation;
		float m_flSimulationTime;
	};
	std::map<int, OnShotData_t> m_OnShotData;
	QAngle m_ResolverData[MAX_PLAYERS];
	bool m_PitchValid[MAX_PLAYERS];
	bool m_YawValid[MAX_PLAYERS];
};

namespace F { inline CFeatures_AntiHack AntiHack; }