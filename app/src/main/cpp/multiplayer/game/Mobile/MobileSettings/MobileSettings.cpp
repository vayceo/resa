//
// Created by traw-GG on 14.07.2025.
//

#include "MobileSettings.h"
#include "util/patch.h"

void CMobileSettings::InjectHooks() {
    CHook::Write(g_libGTASA + (VER_x32 ? 0x00679A3C : 0x851498), &ms_MobileSettings);
}