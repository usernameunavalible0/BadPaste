#include "AimbotHitscan.h"
#include "../../Vars.h"
#include "../../Backtrack/Backtrack.h"
#include "../../AntiHack/AntiHack.h"

void CAimbot_Hitscan::Run(C_TFPlayer* pLocal, C_TFWeaponBase* pWeapon, CUserCmd* cmd)
{
	if (!Vars::Aimbot::Hitscan::Enabled.m_Var)
		return;

	if (GetSortMethod() == ESortMethod::FOV)
		g_Globals.m_flCurAimFOV = Vars::Aimbot::Hitscan::AimFOV.m_Var;

	Target_t Target = {};

	const bool bShouldAim = (Vars::Aimbot::Global::AimKey.m_Var == VK_LBUTTON) ? (cmd->buttons & IN_ATTACK) : A::Global.IsKeyDown();

	//Shtewpid check for sniperrifle so my onshot window is not bigger that 1 tick
	if (!bShouldAim || (pWeapon->GetWeaponID() == TF_WEAPON_SNIPERRIFLE && !pWeapon->CanPrimaryAttack()))
		return;

	if (!GetTarget(pLocal, pWeapon, cmd, Target))
		return;

	Aim(pLocal, cmd, Target.m_vAngleTo);

	if (ShouldFire(pLocal, pWeapon, cmd, Target))
	{
		cmd->buttons |= IN_ATTACK;

		if (Vars::Misc::CL_Move::Enabled.m_Var && Vars::Misc::CL_Move::Doubletap.m_Var && (cmd->buttons & IN_ATTACK) && !g_Globals.m_nShifted && !g_Globals.m_nWaitForShift)
		{
			if (pLocal->m_iClass() == TF_CLASS_HEAVYWEAPONS && pWeapon->GetSlot() == 0 && !pLocal->m_vecVelocity().IsZero())
				g_Globals.m_bShouldShift = false;
			else
				g_Globals.m_bShouldShift = true;
		}
	}
}

int CAimbot_Hitscan::GetBestHitbox(C_TFPlayer* pLocal, C_TFWeaponBase* pWeapon)
{
	switch (Vars::Aimbot::Hitscan::PreferedHitbox.m_Var)
	{
	case 0: return HITBOX_HEAD;
	case 1: return HITBOX_BODY;
	case 2:
	{
		const int iDefIndex = pWeapon->m_iItemDefinitionIndex();

		if (pLocal->IsClass(TF_CLASS_SNIPER))
		{
			C_TFSniperRifle* pRifle = pWeapon->As<C_TFSniperRifle*>();

			if (pRifle && (pRifle->GetRifleType() != RifleTypes_t::RIFLE_JARATE) && pLocal->IsZoomed())
				return HITBOX_HEAD;

			return HITBOX_BODY;
		}
		else if (pLocal->IsClass(TF_CLASS_SPY))
		{
			const bool bIsAmbassador = (iDefIndex == Spy_m_TheAmbassador || iDefIndex == Spy_m_FestiveAmbassador);
			return (bIsAmbassador ? HITBOX_HEAD : HITBOX_BODY);
		}
		else
			return HITBOX_BODY;
	}
	}

	return HITBOX_HEAD;
}

void CAimbot_Hitscan::Aim(C_TFPlayer* pLocal, CUserCmd* cmd, QAngle& vAngle)
{
	vAngle -= pLocal->m_vecPunchAngle();
	ClampAngles(vAngle);

	int nAimMethod = (Vars::Aimbot::Hitscan::SpectatedSmooth.m_Var && g_Globals.m_bIsBeingSpectated) ? 1 : Vars::Aimbot::Hitscan::AimMethod.m_Var;

	switch (nAimMethod)
	{
	case 0: //Plain
	{
		cmd->viewangles = vAngle;
		I::EngineClient->SetViewAngles(cmd->viewangles);
		break;
	}

	case 1: //Smooth
	{
		//Grab the viewangles from the engine
		QAngle va; I::EngineClient->GetViewAngles(va);

		//Calculate delta of current viewangles and wanted angles
		QAngle angDelta = vAngle - va;

		//Clamp, keep the angle in possible bounds
		ClampAngles(angDelta);

		//Basic smooth by dividing the delta by wanted smooth amount
		cmd->viewangles += angDelta / Vars::Aimbot::Hitscan::SmoothingAmount.m_Var;

		//Set the viewangles from engine
		I::EngineClient->SetViewAngles(cmd->viewangles);
		break;
	}

	case 2: //Silent
	{
		G::Util.FixMovement(vAngle, cmd);
		cmd->viewangles = vAngle;
		break;
	}

	default: break;
	}
}

