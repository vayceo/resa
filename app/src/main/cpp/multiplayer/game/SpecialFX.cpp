//
// Created on 23.09.2023.
//

#include "SpecialFX.h"
#include "../util/patch.h"

void CSpecialFX::Render() {
    CHook::CallFunction<void>(g_libGTASA + (VER_x32 ? 0x005C0B14 + 1 : 0x6E50CC));
}
