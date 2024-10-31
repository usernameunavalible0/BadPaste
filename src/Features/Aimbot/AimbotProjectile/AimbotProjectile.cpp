#include "AimbotProjectile.h"
#include "MovementSimulation/MovementSimulation.h"
#include "../../Vars.h"

Vector CAimbot_Projectile::Predictor_t::Extrapolate(float time)
{
    Vector vecOut = Vector();

    if (!m_pEntity->IsPlayer() || (m_pEntity->IsPlayer() && m_pEntity->As<C_BasePlayer*>()->m_fFlags().IsFlagSet(FL_ONGROUND)))
        vecOut = (m_vPosition + (m_vVelocity * time));

    else vecOut = (m_vPosition + (m_vVelocity * time) - m_vAcceleration * 0.5f * time * time);

    return vecOut;
}

bool CAimbot_Projectile::GetProjectileInfo(C_TFWeaponBase* pWeapon, ProjectileInfo_t& out)
{
	switch (pWeapon->m_iItemDefinitionIndex())
	{
	case Soldier_m_RocketLauncher:
	case Soldier_m_RocketLauncherR:
	case Soldier_m_TheBlackBox:
	case Soldier_m_TheCowMangler5000:
	case Soldier_m_TheOriginal:
	case Soldier_m_FestiveRocketLauncher:
	case Soldier_m_TheBeggarsBazooka:
	case Soldier_m_SilverBotkillerRocketLauncherMkI:
	case Soldier_m_GoldBotkillerRocketLauncherMkI:
	case Soldier_m_RustBotkillerRocketLauncherMkI:
	case Soldier_m_BloodBotkillerRocketLauncherMkI:
	case Soldier_m_CarbonadoBotkillerRocketLauncherMkI:
	case Soldier_m_DiamondBotkillerRocketLauncherMkI:
	case Soldier_m_SilverBotkillerRocketLauncherMkII:
	case Soldier_m_GoldBotkillerRocketLauncherMkII:
	case Soldier_m_FestiveBlackBox:
	case Soldier_m_TheAirStrike:
	case Soldier_m_WoodlandWarrior:
	case Soldier_m_SandCannon:
	case Soldier_m_AmericanPastoral:
	case Soldier_m_SmalltownBringdown:
	case Soldier_m_ShellShocker:
	case Soldier_m_AquaMarine:
	case Soldier_m_Autumn:
	case Soldier_m_BlueMew:
	case Soldier_m_BrainCandy:
	case Soldier_m_CoffinNail:
	case Soldier_m_HighRollers:
	case Soldier_m_Warhawk: {
		out = { 1100.0f, 0.0f };
		break;
	}

	case Soldier_m_TheDirectHit: {
		out = { 1980.0f, 0.0f };
		break;
	}

	case Soldier_m_TheLibertyLauncher: {
		out = { 1540.0f, 0.0f };
		break;
	}

	case Demoman_m_GrenadeLauncher:
	case Demoman_m_GrenadeLauncherR:
	case Demoman_m_FestiveGrenadeLauncher:
	case Demoman_m_TheIronBomber:
	case Demoman_m_Autumn:
	case Demoman_m_MacabreWeb:
	case Demoman_m_Rainbow:
	case Demoman_m_SweetDreams:
	case Demoman_m_CoffinNail:
	case Demoman_m_TopShelf:
	case Demoman_m_Warhawk:
	case Demoman_m_ButcherBird: {
		out = { 1200.0f, 0.4f };
		break;
	}

	case Soldier_s_TheRighteousBison:
	case Engi_m_ThePomson6000: {
		out = { 1200.0f, 0.0f };
		break;
	}

	case Demoman_m_TheLooseCannon: {
		out = { 1453.9f, 0.4f };
		break;
	}

	case Demoman_m_TheLochnLoad: {
		out = { 1513.3f, 0.4f };
		break;
	}

	case Engi_m_TheRescueRanger:
	case Medic_m_FestiveCrusadersCrossbow:
	case Medic_m_CrusadersCrossbow: {
		out = { 2400.0f, 0.2f };
		break;
	}

	case Pyro_m_DragonsFury: {
		out = { 3000.0f, 0.0f, 0.12f };
		break;
	}

	case Pyro_m_FlameThrower:
	case Pyro_m_FlameThrowerR:
	case Pyro_m_TheBackburner:
	case Pyro_m_TheDegreaser:
	case Pyro_m_ThePhlogistinator:
	case Pyro_m_FestiveFlameThrower:
	case Pyro_m_TheRainblower:
	case Pyro_m_SilverBotkillerFlameThrowerMkI:
	case Pyro_m_GoldBotkillerFlameThrowerMkI:
	case Pyro_m_RustBotkillerFlameThrowerMkI:
	case Pyro_m_BloodBotkillerFlameThrowerMkI:
	case Pyro_m_CarbonadoBotkillerFlameThrowerMkI:
	case Pyro_m_DiamondBotkillerFlameThrowerMkI:
	case Pyro_m_SilverBotkillerFlameThrowerMkII:
	case Pyro_m_GoldBotkillerFlameThrowerMkII:
	case Pyro_m_FestiveBackburner:
	case Pyro_m_ForestFire:
	case Pyro_m_BarnBurner:
	case Pyro_m_BovineBlazemaker:
	case Pyro_m_EarthSkyandFire:
	case Pyro_m_FlashFryer:
	case Pyro_m_TurbineTorcher:
	case Pyro_m_Autumn:
	case Pyro_m_PumpkinPatch:
	case Pyro_m_Nutcracker:
	case Pyro_m_Balloonicorn:
	case Pyro_m_Rainbow:
	case Pyro_m_CoffinNail:
	case Pyro_m_Warhawk:
	case Pyro_m_NostromoNapalmer: {
		out = { 1000.0f, 0.0f, 0.33f };
		m_bIsFlameThrower = true;
		break;
	}

	case Pyro_s_TheDetonator:
	case Pyro_s_TheFlareGun:
	case Pyro_s_FestiveFlareGun:
	case Pyro_s_TheScorchShot: {
		out = { 2000.0f, 0.3f };
		break;
	}

	case Pyro_s_TheManmelter: {
		out = { 3000.0f, 0.2f };
		break;
	}

	case Medic_m_SyringeGun:
	case Medic_m_SyringeGunR:
	case Medic_m_TheBlutsauger:
	case Medic_m_TheOverdose: {
		out = { 1000.0f, 0.2f };
		break;
	}

	case Sniper_m_TheHuntsman:
	case Sniper_m_FestiveHuntsman:
	case Sniper_m_TheFortifiedCompound: {
		float charge = (I::GlobalVars->curtime - pWeapon->As<C_TFCompoundBow*>()->m_flChargeBeginTime());
		out = { ((fminf(fmaxf(charge, 0.0f), 1.0f) * 800.0f) + 1800.0f), ((fminf(fmaxf(charge, 0.0f), 1.0f) * -0.4f) + 0.5f) };
		break;
	}
	}

	return out.m_flVelocity;
}

