//
// Created on 23.05.2024.
//

#include "common.h"
#include "Automobile.h"
#include "../Ped/Ped.h"

bool CAutomobile::BreakTowLink() {
    if (m_pTractor) {
        CEntity::ClearReference(m_pTractor->m_pTrailer);
        CEntity::ClearReference(m_pTractor);
    }

    switch (m_nStatus) {
        case eEntityStatus::STATUS_IS_TOWED:
        case eEntityStatus::STATUS_IS_SIMPLE_TOWED: {
            if (pDriver) {
                m_nStatus = pDriver->IsPlayer() ? eEntityStatus::STATUS_PLAYER : eEntityStatus::STATUS_PHYSICS;
            } else {
                m_nStatus = fHealth >= 1.f ? eEntityStatus::STATUS_ABANDONED : eEntityStatus::STATUS_WRECKED;
            }
            return true;
        }
    }
    return false;
}