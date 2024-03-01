#include "../../client/tf/c_tf_player.h"
#include "tf_weapon_knife.h"

bool C_TFKnife::DoSwingTraceKnife(trace_t& trace, QAngle vAngleOverride)
{
	// Setup a volume for the melee weapon to be swung - approx size, so all melee behave the same.
	static Vector vecSwingMinsBase(-18, -18, -18);
	static Vector vecSwingMaxsBase(18, 18, 18);

	float fBoundsScale = 1.0f;

	//This keeps crashing b/c value IsFinite so i'm commenting it out. It isnt even needed for this usage right??
	//CALL_ATTRIB_HOOK_FLOAT(fBoundsScale, melee_bounds_multiplier);

	Vector vecSwingMins = vecSwingMinsBase * fBoundsScale;
	Vector vecSwingMaxs = vecSwingMaxsBase * fBoundsScale;

	// Get the current player.
	C_TFPlayer* pPlayer = this->m_hOwner().Get()->As<C_TFPlayer*>();
	if (!pPlayer)
		return false;

	// Setup the swing range.
	float fSwingRange = GetSwingRange();

	// Scale the range and bounds by the model scale if they're larger
	// Not scaling down the range for smaller models because midgets need all the help they can get
	if (pPlayer->m_flModelScale() > 1.0f)
	{
		fSwingRange *= pPlayer->m_flModelScale();
		vecSwingMins *= pPlayer->m_flModelScale();
		vecSwingMaxs *= pPlayer->m_flModelScale();
	}

	//This keeps crashing b/c value IsFinite so i'm commenting it out. It isnt even needed for this usage right??
	//CALL_ATTRIB_HOOK_FLOAT(fSwingRange, melee_range_multiplier);

	Vector vecForward;
	AngleVectors(vAngleOverride.IsValid() ? vAngleOverride : pPlayer->EyeAngles(), &vecForward);
	Vector vecSwingStart = pPlayer->Weapon_ShootPosition();
	Vector vecSwingEnd = vecSwingStart + vecForward * fSwingRange;

	CTraceFilterIgnoreFriendlyCombatItems filter(pPlayer, COLLISION_GROUP_NONE, pPlayer->GetTeamNumber());
	UTIL_TraceLine(vecSwingStart, vecSwingEnd, MASK_SOLID, &filter, &trace);

	if (trace.fraction >= 1.0)
	{
		CTraceFilterIgnoreFriendlyCombatItems filter(pPlayer, COLLISION_GROUP_NONE, pPlayer->GetTeamNumber());
		UTIL_TraceHull(vecSwingStart, vecSwingEnd, vecSwingMins, vecSwingMaxs, MASK_SOLID, &filter, &trace);

		if (trace.fraction < 1.0)
		{
			// Calculate the point of intersection of the line (or hull) and the object we hit
			// This is and approximation of the "best" intersection
			C_BaseEntity* pHit = trace.m_pEnt;
			if (!pHit || pHit->IsBSPModel())
			{
				// Why duck hull min/max?
				//https://github.com/lua9520/source-engine-2018-hl2_src/blob/master/game/shared/gamerules.cpp#L51
				FindHullIntersection(vecSwingStart, trace, Vector(-16, -16, 0), Vector(16, 16, 36), pPlayer);
			}

			// This is the point on the actual surface (the hull could have hit space)
			vecSwingEnd = trace.endpos;
		}
	}

	return (trace.fraction < 1.0f);
}

bool C_TFKnife::CanPerformBackstabAgainstTarget(C_TFPlayer* pTarget, QAngle vAngleTo)
{
	if (!pTarget)
		return false;

	// Immune?
	int iNoBackstab = 0;
	CALL_ATTRIB_HOOK_INT_ON_OTHER(pTarget, iNoBackstab, cannot_be_backstabbed);
	if (iNoBackstab)
		return false;

	// Behind and facing target's back?
	if (IsBehindAndFacingTarget(pTarget, vAngleTo))
		return true;

	// Is target (bot) disabled via a sapper?
	//if (TFGameRules() && TFGameRules()->IsMannVsMachineMode() && pTarget->GetTeamNumber() == TF_TEAM_PVE_INVADERS)
	//{
	//	if (pTarget->m_Shared.InCond(TF_COND_MVM_BOT_STUN_RADIOWAVE))
	//		return true;

	//	if (pTarget->m_Shared.InCond(TF_COND_SAPPED) && !pTarget->IsMiniBoss())
	//		return true;
	//}

	return false;
}

bool C_TFKnife::IsBehindAndFacingTarget(C_TFPlayer* pTarget, QAngle vAngleTo)
{
	C_TFPlayer* pOwner = this->m_hOwner()->As<C_TFPlayer*>();
	if (!pOwner)
		return false;

	// Get a vector from owner origin to target origin
	Vector vecToTarget;
	vecToTarget = pTarget->WorldSpaceCenter() - pOwner->WorldSpaceCenter();
	vecToTarget.z = 0.0f;
	vecToTarget.NormalizeInPlace();

	// Get owner forward view vector
	Vector vecOwnerForward;
	AngleVectors(vAngleTo, &vecOwnerForward, NULL, NULL);
	vecOwnerForward.z = 0.0f;
	vecOwnerForward.NormalizeInPlace();

	// Get target forward view vector
	Vector vecTargetForward;
	AngleVectors(pTarget->EyeAngles(), &vecTargetForward, NULL, NULL);
	vecTargetForward.z = 0.0f;
	vecTargetForward.NormalizeInPlace();

	// Make sure owner is behind, facing and aiming at target's back
	float flPosVsTargetViewDot = DotProduct(vecToTarget, vecTargetForward);	// Behind?
	float flPosVsOwnerViewDot = DotProduct(vecToTarget, vecOwnerForward);		// Facing?
	float flViewAnglesDot = DotProduct(vecTargetForward, vecOwnerForward);	// Facestab?

	// Debug
	// 	NDebugOverlay::HorzArrow( pTarget->WorldSpaceCenter(), pTarget->WorldSpaceCenter() + 50.0f * vecTargetForward, 5.0f, 0, 255, 0, 255, true, NDEBUG_PERSIST_TILL_NEXT_SERVER );
	// 	NDebugOverlay::HorzArrow( pOwner->WorldSpaceCenter(), pOwner->WorldSpaceCenter() + 50.0f * vecOwnerForward, 5.0f, 0, 255, 0, 255, true, NDEBUG_PERSIST_TILL_NEXT_SERVER );
	// 	DevMsg( "PosDot: %3.2f FacingDot: %3.2f AnglesDot: %3.2f\n", flPosVsTargetViewDot, flPosVsOwnerViewDot, flViewAnglesDot );

	return (flPosVsTargetViewDot > 0.f && flPosVsOwnerViewDot > 0.5 && flViewAnglesDot > -0.3f);
}