bool CAimbot_Projectile::CalcProjAngle(const Vector& vLocalPos, const Vector& vTargetPos, const ProjectileInfo_t& ProjInfo, Solution_t& out)
{
	const Vector v = vTargetPos - vLocalPos;
	const float dx = sqrt(v.x * v.x + v.y * v.y);
	const float dy = v.z;
	const float v0 = ProjInfo.m_flVelocity;

	//Ballistics
	if (const float g = G::ConVars.sv_gravity->GetFloat() * ProjInfo.m_flGravity)
	{
		const float root = v0 * v0 * v0 * v0 - g * (g * dx * dx + 2.0f * dy * v0 * v0);

		if (root < 0.0f)
			return false;

		out.m_flPitch = atan((v0 * v0 - sqrt(root)) / (g * dx));
		out.m_flYaw = atan2(v.y, v.x);
	}

	//Straight trajectory (the time stuff later doesn't make sense with this but hey it works!)
	else
	{
		QAngle vecAngle = GetAngleToPosition(vLocalPos, vTargetPos);
		out.m_flPitch = -DEG2RAD(vecAngle.x);
		out.m_flYaw = DEG2RAD(vecAngle.y);
	}

	out.m_flTime = dx / (cos(out.m_flPitch) * v0);

	return true;
}

