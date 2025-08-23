//
// Created on 21.09.2023.
//

#include "Clock.h"
#include "../util/patch.h"

void CClock::InjectHooks() {
    CHook::Write(g_libGTASA + (VER_x32 ? 0x00676634 : 0x84ACD8), &ms_nGameClockSeconds);
    CHook::Write(g_libGTASA + (VER_x32 ? 0x00676260 : 0x84A540), &ms_nGameClockMinutes);
    CHook::Write(g_libGTASA + (VER_x32 ? 0x00679B40 : 0x8516A0), &ms_nGameClockHours);
    CHook::Write(g_libGTASA + (VER_x32 ? 0x0067871C : 0x84EE68), &ms_nGameClockDays);
}

/*
 * @brief   Checks if the current hour is greater than or equal to from and less than *to*.
 * @param   from Hour for *from*
 * @param   to Hour for *to*
 * @returns True if the parameters ensure the check, false otherwise.
 */
bool CClock::GetIsTimeInRange(uint8 from, uint8 to) {
    if (from > to)
        return ms_nGameClockHours >= from || ms_nGameClockHours < to;
    else
        return ms_nGameClockHours >= from && ms_nGameClockHours < to;
}
