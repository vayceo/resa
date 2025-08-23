//
// Created on 20.09.2023.
//

#include "TimeCycle.h"
#include "util/patch.h"

void CTimeCycle::InjectHooks() {
    CHook::Write(g_libGTASA + (VER_x32 ? 0x006793E4 : 0x8507F0), &m_VectorToSun);
    CHook::Write(g_libGTASA + (VER_x32 ? 0x00676404 : 0x84A880), &m_CurrentStoredValue);

    CHook::Write(g_libGTASA + (VER_x32 ? 0x00676BB8 : 0x84B7D0), &CTimeCycle::m_CurrentColours);
    CHook::Write(g_libGTASA + (VER_x32 ? 0x006770C0 : 0x84C1D0), &CTimeCycle::m_BelowHorizonGrey);
}