static void GetProjectileFireSetup(C_TFPlayer* pPlayer, const QAngle& vViewAngles, Vector vOffset, Vector* vSrc)
{
	if (G::ConVars.cl_flipviewmodels->GetInt())
		vOffset.y *= -1.0f;

	Vector vecForward = Vector(), vecRight = Vector(), vecUp = Vector();
	AngleVectors(vViewAngles, &vecForward, &vecRight, &vecUp);

	*vSrc = pPlayer->Weapon_ShootPosition() + (vecForward * vOffset.x) + (vecRight * vOffset.y) + (vecUp * vOffset.z);
}

bool CAimbot_Projectile::SolveProjectile(C_TFPlayer* pLocal, C_TFWeaponBase* pWeapon, CUserCmd* cmd, Predictor_t& Predictor, const ProjectileInfo_t& ProjInfo, Solution_t& out)
{
	INetChannelInfo* pNetChannel = I::EngineClient->GetNetChannelInfo();

	if (!pNetChannel)
		return false;

	trace_t tr;
	CTraceFilterWorldAndPropsOnly filter;

	Vector vecSrc = pLocal->Weapon_ShootPosition();

	const float fInterp = G::ConVars.cl_interp->GetFloat();
	const float fLatency = (pNetChannel->GetLatency(FLOW_OUTGOING) + pNetChannel->GetLatency(FLOW_INCOMING));

	const float MAX_TIME = ProjInfo.m_flMaxTime;
	const float TIME_STEP = (MAX_TIME / 128.0f);

	const bool bUseExtrapolation = !Vars::Aimbot::Projectile::UseMovesim.m_Var || !Predictor.m_pEntity->IsPlayer() || (Predictor.m_pEntity->IsPlayer() && Predictor.m_pEntity->As<C_TFPlayer*>()->m_vecVelocity().IsZero());
	
	if (bUseExtrapolation)
	{
		for (float fPredTime = 0.0f; fPredTime < MAX_TIME; fPredTime += TIME_STEP)
		{
			const float fCorrectTime = (fPredTime + fInterp + fLatency);
			Vector vecPredicted = Predictor.Extrapolate(fCorrectTime);

			switch (pWeapon->GetWeaponID())
			{
			case TF_WEAPON_GRENADELAUNCHER:
			case TF_WEAPON_PIPEBOMBLAUNCHER:
			{
				Vector vecDelta = (vecPredicted - vecSrc);
				float fRange = VectorNormalize(vecDelta);

				float fElevationAngle = (fRange * (pWeapon->m_iItemDefinitionIndex() == Demoman_m_TheLochnLoad ? 0.0075f : 0.013f));

				if (fElevationAngle > 45.0f)
					fElevationAngle = 45.0f;

				float s = 0.0f, c = 0.0f;
				SinCos((fElevationAngle * M_PI / 180.0f), &s, &c);

				float fElevation = (fRange * (s / c));
				vecPredicted.z += (c > 0.0f ? fElevation : 0.0f);
				break;
			}

			default: break;
			}

			//G::Util.TraceHull(Predictor.m_vPosition, vPredictedPos, Vector(-2, -2, -2), Vector(2, 2, 2), MASK_SOLID_BRUSHONLY, &TraceFilter, &Trace);
			UTIL_TraceHull(Predictor.m_vPosition, vecPredicted, Vector(-2, -2, -2), Vector(2, 2, 2), MASK_SOLID_BRUSHONLY, &filter, &tr);

			if (tr.DidHit())
				vecPredicted.z = tr.endpos.z;

			switch (pWeapon->GetWeaponID())
			{
			case TF_WEAPON_GRENADELAUNCHER:
			case TF_WEAPON_PIPEBOMBLAUNCHER:
			{
				Vector vecOffset(16.0f, 8.0f, -6.0f);
				GetProjectileFireSetup(pLocal, cmd->viewangles, vecOffset, &vecSrc);
				break;
			}

			default: break;
			}

			if (!CalcProjAngle(vecSrc, vecPredicted, ProjInfo, out))
				return false;

			if (out.m_flTime < fCorrectTime)
			{
				Vector vecVis = vecSrc;

				switch (pWeapon->GetWeaponID())
				{
				case TF_WEAPON_ROCKETLAUNCHER:
				case TF_WEAPON_ROCKETLAUNCHER_DIRECTHIT:
				case TF_WEAPON_FLAREGUN:
				case TF_WEAPON_FLAREGUN_REVENGE:
				case TF_WEAPON_COMPOUND_BOW:
				case TF_WEAPON_SYRINGEGUN_MEDIC:
				{
					if (pWeapon->m_iItemDefinitionIndex() != Soldier_m_TheOriginal)
					{
						Vector vecOffset(23.5f, 12.0f, -3.0f);

						if (pLocal->IsDucking())
							vecOffset.z = 8.0f;

						GetProjectileFireSetup(pLocal, cmd->viewangles, vecOffset, &vecVis);
					}

					break;
				}

				case TF_WEAPON_GRENADELAUNCHER:
				case TF_WEAPON_PIPEBOMBLAUNCHER:
				{
					QAngle vecAngle = QAngle(); Vector vecForward = Vector(), vecRight = Vector(), vecUp = Vector();
					AngleVectors({ -RAD2DEG(out.m_flPitch), RAD2DEG(out.m_flYaw), 0.0f }, &vecForward, &vecRight, &vecUp);
					Vector vecVelocity = ((vecForward * ProjInfo.m_flVelocity) - (vecUp * 200.0f));
					VectorAngles(vecVelocity, vecAngle);
					out.m_flPitch = -DEG2RAD(vecAngle.x);

					break;
				}

				default: break;
				}

				UTIL_TraceHull(vecVis, vecPredicted, Vector(-2, -2, -2), Vector(2, 2, 2), MASK_SOLID_BRUSHONLY, &filter, &tr);

				if (tr.DidHit())
					return false;

				//g_GlobalInfo.m_vPredictedPos = vPredictedPos;
				return true;
			}
		}
	}
	else
	{
		C_TFPlayer* pPlayer = Predictor.m_pEntity->As<C_TFPlayer*>();

		/*Vector vecPredictedOffset = Vector(0, 0, 0);
		switch (Vars::Aimbot::Projectile::AimPosition.m_Var)
		{
		case 0:
		{
			Vector vecBody; pPlayer->GetHitboxPosition(HITBOX_BODY, vecBody);
			vecPredictedOffset = vecBody - pPlayer->m_vecOrigin();
			break;
		}
		case 1:
		{
			Vector vecFootL, vecFootR;
			pPlayer->GetHitboxPosition(HITBOX_LEFT_FOOT, vecFootL);
			pPlayer->GetHitboxPosition(HITBOX_RIGHT_FOOT, vecFootR);
			const Vector vecDelta = (vecFootL + vecFootR) / 2.f;
			vecPredictedOffset = vecDelta - pPlayer->m_vecOrigin();
			break;
		}
		case 2:
		{
			switch (pLocal->m_iClass())
			{
			case TF_CLASS_SOLDIER:
			case TF_CLASS_DEMOMAN:
			{
				Vector vecFootL, vecFootR;
				pPlayer->GetHitboxPosition(HITBOX_LEFT_FOOT, vecFootL);
				pPlayer->GetHitboxPosition(HITBOX_RIGHT_FOOT, vecFootR);
				const Vector vecDelta = (vecFootL + vecFootR) / 2.f;
				vecPredictedOffset = vecDelta - pPlayer->m_vecOrigin();
				break;
			}
			case TF_CLASS_SNIPER:
			{
				Vector vecHead; pPlayer->GetHitboxPosition(HITBOX_HEAD, vecHead);
				vecPredictedOffset = vecHead - pPlayer->m_vecOrigin();
				break;
			}
			default:
			{
				Vector vecBody; pPlayer->GetHitboxPosition(HITBOX_BODY, vecBody);
				vecPredictedOffset = vecBody - pPlayer->m_vecOrigin();
				break;
			}
			}

			break;
		}
		default: break;
		}*/

		if (!A::MovementSimulation.Initialize(pPlayer))
			return false;

		for (int nTick = 0; nTick < TIME_TO_TICKS(MAX_TIME); nTick++)
		{
			A::MovementSimulation.RunTick();

			Vector vecPredicted = A::MovementSimulation.GetOrigin();

			switch (Vars::Aimbot::Projectile::AimPosition.m_Var)
			{
			case 0:
			{
				Vector vecBody; pPlayer->GetHitboxPosition(HITBOX_BODY, vecBody);
				vecPredicted.z += vecBody.z - pPlayer->m_vecOrigin().z;
				break;
			}
			case 1:
			{
				Vector vecFootL, vecFootR;
				pPlayer->GetHitboxPosition(HITBOX_LEFT_FOOT, vecFootL);
				pPlayer->GetHitboxPosition(HITBOX_RIGHT_FOOT, vecFootR);
				const Vector vecDelta = (vecFootL + vecFootR) / 2.f;
				vecPredicted.z += vecDelta.z - pPlayer->m_vecOrigin().z;
				break;
			}
			case 2:
			{
				switch (pLocal->m_iClass())
				{
				case TF_CLASS_SOLDIER:
				case TF_CLASS_DEMOMAN:
				{
					Vector vecFootL, vecFootR;
					pPlayer->GetHitboxPosition(HITBOX_LEFT_FOOT, vecFootL);
					pPlayer->GetHitboxPosition(HITBOX_RIGHT_FOOT, vecFootR);
					const Vector vecDelta = (vecFootL + vecFootR) / 2.f;
					vecPredicted.z += vecDelta.z - pPlayer->m_vecOrigin().z;
					break;
				}
				case TF_CLASS_SNIPER:
				{
					Vector vecHead; pPlayer->GetHitboxPosition(HITBOX_HEAD, vecHead);
					vecPredicted.z += vecHead.z - pPlayer->m_vecOrigin().z;
					break;
				}
				default:
				{
					Vector vecBody; pPlayer->GetHitboxPosition(HITBOX_BODY, vecBody);
					vecPredicted.z += vecBody.z - pPlayer->m_vecOrigin().z;
					break;
				}
				}

				break;
			}
			default: break;
			}

			switch (pWeapon->GetWeaponID())
			{
			case TF_WEAPON_GRENADELAUNCHER:
			case TF_WEAPON_PIPEBOMBLAUNCHER:
			{
				Vector vecDelta = (vecPredicted - vecSrc);
				float fRange = VectorNormalize(vecDelta);

				float fElevationAngle = (fRange * (pWeapon->m_iItemDefinitionIndex() == Demoman_m_TheLochnLoad ? 0.0075f : 0.013f));

				if (fElevationAngle > 45.0f)
					fElevationAngle = 45.0f;

				float s = 0.0f, c = 0.0f;
				SinCos((fElevationAngle * M_PI / 180.0f), &s, &c);

				float fElevation = (fRange * (s / c));
				vecPredicted.z += (c > 0.0f ? fElevation : 0.0f);
				break;
			}

			default: break;
			}

			switch (pWeapon->GetWeaponID())
			{
			case TF_WEAPON_GRENADELAUNCHER:
			case TF_WEAPON_PIPEBOMBLAUNCHER:
			{
				Vector vecOffset(16.0f, 8.0f, -6.0f);
				GetProjectileFireSetup(pLocal, cmd->viewangles, vecOffset, &vecPredicted);
				break;
			}

			default: break;
			}

			if (!CalcProjAngle(vecSrc, vecPredicted, ProjInfo, out))
				continue;

			const int nTargetTick = TIME_TO_TICKS(out.m_flTime + fLatency + fInterp);

			if ((nTargetTick == nTick || nTargetTick == nTick - 1))
			{
				Vector vecVis = vecSrc;

				switch (pWeapon->GetWeaponID())
				{
				case TF_WEAPON_ROCKETLAUNCHER:
				case TF_WEAPON_ROCKETLAUNCHER_DIRECTHIT:
				case TF_WEAPON_FLAREGUN:
				case TF_WEAPON_FLAREGUN_REVENGE:
				case TF_WEAPON_COMPOUND_BOW:
				case TF_WEAPON_SYRINGEGUN_MEDIC:
				{
					if (pWeapon->m_iItemDefinitionIndex() != Soldier_m_TheOriginal)
					{
						Vector vecOffset(23.5f, 12.0f, -3.0f);

						if (pLocal->IsDucking())
							vecOffset.z = 8.0f;

						GetProjectileFireSetup(pLocal, cmd->viewangles, vecOffset, &vecVis);
					}

					break;
				}

				case TF_WEAPON_GRENADELAUNCHER:
				case TF_WEAPON_PIPEBOMBLAUNCHER:
				{
					QAngle vecAngle = QAngle(); Vector vecForward = Vector(), vecRight = Vector(), vecUp = Vector();
					AngleVectors({ -RAD2DEG(out.m_flPitch), RAD2DEG(out.m_flYaw), 0.0f }, &vecForward, &vecRight, &vecUp);
					Vector vecVelocity = ((vecForward * ProjInfo.m_flVelocity) - (vecUp * 200.0f));
					VectorAngles(vecVelocity, vecAngle);
					out.m_flPitch = -DEG2RAD(vecAngle.x);

					break;
				}

				default: break;
				}

				UTIL_TraceHull(vecVis, vecPredicted, Vector(-2, -2, -2), Vector(2, 2, 2), MASK_SOLID_BRUSHONLY, &filter, &tr);

				if (tr.DidHit()) { A::MovementSimulation.Restore(); return false; }

				//g_GlobalInfo.m_vPredictedPos = vPredictedPos;
				A::MovementSimulation.Restore();
				return true;
			}
		}

		A::MovementSimulation.Restore();
	}

	return false;
}

