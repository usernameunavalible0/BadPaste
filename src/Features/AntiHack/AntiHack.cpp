#include "AntiHack.h"
#include "../Vars.h"
#include "../../Hooks/Hooks.h"

using namespace Hooks::IVModelRender_DrawModelExecute;

void FixMovement(CUserCmd* pCmd, QAngle vOldAngles, float fOldSideMove, float fOldForwardMove)
{
	QAngle curAngs = pCmd->viewangles;

	float fDelta = pCmd->viewangles.y - vOldAngles.y;
	float f1, f2;

	if (vOldAngles.y < 0.0f)
		f1 = 360.0f + vOldAngles.y;

	else f1 = vOldAngles.y;

	if (pCmd->viewangles.y < 0.0f)
		f2 = 360.0f + pCmd->viewangles.y;

	else f2 = pCmd->viewangles.y;

	if (f2 < f1)
		fDelta = abs(f2 - f1);

	else fDelta = 360.0f - abs(f1 - f2);

	fDelta = 360.0f - fDelta;

	pCmd->forwardmove = cos(DEG2RAD(fDelta)) * fOldForwardMove + cos(DEG2RAD(fDelta + 90.0f)) * fOldSideMove;
	pCmd->sidemove = sin(DEG2RAD(fDelta)) * fOldForwardMove + sin(DEG2RAD(fDelta + 90.0f)) * fOldSideMove;
}

void CFeatures_AntiHack::Run(CUserCmd* cmd, bool* pbSendPacket)
{
	g_Globals.m_vRealViewAngles = g_Globals.m_vViewAngles;
	m_vFakeViewAngles.x = g_Globals.m_vViewAngles.x;

	if (!Vars::AntiHack::AntiAim::Enabled.m_Var)
		return;

	C_TFPlayer* pLocal = G::EntityCache.GetLocal();

	if (!pLocal || !pLocal->IsAlive() || pLocal->IsTaunting() || pLocal->IsHalloweenKart() || pLocal->IsHalloweenGhostMode())
		return;

	if (cmd->buttons & IN_ATTACK)
		return;

	bool bPitchSet = true;
	bool bYawSet = true;

	QAngle vOldAngles = cmd->viewangles;
	float fOldSideMove = cmd->sidemove;
	float fOldForwardMove = cmd->forwardmove;

	QAngle vAngles = cmd->viewangles;

	static float flYawRealOffset = 0.0f;

	switch (Vars::AntiHack::AntiAim::Pitch.m_Var) {
	case 1: { cmd->viewangles.x = -89.0f; g_Globals.m_vRealViewAngles.x = -89.0f; break; }
	case 2: { cmd->viewangles.x = 89.0f; g_Globals.m_vRealViewAngles.x = 89.0f; break; }
	case 3: { cmd->viewangles.x = -271.0f; g_Globals.m_vRealViewAngles.x = 89.0f; m_vFakeViewAngles.x = -89.0f; break; }
	case 4: { cmd->viewangles.x = 271.0f; g_Globals.m_vRealViewAngles.x = -89.0f; m_vFakeViewAngles.x = 89.0f; break; }
	default: { bPitchSet = false; break; }
	}

	static bool b = false;

	if (b)
	{
		switch (Vars::AntiHack::AntiAim::YawReal.m_Var) {
		case 1: { cmd->viewangles.y += 90.0f; flYawRealOffset = 90.0f; break; }
		case 2: { cmd->viewangles.y -= 90.0f; flYawRealOffset = -90.0f; break; }
		case 3: { cmd->viewangles.y += 180.0f; flYawRealOffset = 180.0f; break; }
		case 4: { cmd->viewangles.y -= 89.99985719438652715f; flYawRealOffset = -89.99985719438652715f; break; }
		default: { bYawSet = false; break; }
		}

		g_Globals.m_vRealViewAngles.y = cmd->viewangles.y;
	}

	else
	{
		switch (Vars::AntiHack::AntiAim::YawFake.m_Var) {
		case 1: { cmd->viewangles.y += 90.0f; m_vFakeViewAngles.y = 90.0f; break; }
		case 2: { cmd->viewangles.y -= 90.0f; m_vFakeViewAngles.y = -90.0f; break; }
		case 3: { cmd->viewangles.y += 180.0f; m_vFakeViewAngles.y = 180.0f; break; }
		case 4: { float flRand = I::UniformRandomStream->RandomFloat(-180.0f, 180.0f); cmd->viewangles.y += flRand; m_vFakeViewAngles.y = flRand; break; }
		default: { bYawSet = false; break; }
		}

		// This shit is so dumb but it works so whatever
		m_vFakeViewAngles.y = m_vFakeViewAngles.y - flYawRealOffset;
	}

	*pbSendPacket = b = !b;
	//g_Globals.m_bAAActive = bPitchSet || bYawSet;
	FixMovement(cmd, vOldAngles, fOldSideMove, fOldForwardMove);
}

