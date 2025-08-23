//
// Created on 21.09.2023.
//

#include "BrightLights.h"
#include "util/patch.h"

void CBrightLights::RenderOutGeometryBuffer() {
    CHook::CallFunction<void>(g_libGTASA + (VER_x32 ? 0x005C2C78 + 1 : 0x6E6FBC));
}
