//
// Created on 03.08.2023.
//

#include "../game/common.h"
#include "../util/patch.h"

void LightsCreate(RpWorld* world) {
    CHook::CallFunction<void>(g_libGTASA + (VER_x32 ? 0x0046FC08 + 1 : 0x55BDCC), world);
}

void DeActivateDirectional() {
    CHook::CallFunction<void>(g_libGTASA + (VER_x32 ? 0x005D1F98 + 1 : 0x6F6664));
}

void SetFullAmbient() {
    CHook::CallFunction<void>(g_libGTASA + (VER_x32 ? 0x005D204C + 1 : 0x6F6720));
}

void SetAmbientColours() {
    CHook::CallFunction<void>(g_libGTASA + (VER_x32 ? 0x005D2068 + 1 : 0x6F6738));
}