void CFeatures_AntiHack::Render(void* ecx, void* edx, const DrawModelState_t& state, const ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld)
{
	if (!Vars::AntiHack::AntiAim::DrawFakeAngles.m_Var || !Vars::AntiHack::AntiAim::Enabled.m_Var || !I::Input->CAM_IsThirdPerson())
		return;

	static IMaterial* pMatShaded = NULL;
	if (!pMatShaded)
	{
		KeyValues* kv = new KeyValues("VertexLitGeneric");
		kv->SetString("$basetexture", "vgui/white_additive");
		kv->SetString("$bumpmap", "vgui/white_additive");
		kv->SetString("$selfillum", "1");
		kv->SetString("$selfillumfresnel", "1");
		kv->SetString("$selfillumfresnelminmaxexp", "[-0.25 1 1]");
		pMatShaded = I::MaterialSystem->CreateMaterial("AAMaterialShaded", kv);
	}

	if (pInfo.entity_index != g_Globals.m_nLocalIndex)
		return;

	C_TFPlayer* pLocal = UTIL_TFPlayerByIndex(pInfo.entity_index);

	matrix3x4_t BoneMatrix[MAXSTUDIOBONES];

	// Invalidate Bone Cache
	*reinterpret_cast<int*>(reinterpret_cast<DWORD>(pLocal) + 0x59C) = -INT_MAX;
	*reinterpret_cast<float*>(reinterpret_cast<DWORD>(pLocal) + 0x860) = -FLT_MAX;

	if (!m_MatrixHelper.Create(pLocal, m_vFakeViewAngles.x, m_vFakeViewAngles.y, BoneMatrix))
		return;

	I::ModelRender->ForcedMaterialOverride(pMatShaded);

	Func.Original<FN>()(ecx, edx, state, pInfo, BoneMatrix);

	I::ModelRender->ForcedMaterialOverride(nullptr);

	m_MatrixHelper.Restore(pLocal, pLocal->GetAnimState());
}

bool CFeatures_AntiHack::CFakeAngleMatrixHelper::Create(C_TFPlayer* pPlayer, float flPitch, float flYawOffset, matrix3x4_t* pMatrixOut)
{
	if (!pPlayer || pPlayer->deadflag())
		return false;

	auto pAnimState = pPlayer->GetAnimState();

	if (!pAnimState)
		return false;

	m_flOldFrameTime = I::GlobalVars->frametime;
	m_nOldSequence = pPlayer->m_nSequence();
	m_flOldCycle = pPlayer->m_flCycle();
	m_pOldPoseParams = pPlayer->m_flPoseParameter();

	memcpy(m_pOldAnimState, pAnimState, sizeof(CMultiPlayerAnimState));

	I::GlobalVars->frametime = 0.0f;

	pAnimState->m_flCurrentFeetYaw = pAnimState->m_flCurrentFeetYaw + flYawOffset;
	pAnimState->m_flGoalFeetYaw = pAnimState->m_flGoalFeetYaw + flYawOffset;
	pAnimState->Update(pAnimState->m_flEyeYaw + flYawOffset, flPitch);

	if (!pPlayer->SetupBones(pMatrixOut, MAXSTUDIOBONES, BONE_USED_BY_ANYTHING, I::GlobalVars->curtime)) {
		Restore(pPlayer, pAnimState);
		return false;
	}

	return true;
}