Vector CAimbot_Projectile::GetAimPos(C_TFPlayer* pLocal, C_BaseEntity* pEntity)
{
	switch (Vars::Aimbot::Projectile::AimPosition.m_Var)
	{
	case 0: return pEntity->WorldSpaceCenter();
	case 1: return pEntity->WorldSpaceCenter() - Vector(0.0f, 0.0f, 27.0f);
	case 2:
	{
		switch (pLocal->m_iClass())
		{
		case TF_CLASS_SOLDIER: return pEntity->WorldSpaceCenter() - Vector(0.0f, 0.0f, 27.0f);

		case TF_CLASS_SNIPER:
		{
			// TODO: FUCK GAYBE AND HIS MUSIC TASTE AND THIS CAST TO C_BASEANIMATING
			Vector vPos; pEntity->As<C_BaseAnimating*>()->GetHitboxPosition(HITBOX_HEAD, vPos);

			Vector vEntForward = Vector();
			AngleVectors(pEntity->EyeAngles(), &vEntForward);
			Vector vToEnt = pEntity->GetAbsOrigin() - pLocal->GetAbsOrigin();
			vToEnt.NormalizeInPlace();

			if (vToEnt.Dot(vEntForward) > 0.1071f)
				vPos.z += 5.0f;

			return vPos;
		}

		default: return pEntity->WorldSpaceCenter();
		}
	}
	default: return Vector();
	}
}

