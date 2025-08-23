//
// Created by Traw-GG on 09.08.2024.
//
#include "EntryExit.h"

void CEntryExit::GetPositionRelativeToOutsideWorld(CVector& outPos) {
    const auto enex = GetLinkedOrThis();
    if (enex->m_nArea != eAreaCodes::AREA_CODE_NORMAL_WORLD) {
        outPos += GetPosition() - enex->m_vecExitPos;
    }
}

CVector CEntryExit::GetPosition() const {
    return CVector{ m_recEntrance.GetCenter(), m_fEntranceZ };
}