#include "Misc.h"

#include "../Vars.h"

void CFeatures_Misc::Run(C_TFPlayer* pLocal, CUserCmd* cmd)
{
	if (Vars::Misc::Bunnyhop.m_Var)
		Bunnyhop(pLocal, cmd);

	FollowBot(pLocal, cmd);

	AntiBackstab(pLocal, cmd);

	TeleportSpam(pLocal, cmd);

	if (g_Globals.m_nShifted > 0 && g_Globals.m_bShouldShift)
		AntiWarp(pLocal, cmd);
}

void CFeatures_Misc::Bunnyhop(C_TFPlayer* pLocal, CUserCmd* cmd)
{
	static bool s_bState = false;

	if (cmd->buttons & IN_JUMP)
	{
		if (!s_bState && !pLocal->m_fFlags().IsFlagSet(FL_ONGROUND))
		{
			cmd->buttons &= ~IN_JUMP;
		}
		else if (s_bState)
		{
			s_bState = false;
		}
	}
	else if (!s_bState)
	{
		s_bState = true;
	}
}

void CFeatures_Misc::FollowBot(C_TFPlayer* pLocal, CUserCmd* cmd)
{
	static bool bEnabled = false;

	if (GetAsyncKeyState(VK_SHIFT) & 1)
		bEnabled = !bEnabled;

	if (!bEnabled)
		return;

	auto WalkTo = [&](const Vector& vecFrom, const Vector& vecTo, float flScale) -> void
	{
			Vector vecDelta = vecTo - vecFrom;

			if (vecDelta.Length() == 0.0f)
				return;

			Vector vecDeltaMove = { vecDelta.x, vecDelta.y, 0.0f };
			QAngle vecDeltaDir = QAngle();
			VectorAngles(vecDeltaMove, vecDeltaDir);
			float flYaw = DEG2RAD(vecDeltaDir.y - cmd->viewangles.y);
			cmd->forwardmove = cosf(flYaw) * (450.0f * flScale);
			cmd->sidemove = -sinf(flYaw) * (450.0f * flScale);
	};

	for (IClientEntity* pEntity : G::EntityCache.GetGroup(EEntGroup::PLAYERS_ALL))
	{
		C_TFPlayer* pPlayer = pEntity->As<C_TFPlayer*>();

		if (!pPlayer || pPlayer->deadflag())
			continue;

		CSteamID steamID;
		pPlayer->GetSteamID(&steamID);
		if (!(static_cast<uint32>(Vars::Misc::FollowFriendID.m_Var) == steamID.m_steamid.m_comp.m_unAccountID))
			continue;

		cmd->viewangles = pPlayer->EyeAngles();

		if (pPlayer->m_bJumping())
			cmd->buttons |= IN_JUMP;

		if (pPlayer->InCond(TF_COND_AIMING))
			cmd->buttons |= IN_ATTACK2;

		WalkTo(pLocal->m_vecOrigin(), pPlayer->m_vecOrigin(), 1.0f);
	}
}

void CFeatures_Misc::AntiBackstab(C_TFPlayer* pLocal, CUserCmd* cmd)
{
	for (IClientEntity* pEntity : G::EntityCache.GetGroup(EEntGroup::PLAYERS_ENEMIES))
	{
		C_TFPlayer* pPlayer = pEntity->As<C_TFPlayer*>();

		if (pPlayer->deadflag())
			continue;

		C_TFWeaponBase* pWeapon = pPlayer->GetActiveTFWeapon();

		if (!pWeapon)
			continue;

		C_TFKnife* pKnife = pWeapon->As<C_TFKnife*>();

		if (!pKnife)
			continue;

		if (!pKnife->m_bReadyToBackstab())
			continue;

		Vector vecBody; pPlayer->GetHitboxPosition(HITBOX_BODY, vecBody);
		QAngle angTo = GetAngleToPosition(pLocal->Weapon_ShootPosition(), vecBody);
		G::Util.FixMovement(angTo, cmd);
		cmd->viewangles = angTo;
	}
}

void CFeatures_Misc::TeleportSpam(C_TFPlayer* pLocal, CUserCmd* cmd)
{
	auto WalkTo = [&](const Vector& vecFrom, const Vector& vecTo, float flScale) -> void
		{
			Vector vecDelta = vecTo - vecFrom;

			if (vecDelta.Length() == 0.0f)
				return;

			Vector vecDeltaMove = { vecDelta.x, vecDelta.y, 0.0f };
			QAngle vecDeltaDir = QAngle();
			VectorAngles(vecDeltaMove, vecDeltaDir);
			float flYaw = DEG2RAD(vecDeltaDir.y - cmd->viewangles.y);
			cmd->forwardmove = cosf(flYaw) * (450.0f * flScale);
			cmd->sidemove = -sinf(flYaw) * (450.0f * flScale);
		};

	for (IClientEntity* pEntity : G::EntityCache.GetGroup(EEntGroup::BUILDINGS_TEAMMATES))
	{
		C_ObjectTeleporter* pTeleporter = pEntity->As<C_ObjectTeleporter*>();

		if (!pTeleporter || !pTeleporter->IsAlive() || pTeleporter->m_iObjectMode() != MODE_TELEPORTER_ENTRANCE)
			continue;

		C_TFPlayer* pOwner = pTeleporter->m_hBuilder().Get()->As<C_TFPlayer*>();

		if (!pOwner || pOwner->deadflag() || !pOwner->IsPlayerOnSteamFriendsList())
			continue;

		WalkTo(pLocal->m_vecOrigin(), pTeleporter->m_vecOrigin(), 1.0f);
		return;
	}
}

void CFeatures_Misc::AntiWarp(C_TFPlayer* pLocal, CUserCmd* cmd)
{
	auto WalkTo = [&](const Vector& vecFrom, const Vector& vecTo, float flScale) -> void
		{
			Vector vecDelta = vecTo - vecFrom;

			if (vecDelta.Length() == 0.0f)
				return;

			Vector vecDeltaMove = { vecDelta.x, vecDelta.y, 0.0f };
			QAngle vecDeltaDir = QAngle();
			VectorAngles(vecDeltaMove, vecDeltaDir);
			float flYaw = DEG2RAD(vecDeltaDir.y - cmd->viewangles.y);
			cmd->forwardmove = cosf(flYaw) * (450.0f * flScale);
			cmd->sidemove = -sinf(flYaw) * (450.0f * flScale);
		};
}
