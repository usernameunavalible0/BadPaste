#include "Auto.h"
#include "../Vars.h"

void CFeatures_Auto::Run(C_TFPlayer* pLocal, C_TFWeaponBase* pWeapon, CUserCmd* cmd)
{
	if (Vars::Auto::AutoBackstab.m_Var)
		AutoBackstab(pLocal, pWeapon, cmd);
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
						G::Util.FixMovement(vAngleTo, cmd);
						cmd->viewangles = vAngleTo;
						cmd->buttons |= IN_ATTACK;
					}
				}
			}
		}
	}
}