ESortMethod CAimbot_Projectile::GetSortMethod()
{
	switch (Vars::Aimbot::Projectile::SortMethod.m_Var) {
	case 0: return ESortMethod::FOV;
	case 1: return ESortMethod::DISTANCE;
	default: return ESortMethod::UNKNOWN;
	}
}

bool CAimbot_Projectile::GetTargets(C_TFPlayer* pLocal, C_TFWeaponBase* pWeapon)
{
	const ESortMethod SortMethod = GetSortMethod();

	A::Global.m_vecTargets.Purge();

	const Vector vecSrc = pLocal->Weapon_ShootPosition();
	QAngle va; I::EngineClient->GetViewAngles(va);

	if (Vars::Aimbot::Global::Targets[0])
	{
		const bool bIsCrossbow = pWeapon->GetWeaponID() == TF_WEAPON_CROSSBOW;

		for (auto pEntity : G::EntityCache.GetGroup(bIsCrossbow ? EEntGroup::PLAYERS_ALL : EEntGroup::PLAYERS_ENEMIES))
		{
			C_TFPlayer* pPlayer = pEntity->As<C_TFPlayer*>();

			if (pPlayer->deadflag() || pPlayer->IsHalloweenGhostMode() || pPlayer == pLocal)
				continue;

			if (pPlayer->m_iTeamNum() != pLocal->m_iTeamNum())
			{
				if (Vars::Aimbot::Global::Ignores[0] && pPlayer->IsInvulnerable())
					continue;

				if (Vars::Aimbot::Global::Ignores[1] && pPlayer->IsCloaked())
					continue;

				if (Vars::Aimbot::Global::Ignores[2] && pPlayer->IsTaunting())
					continue;

				if (Vars::Aimbot::Global::Ignores[3] && pPlayer->IsPlayerOnSteamFriendsList())
					continue;
			}

			Vector vecPos = GetAimPos(pLocal, pPlayer);
			QAngle vecAngleTo = GetAngleToPosition(vecSrc, vecPos);
			float flFOVTo = GetFovToPosition(va, vecAngleTo);
			float flDistTo = vecSrc.DistTo(vecPos);

			if (SortMethod == ESortMethod::FOV && flFOVTo > Vars::Aimbot::Projectile::AimFOV.m_Var)
				continue;

			A::Global.m_vecTargets.AddToTail({ pPlayer, ETargetType::PLAYER, vecPos, vecAngleTo, flFOVTo, flDistTo });
		}
	}

	if (Vars::Aimbot::Global::Targets[1])
	{
		const bool bIsRescueRanger = pWeapon->GetWeaponID() == TF_WEAPON_SHOTGUN_BUILDING_RESCUE;

		for (auto pEntity : G::EntityCache.GetGroup(bIsRescueRanger ? EEntGroup::BUILDINGS_ALL : EEntGroup::BUILDINGS_ENEMIES))
		{
			C_BaseObject* pBuilding = pEntity->As<C_BaseObject*>();

			if (!pBuilding->IsAlive())
				continue;

			Vector vecPos = pBuilding->WorldSpaceCenter();
			QAngle vecAngleTo = GetAngleToPosition(vecSrc, vecPos);
			float flFOVTo = GetFovToPosition(va, vecAngleTo);
			float flDistTo = vecSrc.DistTo(vecPos);

			if (SortMethod == ESortMethod::FOV && flFOVTo > Vars::Aimbot::Projectile::AimFOV.m_Var)
				continue;

			A::Global.m_vecTargets.AddToTail({ pBuilding, ETargetType::BUILDING, vecPos, vecAngleTo, flFOVTo, flDistTo });
		}
	}

	return !A::Global.m_vecTargets.IsEmpty();
}

