#include "Auto.h"
#include "../Backtrack/Backtrack.h"
#include "../Vars.h"

void CFeatures_Auto::Run(C_TFPlayer* pLocal, C_TFWeaponBase* pWeapon, CUserCmd* cmd)
{
	if (Vars::Auto::AutoBackstab.m_Var)
		AutoBackstab(pLocal, pWeapon, cmd);

	AutoHeal(pLocal, pWeapon, cmd);
}

void CFeatures_Auto::AutoBackstab(C_TFPlayer* pLocal, C_TFWeaponBase* pWeapon, CUserCmd* cmd)
{
	C_TFKnife* pKnife = pWeapon->As<C_TFKnife*>();

	if (!pKnife)
		return;

	if (Vars::Auto::AutoBackstab.m_Var == 1) //Legit
	{
		if (pKnife->m_bReadyToBackstab())
			cmd->buttons |= IN_ATTACK;
	}
	else if (Vars::Auto::AutoBackstab.m_Var == 2) //Rage
	{
		if (!pKnife->CanPrimaryAttack())
			return;

		for (auto pEntity : G::EntityCache.GetGroup(EEntGroup::PLAYERS_ENEMIES))
		{
			C_TFPlayer* pTarget = pEntity->As<C_TFPlayer*>();

			std::function<bool()> CanBackstab = [pTarget, pKnife, pLocal]()
			{
				Vector vBody; pTarget->GetHitboxPosition(HITBOX_BODY, vBody);
				QAngle vAngleTo = GetAngleToPosition(pLocal->Weapon_ShootPosition(), vBody);

				trace_t tr;
				if (!pKnife->DoSwingTraceKnife(tr, vAngleTo))
					return false;

				if (!tr.m_pEnt || tr.m_pEnt != pTarget)
					return false;

				if (!pKnife->CanPerformBackstabAgainstTarget(pTarget, vAngleTo))
					return false;

				return true;
			};

			F::Backtrack.StartBacktracking(pTarget, cmd, CanBackstab);

			if (CanBackstab())
			{
				Vector vBody; pTarget->GetHitboxPosition(HITBOX_BODY, vBody);
				QAngle vAngleTo = GetAngleToPosition(pLocal->Weapon_ShootPosition(), vBody);
				G::Util.FixMovement(vAngleTo, cmd);
				cmd->viewangles = vAngleTo;
				cmd->buttons |= IN_ATTACK;
				g_Globals.m_bChoking = true;
			}

			F::Backtrack.FinishBacktracking();
		}
	}
}

void CFeatures_Auto::AutoHeal(C_TFPlayer* pLocal, C_TFWeaponBase* pWeapon, CUserCmd* cmd)
{
	if (!Vars::Auto::AutoHeal.m_Var)
		return;

	C_WeaponMedigun* pMedigun = pWeapon->As<C_WeaponMedigun*>();

	if (!pMedigun)
		return;

	//Look for players to heal (only if no more players to heal)
	if (m_vecHealablePlayers.IsEmpty())
	{
		for (IClientEntity* pEntity : G::EntityCache.GetGroup(EEntGroup::PLAYERS_TEAMMATES))
		{
			C_TFPlayer* pPlayer = pEntity->As<C_TFPlayer*>();

			Vector vecBody; pPlayer->GetHitboxPosition(HITBOX_BODY, vecBody);
			QAngle angTo = GetAngleToPosition(pLocal->Weapon_ShootPosition(), vecBody);

			if (pPlayer->deadflag() || !pMedigun->CanHealNewTarget(pPlayer, angTo) || (Vars::Auto::AutoHeal.m_Var == 2 && !pPlayer->IsPlayerOnSteamFriendsList()))
				continue;

			m_vecHealablePlayers.AddToTail(pPlayer->entindex());
		}
	}

	//Heal Those Players
	if (!m_vecHealablePlayers.IsEmpty() && pMedigun->CanPrimaryAttack())
	{
		for (int n : m_vecHealablePlayers)
		{
			C_TFPlayer* pPlayer = UTIL_TFPlayerByIndex(n);

			// Sometimes it just happens
			if (!pPlayer)
			{
				m_vecHealablePlayers.FindAndFastRemove(n);
				continue;
			}

			Vector vecBody; pPlayer->GetHitboxPosition(HITBOX_BODY, vecBody);
			QAngle angTo = GetAngleToPosition(pLocal->Weapon_ShootPosition(), vecBody);

			//Double check his ass
			if (pPlayer->deadflag() || !pMedigun->CanHealNewTarget(pPlayer, angTo) || (Vars::Auto::AutoHeal.m_Var == 2 && !pPlayer->IsPlayerOnSteamFriendsList()))
			{
				m_vecHealablePlayers.FindAndFastRemove(n);
				continue;
			}
			
			if (!(cmd->command_number % 2))
			{
				cmd->buttons &= ~IN_ATTACK;
				return;
			}

			m_vecHealablePlayers.FindAndFastRemove(n);
			G::Util.FixMovement(angTo, cmd);
			cmd->viewangles = angTo;
			cmd->buttons |= IN_ATTACK;
			g_Globals.m_bChoking = true;
			return;
		}
	}
}