const ESortMethod CAimbot_Hitscan::GetSortMethod()
{
	switch (Vars::Aimbot::Hitscan::SortMethod.m_Var)
	{
	case 0: return ESortMethod::FOV;
	case 1: return ESortMethod::DISTANCE;
	default: return ESortMethod::UNKNOWN;
	}
}

bool CAimbot_Hitscan::ShouldFire(C_TFPlayer* pLocal, C_TFWeaponBase* pWeapon, CUserCmd* cmd, const Target_t& Target)
{
	if (!Vars::Aimbot::Global::AutoShoot.m_Var)
		return false;

	C_TFSniperRifle* pRifle = pWeapon->As<C_TFSniperRifle*>();

	if (pRifle)
	{
		const bool bIsZoomed = pLocal->IsZoomed();

		if (pRifle->MustBeZoomedToFire() && !bIsZoomed)
			return false;

		const int iRifleType = pRifle->GetRifleType();

		if (Vars::Aimbot::Hitscan::WaitForHeadshot.m_Var)
		{
			if (iRifleType != RifleTypes_t::RIFLE_CLASSIC
				&& iRifleType != RifleTypes_t::RIFLE_JARATE
				&& !pRifle->CanHeadShot() && bIsZoomed)
				return false;
		}

		if (Vars::Aimbot::Hitscan::WaitForCharge.m_Var && bIsZoomed)
		{
			// TODO: Look at what server does to calculate dmg.

			const int nHealth = Target.m_pEntity->GetHealth();
			const int bIsCritBoosted = pLocal->IsCritBoosted();

			if (Target.m_nAimedHitbox == HITBOX_HEAD && iRifleType != RifleTypes_t::RIFLE_JARATE)
			{
				if (nHealth > 150)
				{
					const float flDamage = RemapValClamped(pRifle->m_flChargedDamage(), 0.0f, 150.0f, 0.0f, 450.0f);
					const int nDamage = static_cast<int>(flDamage);

					if (nDamage < nHealth && nDamage != 450)
						return false;
				}
				else
				{
					if (!bIsCritBoosted && !pRifle->CanHeadShot())
						return false;
				}
			}
			else
			{
				if (nHealth > (bIsCritBoosted ? 150 : 50))
				{
					float flMult = (Target.m_TargetType == ETargetType::PLAYER && Target.m_pEntity->As<C_TFPlayer*>()->InCond(TF_COND_URINE)) ? 1.36f : 1.0f;

					if (bIsCritBoosted)
						flMult = 3.0f;

					const float flMax = 150.0f * flMult;
					const int nDamage = static_cast<int>(pRifle->m_flChargedDamage() * flMult);

					if (nDamage < Target.m_pEntity->GetHealth() && nDamage != static_cast<int>(flMax))
						return false;
				}
			}

		}
	}

	if (pLocal->IsClass(TF_CLASS_SPY))
	{
		if (Vars::Aimbot::Hitscan::WaitForHeadshot.m_Var && !pWeapon->CanHeadShot())
		{
			const int iDefIndex = pWeapon->m_iItemDefinitionIndex();

			if (iDefIndex == Spy_m_TheAmbassador || iDefIndex == Spy_m_FestiveAmbassador)
				return false;
		}
	}

	if ((Vars::Aimbot::Hitscan::SpectatedSmooth.m_Var && g_Globals.m_bIsBeingSpectated) || (Vars::Aimbot::Hitscan::AimMethod.m_Var == 1))
	{
		Vector vecForward;
		AngleVectors(cmd->viewangles, &vecForward);
		Vector vecStart = pLocal->Weapon_ShootPosition();
		Vector vecEnd = (vecStart + (vecForward * 8192.f));

		trace_t tr;
		CTraceFilterSimple filter(pLocal, COLLISION_GROUP_NONE);
		UTIL_TraceLine(vecStart, vecEnd, (MASK_SHOT | CONTENTS_GRATE), &filter, &tr);

		if (tr.m_pEnt != Target.m_pEntity)
			return false;
	}

	return true;
}