bool CAimbot_Projectile::VerifyTarget(C_TFPlayer* pLocal, C_TFWeaponBase* pWeapon, CUserCmd* cmd, Target_t& Target)
{
	ProjectileInfo_t ProjInfo = {};

	if (!GetProjectileInfo(pWeapon, ProjInfo))
		return false;

	Vector vVelocity = Vector();
	Vector vAcceleration = Vector();

	switch (Target.m_TargetType)
	{
	case ETargetType::PLAYER: {
		Target.m_pEntity->EstimateAbsVelocity(vVelocity);
		vAcceleration = Vector(0.0f, 0.0f, G::ConVars.sv_gravity->GetFloat() * ((Target.m_pEntity->IsPlayer() && Target.m_pEntity->As<C_TFPlayer*>()->InCond(TF_COND_PARACHUTE_ACTIVE)) ? 0.224f : 1.0f));
		break;
	}

	default: break;
	}

	Predictor_t Predictor = {
		Target.m_pEntity,
		Target.m_vPos,
		vVelocity,
		vAcceleration
	};

	Solution_t Solution = {};

	if (!SolveProjectile(pLocal, pWeapon, cmd, Predictor, ProjInfo, Solution))
		return false;

	Target.m_vAngleTo = { -RAD2DEG(Solution.m_flPitch), RAD2DEG(Solution.m_flYaw), 0.0f };

	return true;
}

