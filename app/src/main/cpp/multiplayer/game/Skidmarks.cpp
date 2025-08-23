//
// Created on 23.09.2023.
//

#include "Skidmarks.h"
#include "../util/patch.h"

void CSkidmarks::Render() {
    CHook::CallFunction<void>(g_libGTASA + (VER_x32 ? 0x005BE914 + 1 : 0x6E2FB4));
}
