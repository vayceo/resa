//
// Created on 23.09.2023.
//

#include "MovingThings.h"
#include "../util/patch.h"

void CMovingThings::Render() {
    CHook::CallFunction<void>(g_libGTASA + (VER_x32 ? 0x005A6BC8 + 1 : 0x6CA5D0));
}

void CMovingThings::Render_BeforeClouds() {
    CHook::CallFunction<void>("_ZN13CMovingThings19Render_BeforeCloudsEv");
}
