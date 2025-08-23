//
// Created on 23.09.2023.
//

#include "WeaponEffects.h"
#include "../util/patch.h"

void CWeaponEffects::Render() {
    CHook::CallFunction<void>(g_libGTASA + (VER_x32 ? 0x005E3390 + 1 : 0x708DF0));
}
