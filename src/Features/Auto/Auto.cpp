#include "Auto.h"
#include "../Backtrack/Backtrack.h"
#include "../Vars.h"

void CFeatures_Auto::Run(C_TFPlayer* pLocal, C_TFWeaponBase* pWeapon, CUserCmd* cmd)
{
	if (Vars::Auto::AutoBackstab.m_Var)
		AutoBackstab(pLocal, pWeapon, cmd);

	//AutoHeal(pLocal, pWeapon, cmd);
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
		for (auto pEntity : G::EntityCache.GetGroup(EEntGroup::PLAYERS_ENEMIES))
		{
			C_TFPlayer* pTarget = pEntity->As<C_TFPlayer*>();

			Vector vBody; pTarget->GetHitboxPosition(HITBOX_BODY, vBody);
			QAngle vAngleTo = GetAngleToPosition(pLocal->Weapon_ShootPosition(), vBody);

			trace_t trace;
			if (pKnife->DoSwingTraceKnife(trace, vAngleTo) && pKnife->CanAttack())
			{
				if (trace.m_pEnt && trace.m_pEnt == pTarget)
				{
					if (pKnife->CanPerformBackstabAgainstTarget(pTarget, vAngleTo))
					{			
						//G::Util.FixMovement(vAngleTo, cmd);
						//cmd->viewangles = vAngleTo;
						//cmd->buttons |= IN_ATTACK;
						//return;
					}
				}
			}

			float correct = 0.0f;

			INetChannelInfo* nci = reinterpret_cast<INetChannelInfo*>(I::EngineClient->GetNetChannelInfo());

			if (nci)
			{
				correct += nci->GetLatency(FLOW_INCOMING);
				correct += nci->GetLatency(FLOW_OUTGOING);
			}

			int lerpTicks = TIME_TO_TICKS(G::Util.GetClientInterpAmount());

			correct += TICKS_TO_TIME(lerpTicks);

			correct = clamp(correct, 0.0f, G::ConVars.sv_maxunlag->GetFloat());

			// TEST AMOUNT!!!!!! Please dont bully me :)
			int wowhowmuchshouldweadd = TIME_TO_TICKS(0.04f);

			int targettick = (cmd->tick_count - lerpTicks)+wowhowmuchshouldweadd;

			F::Backtrack.BacktrackPlayer(pTarget, TICKS_TO_TIME(targettick));

			pTarget->GetHitboxPosition(HITBOX_BODY, vBody);
			vAngleTo = GetAngleToPosition(pLocal->Weapon_ShootPosition(), vBody);

			if (pKnife->DoSwingTraceKnife(trace, vAngleTo) && pKnife->CanAttack())
			{
				if (trace.m_pEnt && trace.m_pEnt == pTarget)
				{
					if (pKnife->CanPerformBackstabAgainstTarget(pTarget, vAngleTo))
					{
						G::Util.FixMovement(vAngleTo, cmd);
						cmd->viewangles = vAngleTo;
						cmd->buttons |= IN_ATTACK;
						cmd->tick_count = targettick + lerpTicks;
					}
				}
			}

			F::Backtrack.RestorePlayer(pTarget);
		}
	}
}

void CFeatures_Auto::AutoHeal(C_TFPlayer* pLocal, C_TFWeaponBase* pWeapon, CUserCmd* cmd)
{
	C_WeaponMedigun* pMedigun = pWeapon->As<C_WeaponMedigun*>();

	if (!pMedigun)
		return;

	// Basic checks if you do not have a C_WeaponMedigun class
	//if (pLocal->deadflag() || !pLocal->IsClass(TF_CLASS_MEDIC))
	//	return;

	// We are going to essentially use this function like a giant for loop. Every time CreateMove is called we will increase n by one.
	static int n = 1; 

	// If we have reached the maximum number of clients on the server, reset count and keep going
	if (n < (g_Globals.m_nMaxClients + 1))
		n = 1;

	// We only want to heal a different person every other tick, or else it will stay healing the same person.
	if (cmd->tick_count % 2)
		return;

	C_TFPlayer* pPlayer = UTIL_TFPlayerByIndex(n);

	// Any problems, increment and return
	if (!pPlayer || pPlayer->IsDormant() || pPlayer->deadflag() || !pPlayer->InLocalTeam())
		n++; return;
}