bool CAimbot_Projectile::GetTarget(C_TFPlayer* pLocal, C_TFWeaponBase* pWeapon, CUserCmd* cmd, Target_t& Out)
{
	if (!GetTargets(pLocal, pWeapon))
		return false;

	if (Vars::Aimbot::Projectile::PerformanceMode.m_Var)
	{
		Target_t Target = A::Global.GetBestTarget(GetSortMethod());

		if (!VerifyTarget(pLocal, pWeapon, cmd, Target))
			return false;

		Out = Target;
		return true;
	}

	else
	{
		A::Global.SortTargets(GetSortMethod());

		for (Target_t& Target : A::Global.m_vecTargets)
		{
			if (!VerifyTarget(pLocal, pWeapon, cmd, Target))
				continue;

			Out = Target;
			return true;
		}
	}

	return false;
}

void CAimbot_Projectile::Aim(C_TFPlayer* pLocal, CUserCmd* cmd, C_TFWeaponBase* pWeapon, QAngle& vAngle)
{
	vAngle -= pLocal->m_vecPunchAngle();
	ClampAngles(vAngle);

	switch (Vars::Aimbot::Projectile::AimMethod.m_Var)
	{
	case 0: {
		cmd->viewangles = vAngle;
		I::EngineClient->SetViewAngles(cmd->viewangles);
		break;
	}

	case 1: {
		G::Util.FixMovement(vAngle, cmd);
		cmd->viewangles = vAngle;
		break;
	}

	default: break;
	}
}

