#include "AimbotMelee.h"
#include "../../Vars.h"

bool CAimbot_Melee::CanMeleeHit(C_TFPlayer* pLocal, C_TFWeaponBaseMelee* pWeapon, C_BaseEntity* pEntity)
{
    trace_t trace;
    if (pWeapon->DoSwingTrace(trace))
    {
        if (trace.m_pEnt && trace.m_pEnt == pEntity)
            return true;
    }

    return false;
}

ESortMethod CAimbot_Melee::GetSortMethod()
{
    switch (Vars::Aimbot::Melee::SortMethod.m_Var) {
    case 0: return ESortMethod::FOV;
    case 1: return ESortMethod::DISTANCE;
    default: return ESortMethod::UNKNOWN;
    }
}

bool CAimbot_Melee::GetTargets(C_TFPlayer* pLocal, C_TFWeaponBaseMelee* pWeapon)
{
	ESortMethod SortMethod = GetSortMethod();

	if (SortMethod == ESortMethod::FOV)
		g_Globals.m_flAimFov = Vars::Aimbot::Melee::AimFOV.m_Var;

	A::Global.m_vecTargets.Purge();

	Vector vLocalPos = pLocal->Weapon_ShootPosition();
	QAngle vLocalAngles;
	I::EngineClient->GetViewAngles(vLocalAngles);

	if (Vars::Aimbot::Global::AimPlayers.m_Var)
	{
		const bool bWhipTeam = (pWeapon->m_iItemDefinitionIndex() == Soldier_t_TheDisciplinaryAction && Vars::Aimbot::Melee::WhipTeam.m_Var);

		for (auto pEntity : G::EntityCache.GetGroup(bWhipTeam ? EEntGroup::PLAYERS_ALL : EEntGroup::PLAYERS_ENEMIES))
		{
			C_TFPlayer* pPlayer = pEntity->As<C_TFPlayer*>();

			if (!pPlayer->IsAlive() || pPlayer->InCond(TF_COND_HALLOWEEN_GHOST_MODE))
				continue;

			if (Vars::Aimbot::Global::IgnoreInvlunerable.m_Var && pPlayer->IsInvulnerable())
				continue;

			if (Vars::Aimbot::Global::IgnoreCloaked.m_Var && pPlayer->IsCloaked())
				continue;

			if (Vars::Aimbot::Global::IgnoreTaunting.m_Var && pPlayer->InCond(TF_COND_TAUNTING))
				continue;

			if (Vars::Aimbot::Global::IgnoreFriends.m_Var && pPlayer->IsPlayerOnSteamFriendsList())
				continue;

			Vector vPos;
			pPlayer->GetHitboxPosition(HITBOX_BODY, vPos);
			QAngle vAngleTo = GetAngleToPosition(vLocalPos, vPos);
			float flFOVTo = SortMethod == ESortMethod::FOV ? GetFovToPosition(vLocalAngles, vAngleTo) : 0.0f;
			float flDistTo = SortMethod == ESortMethod::DISTANCE ? vLocalPos.DistTo(vPos) : 0.0f;

			if (SortMethod == ESortMethod::FOV && flFOVTo > Vars::Aimbot::Melee::AimFOV.m_Var)
				continue;

			A::Global.m_vecTargets.AddToTail({ pPlayer, ETargetType::PLAYER, vPos, vAngleTo, flFOVTo, flDistTo });
		}
	}

	if (Vars::Aimbot::Global::AimBuildings.m_Var)
	{
		for (auto pEntity : G::EntityCache.GetGroup(EEntGroup::BUILDINGS_ENEMIES))
		{
			C_BaseObject* pBuilding = pEntity->As<C_BaseObject*>();

			if (!pBuilding->IsAlive())
				continue;

			Vector vPos = pBuilding->WorldSpaceCenter();
			QAngle vAngleTo = GetAngleToPosition(vLocalPos, vPos);
			float flFOVTo = SortMethod == ESortMethod::FOV ? GetFovToPosition(vLocalAngles, vAngleTo) : 0.0f;
			float flDistTo = SortMethod == ESortMethod::DISTANCE ? vLocalPos.DistTo(vPos) : 0.0f;

			if (SortMethod == ESortMethod::FOV && flFOVTo > Vars::Aimbot::Melee::AimFOV.m_Var)
				continue;

			A::Global.m_vecTargets.AddToTail({ pBuilding, ETargetType::BUILDING, vPos, vAngleTo, flFOVTo, flDistTo });
		}
	}

	return !A::Global.m_vecTargets.IsEmpty();
}

bool CAimbot_Melee::VerifyTarget(C_TFPlayer* pLocal, C_TFWeaponBaseMelee* pWeapon, Target_t& Target)
{
	return false;
}

bool CAimbot_Melee::GetTarget(C_TFPlayer* pLocal, C_TFWeaponBaseMelee* pWeapon, Target_t& Out)
{
	return false;
}

void CAimbot_Melee::Aim(CUserCmd* pCmd, QAngle& vAngle)
{

}

bool CAimbot_Melee::ShouldSwing(C_TFPlayer* pLocal, C_TFWeaponBaseMelee* pWeapon, CUserCmd* pCmd, const Target_t& Target)
{
	return false;
}

bool CAimbot_Melee::IsAttacking(CUserCmd* pCmd, C_TFWeaponBaseMelee* pWeapon)
{
	return false;
}

void CAimbot_Melee::Run(C_TFPlayer* pLocal, C_TFWeaponBase* pWeapon, CUserCmd* pCmd)
{

}
