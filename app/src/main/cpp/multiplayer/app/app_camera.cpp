//
// Created on 03.08.2023.
//

#include "app.h"
#include "../util/patch.h"

void CameraSize(RwCamera* camera, RwRect* rect, RwReal viewWindow, RwReal aspectRatio) {
    CHook::CallFunction<void>(g_libGTASA + (VER_x32 ? 0x005D32AC + 1 : 0x6F7F84), camera, rect, viewWindow, aspectRatio);
}

void CameraDestroy(RwCamera* camera) {
    CHook::CallFunction<void>(g_libGTASA + (VER_x32 ? 0x005D33A4 + 1 : 0x6F80C0), camera);
}