bool CAimbot_Projectile::ShouldFire(C_TFWeaponBase* pWeapon, CUserCmd* cmd)
{
	return (Vars::Aimbot::Global::AutoShoot.m_Var && (pWeapon->CanPrimaryAttack() || pWeapon->m_iReloadMode() != 0));
}

bool CAimbot_Projectile::IsAttacking(CUserCmd* cmd, C_TFWeaponBase* pWeapon)
{
	if (pWeapon->m_iItemDefinitionIndex() == Soldier_m_TheBeggarsBazooka)
	{
		static bool bLoading = false;

		if (pWeapon->Clip1() > 0)
			bLoading = true;

		if (!(cmd->buttons & IN_ATTACK) && bLoading) {
			bLoading = false;
			return true;
		}
	}
	else
	{
		if (pWeapon->GetWeaponID() == TF_WEAPON_COMPOUND_BOW)
		{
			static bool bCharging = false;

			if (pWeapon->As<C_TFCompoundBow*>()->m_flChargeBeginTime() > 0.0f)
				bCharging = true;

			if (!(cmd->buttons & IN_ATTACK) && bCharging) {
				bCharging = false;
				return true;
			}
		}

		else
		{
			if ((cmd->buttons & IN_ATTACK) && (pWeapon->CanPrimaryAttack() || pWeapon->m_iReloadMode() != 0))
				return true;
		}
	}

	return false;
}

void CAimbot_Projectile::Run(C_TFPlayer* pLocal, C_TFWeaponBase* pWeapon, CUserCmd* cmd)
{
	m_bIsFlameThrower = false;

	if (!Vars::Aimbot::Projectile::Active.m_Var)
		return;

	if (GetSortMethod() == ESortMethod::FOV)
		g_Globals.m_flCurAimFOV = Vars::Aimbot::Projectile::AimFOV.m_Var;

	Target_t Target = {};

	bool bShouldAim = (Vars::Aimbot::Global::AimKey.m_Var == VK_LBUTTON ? (cmd->buttons & IN_ATTACK) : A::Global.IsKeyDown());

	if (GetTarget(pLocal, pWeapon, cmd, Target) && bShouldAim)
	{
		//g_GlobalInfo.m_nCurrentTargetIdx = Target.m_pEntity->GetIndex();

		//if (Vars::Aimbot::Projectile::AimMethod.m_Var == 1)
		//	g_GlobalInfo.m_vAimPos = g_GlobalInfo.m_vPredictedPos;

		if (ShouldFire(pWeapon, cmd))
		{
			cmd->buttons |= IN_ATTACK;

			if (pWeapon->m_iItemDefinitionIndex() == Soldier_m_TheBeggarsBazooka)
			{
				if (pWeapon->Clip1() > 0)
					cmd->buttons &= ~IN_ATTACK;
			}

			else
			{
				if (pWeapon->GetWeaponID() == TF_WEAPON_COMPOUND_BOW && pWeapon->As<C_TFCompoundBow*>()->m_flChargeBeginTime() > 0.0f)
					cmd->buttons &= ~IN_ATTACK;
			}
		}

		const bool bIsAttacking = IsAttacking(cmd, pWeapon);

		//if (bIsAttacking)
		//	g_Globals.m_bAttacking = true;

		if (Vars::Aimbot::Projectile::AimMethod.m_Var == 1)
		{
			if (m_bIsFlameThrower) {
				Aim(pLocal, cmd, pWeapon, Target.m_vAngleTo);
			}
			else
			{
				if (bIsAttacking) {
					Aim(pLocal, cmd, pWeapon, Target.m_vAngleTo);
					g_Globals.m_bChoking = true;
				}
			}
		}

		else Aim(pLocal, cmd, pWeapon, Target.m_vAngleTo);
	}
}
