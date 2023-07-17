#include "../../client/tf/c_tf_player.h"
#include "../../client/tf/c_baseobject.h"
#include "tf_weaponbase_melee.h"
#include "../util_shared.h"

bool C_TFWeaponBaseMelee::DoSwingTraceInternal(trace_t& trace, CUtlVector<trace_t>* pTargetTraceVector, const QAngle vAngleOverride)
{
    int nCleaveAttack = 0;
    CALL_ATTRIB_HOOK_INT(nCleaveAttack, melee_cleave_attack);
    bool bCleave = nCleaveAttack > 0;

    // Setup a volume for the melee weapon to be swung - approx size, so all melee behave the same.
    static Vector vecSwingMinsBase(-18, -18, -18);
    static Vector vecSwingMaxsBase(18, 18, 18);

    float fBoundsScale = 1.0f;
    // this crashes game b/c value isfinite... ask Lak3?
    //CALL_ATTRIB_HOOK_FLOAT(fBoundsScale, melee_bounds_multiplier);
    Vector vecSwingMins = vecSwingMinsBase * fBoundsScale;
    Vector vecSwingMaxs = vecSwingMaxsBase * fBoundsScale;

    // Get the current player
    C_TFPlayer* pPlayer = this->m_hOwner().Get()->As<C_TFPlayer*>();
    if (!pPlayer)
        return false;

    // Setup the swing range
    float fSwingRange = this->GetSwingRange();

    // Scale the range and bounds by the model scale if they're larger
    // Not scaling down the range for smaller models because midgets need all the help they can get
    if (pPlayer->m_flModelScale() > 1.0f)
    {
        fSwingRange *= pPlayer->m_flModelScale();
        vecSwingMins *= pPlayer->m_flModelScale();
        vecSwingMaxs *= pPlayer->m_flModelScale();
    }

    //Same issue as before
    //CALL_ATTRIB_HOOK_FLOAT(fSwingRange, melee_range_multiplier);

    Vector vecForward;
    AngleVectors(vAngleOverride, &vecForward);
    Vector vecSwingStart = pPlayer->Weapon_ShootPosition();
    Vector vecSwingEnd = vecSwingStart + vecForward * fSwingRange;

    //We will never be playing AS the robots in MvM so we don't need to worry about this
    bool bDontHitTeammates = true; //pPlayer->GetTeamNumber() == TF_TEAM_PVE_INVADERS && TFGameRules()->IsMannVsMachineMode();
    CTraceFilterIgnoreTeammates ignoreTeammatesFilter(pPlayer, COLLISION_GROUP_NONE, pPlayer->GetTeamNumber());

    if (bCleave)
    {
        Ray_t ray;
        ray.Init(vecSwingStart, vecSwingEnd, vecSwingMins, vecSwingMaxs);
        C_BaseEntity* pList[256];
        //TODO!!!
        int nTargetCount = 0; //UTIL_EntitiesAlongRay(pList, ARRAYSIZE(pList), ray, FL_CLIENT | FL_OBJECT);

        int nHitCount = 0;
        for (int i = 0; i < nTargetCount; ++i)
        {
            C_BaseEntity* pTarget = pList[i];
            if (pTarget == pPlayer)
            {
                // don't hit yourself
                continue;
            }

            if (bDontHitTeammates && pTarget->GetTeamNumber() == pPlayer->GetTeamNumber())
            {
                // don't hit teammate
                continue;
            }

            if (pTargetTraceVector)
            {
                trace_t tr;
                UTIL_TraceModel(vecSwingStart, vecSwingEnd, vecSwingMins, vecSwingMaxs, pTarget, COLLISION_GROUP_NONE, &tr);
                pTargetTraceVector->AddToTail();
                pTargetTraceVector->Tail() = tr;
            }
            nHitCount++;
        }

        return nHitCount > 0;
    }
    else
    {
        bool bSapperHit = false;

        // if this weapon can damage sappers, do that trace first
        int iDmgSappers = 0;
        CALL_ATTRIB_HOOK_INT(iDmgSappers, set_dmg_apply_to_sapper);
        if (iDmgSappers != 0)
        {
            CTraceFilterIgnorePlayers ignorePlayersFilter(NULL, COLLISION_GROUP_NONE);
            UTIL_TraceLine(vecSwingStart, vecSwingEnd, MASK_SOLID, &ignorePlayersFilter, &trace);
            if (trace.fraction >= 1.0)
            {
                UTIL_TraceHull(vecSwingStart, vecSwingEnd, vecSwingMins, vecSwingMaxs, MASK_SOLID, &ignorePlayersFilter, &trace);
            }

            if (trace.fraction < 1.0f &&
                trace.m_pEnt &&
                trace.m_pEnt->IsBaseObject() &&
                trace.m_pEnt->GetTeamNumber() == pPlayer->GetTeamNumber())
            {
                C_BaseObject* pObject = static_cast<C_BaseObject*>(trace.m_pEnt);
                if (pObject->m_bHasSapper())
                {
                    bSapperHit = true;
                }
            }
        }

        if (!bSapperHit)
        {
            // See if we hit anything.
            if (bDontHitTeammates)
            {
                UTIL_TraceLine(vecSwingStart, vecSwingEnd, MASK_SOLID, &ignoreTeammatesFilter, &trace);
            }
            else
            {
                CTraceFilterIgnoreFriendlyCombatItems filter(pPlayer, COLLISION_GROUP_NONE, pPlayer->GetTeamNumber());
                UTIL_TraceLine(vecSwingStart, vecSwingEnd, MASK_SOLID, &filter, &trace);
            }

            if (trace.fraction >= 1.0)
            {
                if (bDontHitTeammates)
                {
                    UTIL_TraceHull(vecSwingStart, vecSwingEnd, vecSwingMins, vecSwingMaxs, MASK_SOLID, &ignoreTeammatesFilter, &trace);
                }
                else
                {
                    CTraceFilterIgnoreFriendlyCombatItems filter(pPlayer, COLLISION_GROUP_NONE, pPlayer->GetTeamNumber());
                    UTIL_TraceHull(vecSwingStart, vecSwingEnd, vecSwingMins, vecSwingMaxs, MASK_SOLID, &filter, &trace);
                }

                if (trace.fraction < 1.0)
                {
                    // Calculate the point of intersection of the line (or hull) and the object we hit
                    // This is and approximation of the "best" intersection
                    C_BaseEntity* pHit = trace.m_pEnt;
                    if (!pHit || pHit->IsBSPModel())
                    {
                        // Why duck hull min/max?
                        FindHullIntersection(vecSwingStart, trace, Vector(-16, -16, 0), Vector(16, 16, 36), pPlayer);
                    }

                    // This is the point on the actual surface (the hull could have hit space)
                    vecSwingEnd = trace.endpos;
                }
            }
        }

        return (trace.fraction < 1.0f);
    }
}
