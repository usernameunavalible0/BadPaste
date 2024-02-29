#include "AimbotMelee.h"
#include "../../Vars.h"

void CAimbot_Melee::Run(C_TFPlayer* pLocal, C_TFWeaponBase* pWeapon, CUserCmd* cmd)
{
	if (!Vars::Aimbot::Melee::Enabled.m_Var)
		return;

	C_TFWeaponBaseMelee* pMelee = pWeapon->As<C_TFWeaponBaseMelee*>();

	//This should be covered by the function that calls this one but I will leave this here just in case
	if (!pMelee)
		return;

	Target_t Target = {};

	const bool bShouldAim = (Vars::Aimbot::Global::AimKey.m_Var == VK_LBUTTON ? (cmd->buttons & IN_ATTACK) : A::Global.IsKeyDown());

	if (!bShouldAim)
		return;

	if (!GetTarget(pLocal, pMelee, Target))
		return;

	if (Vars::Aimbot::Global::AutoShoot.m_Var)
		cmd->buttons |= IN_ATTACK;

	if (Vars::Aimbot::Melee::AimMethod.m_Var == 2)
	{
		if (IsAttacking(cmd, pMelee))
		{
			Aim(pLocal, cmd, Target.m_vAngleTo);
			g_Globals.m_bSilentTime = true;
		}
	}
	else Aim(pLocal, cmd, Target.m_vAngleTo);
}

const ESortMethod CAimbot_Melee::GetSortMethod()
{
	switch (Vars::Aimbot::Melee::SortMethod.m_Var) {
	case 0: return ESortMethod::FOV;
	case 1: return ESortMethod::DISTANCE;
	default: return ESortMethod::UNKNOWN;
	}
}

bool CAimbot_Melee::IsAttacking(CUserCmd* cmd, C_TFWeaponBaseMelee* pWeapon)
{
	if (pWeapon->GetWeaponID() == TF_WEAPON_KNIFE)
		return ((cmd->buttons & IN_ATTACK) && pWeapon->CanAttack());
	else
		return fabsf(pWeapon->m_flSmackTime() - I::GlobalVars->curtime) < I::GlobalVars->interval_per_tick * 2.0f;
}

void CAimbot_Melee::Aim(C_TFPlayer* pLocal, CUserCmd* cmd, QAngle& vAngle)
{
	vAngle -= pLocal->m_vecPunchAngle();
	ClampAngles(vAngle);

	switch (Vars::Aimbot::Melee::AimMethod.m_Var)
	{
	case 0: {
		cmd->viewangles = vAngle;
		I::EngineClient->SetViewAngles(cmd->viewangles);
		break;
	}

	case 1: {
		QAngle vecDelta = vAngle - cmd->viewangles;
		ClampAngles(vecDelta);
		cmd->viewangles += vecDelta / Vars::Aimbot::Melee::SmoothingAmount.m_Var;
		I::EngineClient->SetViewAngles(cmd->viewangles);
		break;
	}

	case 2: {
		G::Util.FixMovement(vAngle, cmd);
		cmd->viewangles = vAngle;
		break;
	}

	default: break;
	}
}

bool CAimbot_Melee::GetTargets(C_TFPlayer* pLocal, C_TFWeaponBaseMelee* pWeapon)
{
	const ESortMethod SortMethod = GetSortMethod();

	if (SortMethod == ESortMethod::FOV)
		g_Globals.m_flCurAimFOV = Vars::Aimbot::Melee::AimFOV.m_Var;

	A::Global.m_vecTargets.RemoveAll();

	Vector vecSrc = pLocal->Weapon_ShootPosition();
	QAngle vecAngles; I::EngineClient->GetViewAngles(vecAngles);

	if (Vars::Aimbot::Global::Targets[0])
	{
		const bool bWhipTeam = (pWeapon->m_iItemDefinitionIndex() == Soldier_t_TheDisciplinaryAction && Vars::Aimbot::Melee::WhipTeam.m_Var);

		for (auto pEntity : G::EntityCache.GetGroup(bWhipTeam ? EEntGroup::PLAYERS_ALL : EEntGroup::PLAYERS_ENEMIES))
		{
			C_TFPlayer* pPlayer = pEntity->As<C_TFPlayer*>();

			if (pPlayer->deadflag() || pPlayer->IsHalloweenGhostMode())
				continue;

			if (Vars::Aimbot::Global::Ignores[0] && pPlayer->IsInvulnerable())
				continue;

			if (Vars::Aimbot::Global::Ignores[1] && pPlayer->IsCloaked())
				continue;

			if (Vars::Aimbot::Global::Ignores[2] && pPlayer->IsTaunting())
				continue;

			if (Vars::Aimbot::Global::Ignores[3] && pPlayer->IsPlayerOnSteamFriendsList())
				continue;

			Vector vecBody; pPlayer->GetHitboxPosition(HITBOX_BODY, vecBody);
			QAngle vecAngleTo = GetAngleToPosition(vecSrc, vecBody);
			float flFOVTo = GetFovToPosition(vecAngles, vecAngleTo);
			float flDistTo = vecSrc.DistTo(vecBody);

			if (SortMethod == ESortMethod::FOV && flFOVTo > Vars::Aimbot::Melee::AimFOV.m_Var)
				continue;

			A::Global.m_vecTargets.AddToTail({ pPlayer, ETargetType::PLAYER, vecBody, vecAngleTo, flFOVTo, flDistTo });
		}
	}

	if (Vars::Aimbot::Global::Targets[1])
	{
		for (auto pEntity : G::EntityCache.GetGroup(EEntGroup::BUILDINGS_ENEMIES))
		{
			C_BaseObject* pBuilding = pEntity->As<C_BaseObject*>();

			if (!pBuilding->IsAlive())
				continue;

			Vector vecCenter = pBuilding->WorldSpaceCenter();
			QAngle vecAngleTo = GetAngleToPosition(vecSrc, vecCenter);
			float flFOVTo = GetFovToPosition(vecAngles, vecAngleTo);
			float flDistTo = vecSrc.DistTo(vecCenter);

			if (SortMethod == ESortMethod::FOV && flFOVTo > Vars::Aimbot::Melee::AimFOV.m_Var)
				continue;

			A::Global.m_vecTargets.AddToTail({ pBuilding, ETargetType::BUILDING, vecCenter, vecAngleTo, flFOVTo, flDistTo });
		}
	}

	return !A::Global.m_vecTargets.IsEmpty();
}

bool CAimbot_Melee::GetTarget(C_TFPlayer* pLocal, C_TFWeaponBaseMelee* pWeapon, Target_t& Out)
{
	if (!GetTargets(pLocal, pWeapon))
		return false;

	A::Global.SortTargets(GetSortMethod());
	
	Vector vecSrc = pLocal->Weapon_ShootPosition();

	for (Target_t& Target : A::Global.m_vecTargets)
	{
		if (Vars::Aimbot::Melee::RangeCheck.m_Var)
		{
			trace_t tr;
			if (!pWeapon->DoSwingTraceInternal(tr, NULL, Target.m_vAngleTo))
				continue;
		}
		else
		{
			if (!G::Util.IsVisible(vecSrc, Target.m_vPos))
				continue;
		}

		Out = Target;
		return true;
	}

	return false;
}
