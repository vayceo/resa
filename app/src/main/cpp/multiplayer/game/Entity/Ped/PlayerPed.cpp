//
// Created on 04.05.2023.
//

#include "PlayerPed.h"
#include "game/Plugins/RpAnimBlendPlugin/RpAnimBlend.h"
#include "game/Animation/AnimManager.h"
#include "util/patch.h"

void CPlayerPed::ReApplyMoveAnims() {
   // Log("ReApplyMoveAnims");

    constexpr AnimationId anims[]{
            ANIM_ID_WALK,
            ANIM_ID_RUN,
            ANIM_ID_SPRINT,
            ANIM_ID_IDLE,
            ANIM_ID_WALK_START
    };
    for (auto & id : anims) {
        if (auto anim = RpAnimBlendClumpGetAssociation(this->m_pRwClump, id)) {
            if (anim->GetHashKey() != CAnimManager::GetAnimAssociation(m_nAnimGroup, id)->GetHashKey()) {
                CAnimBlendAssociation* addedAnim = CAnimManager::AddAnimation(m_pRwClump, m_nAnimGroup, id);
                addedAnim->m_fBlendDelta = anim->m_fBlendDelta;
                addedAnim->m_fBlendAmount = anim->m_fBlendAmount;

                anim->m_nFlags |= ANIMATION_FREEZE_LAST_FRAME;
                anim->m_fBlendDelta = -1000.0f;
            }
        }
    }
}

float CPlayerPed::GetWeaponRadiusOnScreen() {
    CWeapon& wep = GetActiveWeapon();
    CWeaponInfo& wepInfo = wep.GetWeaponInfo(this);

    if (wep.IsTypeMelee())
        return 0.0f;

    const float accuracyProg = 0.5f / wepInfo.m_fAccuracy;
    switch (wep.m_nType) {
        case eWeaponType::WEAPON_SHOTGUN:
        case eWeaponType::WEAPON_SPAS12_SHOTGUN:
        case eWeaponType::WEAPON_SAWNOFF_SHOTGUN:
            return std::max(0.2f, accuracyProg);

        default: {
            const float rangeProg = std::min(1.0f, 15.0f / wepInfo.m_fWeaponRange);
            const float radius = (m_pPlayerData->m_fAttackButtonCounter * 0.5f + 1.0f) * rangeProg * accuracyProg;
            if (bIsDucking)
                return std::max(0.2f, radius / 2.0f);
            return std::max(0.2f, radius);
        }
    }
}

// --- hooks

void ReApplyMoveAnims_hook(CPlayerPed* thiz) {
    thiz->ReApplyMoveAnims();
}

void CPlayerPed::InjectHooks() {
    CHook::Redirect("_ZN10CPlayerPed16ReApplyMoveAnimsEv", &ReApplyMoveAnims_hook);
}
