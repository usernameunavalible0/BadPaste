#include "../../client/tf/c_tf_player.h"
#include "tf_weapon_medigun.h"

bool C_WeaponMedigun::AllowedToHealTarget(C_BaseEntity* pTarget)
{
    C_TFPlayer* pOwner = this->m_hOwner().Get()->As<C_TFPlayer*>();
    if (!pOwner)
        return false;

    if (!pTarget)
        return false;

    if (pTarget->IsPlayer())
    {
        C_TFPlayer* pTFPlayer = pTarget->As<C_TFPlayer*>();
        if (!pTFPlayer)
            return false;

        if (!pTFPlayer->IsAlive())
            return false;

        C_TFWeaponBase* pTFWeapon = pTFPlayer->GetActiveTFWeapon();
        int iWeaponBlocksHealing = 0;
        CALL_ATTRIB_HOOK_INT_ON_OTHER(pTFWeapon, iWeaponBlocksHealing, weapon_blocks_healing);
        if (iWeaponBlocksHealing == 1)
            return false;

        bool bStealthed = (pTFPlayer->InCond(TF_COND_STEALTHED) || pTFPlayer->InCond(TF_COND_STEALTHED_USER_BUFF) || pTFPlayer->InCond(TF_COND_STEALTHED_USER_BUFF_FADING));
        bool bDisguised = pTFPlayer->InCond(TF_COND_DISGUISED);

        if (!bStealthed && (pTFPlayer->InSameTeam(pOwner) || (bDisguised && pTFPlayer->m_nDisguiseTeam() == pOwner->GetTeamNumber())))
            return true;
    }
    else
    {
        if (!pTarget->InSameTeam(pOwner))
            return false;

        if (pTarget->IsBaseObject())
            return false;

        CTFReviveMarker* pReviveMarker = reinterpret_cast<CTFReviveMarker*>(pTarget);
        if (pReviveMarker)
            return true;
    }

    return false;
}
