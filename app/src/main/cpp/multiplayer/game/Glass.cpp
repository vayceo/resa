//
// Created on 23.09.2023.
//

#include "Glass.h"
#include "../util/patch.h"

void CGlass::Render() {
    CHook::CallFunction<void>(g_libGTASA + (VER_x32 ? 0x005AB798 + 1 : 0x6D05A0));
}