bool CAimbot_Hitscan::GetTargets(C_TFPlayer* pLocal, C_TFWeaponBase* pWeapon, CUserCmd* cmd)
{
	const ESortMethod SortMethod = GetSortMethod();

	A::Global.m_vecTargets.RemoveAll();

	Vector vShootPos = pLocal->Weapon_ShootPosition();
	QAngle va; I::EngineClient->GetViewAngles(va);

	if (Vars::Aimbot::Global::Targets[0])
	{
		const int iBestHitbox = GetBestHitbox(pLocal, pWeapon);

		for (auto pEntity : G::EntityCache.GetGroup(EEntGroup::PLAYERS_ENEMIES))
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

			F::AntiHack.ResolvePlayer(pPlayer, cmd);

			Vector vPosition; pPlayer->GetHitboxPosition(iBestHitbox, vPosition);
			QAngle vAngleTo = GetAngleToPosition(vShootPos, vPosition);
			float flFOVTo = GetFovToPosition(va, vAngleTo);
			float flDistTo = vShootPos.DistTo(vPosition);

			if ((SortMethod == ESortMethod::FOV) && flFOVTo > Vars::Aimbot::Hitscan::AimFOV.m_Var)
				continue;

			A::Global.m_vecTargets.AddToTail({ pPlayer, ETargetType::PLAYER, vPosition, vAngleTo, flFOVTo, flDistTo, iBestHitbox });
		}
	}

	if (Vars::Aimbot::Global::Targets[1])
	{
		for (auto pEntity : G::EntityCache.GetGroup(EEntGroup::BUILDINGS_ENEMIES))
		{
			C_BaseObject* pBuilding = pEntity->As<C_BaseObject*>();

			if (!pBuilding->IsAlive())
				continue;

			Vector vPosition = pBuilding->WorldSpaceCenter();
			QAngle vAngleTo = GetAngleToPosition(vShootPos, vPosition);
			float flFOVTo = GetFovToPosition(va, vAngleTo);
			float flDistTo = vShootPos.DistTo(vPosition);

			if (SortMethod == ESortMethod::FOV && flFOVTo > Vars::Aimbot::Hitscan::AimFOV.m_Var)
				continue;

			A::Global.m_vecTargets.AddToTail({ pBuilding, ETargetType::BUILDING, vPosition, vAngleTo, flFOVTo, flDistTo });
		}
	}

	return !A::Global.m_vecTargets.IsEmpty();
}

bool CAimbot_Hitscan::GetTarget(C_TFPlayer* pLocal, C_TFWeaponBase* pWeapon, CUserCmd* cmd, Target_t& Out)
{
	if (!GetTargets(pLocal, pWeapon, cmd))
		return false;

	A::Global.SortTargets(GetSortMethod());

	Vector vecSrc = pLocal->Weapon_ShootPosition();

	for (Target_t& Target : A::Global.m_vecTargets)
	{
		if (!G::Util.IsVisible(vecSrc, Target.m_vPos))
		{
			if (Target.m_TargetType == ETargetType::PLAYER)
			{
				C_TFPlayer* pPlayer = Target.m_pEntity->As<C_TFPlayer*>();

				if (!pPlayer)
					continue;

				Vector vNewPos;
				bool bFoundNew = false;

				for (int n = 0; n < HITBOX_MAX; n++)
				{
					if (!Vars::Aimbot::Hitscan::ScanHitboxes[n])
						continue;

					if (pPlayer->GetHitboxPosition(n, vNewPos) && G::Util.IsVisible(vecSrc, vNewPos))
					{
						Target.m_vPos = vNewPos;
						Target.m_vAngleTo = GetAngleToPosition(vecSrc, vNewPos);

						bFoundNew = true;
						break;
					}
				}

				if (!bFoundNew)
					continue;
			}
			else
				continue;
		}

		Out = Target;
		return true;
	}

	return false;
}
