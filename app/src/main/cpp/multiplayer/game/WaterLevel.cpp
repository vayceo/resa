//
// Created on 23.09.2023.
//

#include "WaterLevel.h"
#include "../util/patch.h"

void CWaterLevel::RenderWaterFog() {
    CHook::CallFunction<void>(g_libGTASA + (VER_x32 ? 0x0059BF84 + 1 : 0x6C0268));
}

void CWaterLevel::RenderWater() {
    CHook::CallFunction<void>("_ZN11CWaterLevel11RenderWaterEv");
}
