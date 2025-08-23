//
// Created on 23.09.2023.
//

#include "WaterCannons.h"
#include "../util/patch.h"

void CWaterCannons::Render() {
    CHook::CallFunction<void>(g_libGTASA + (VER_x32 ? 0x005CBBAC + 1 : 0x6F054C));
}
