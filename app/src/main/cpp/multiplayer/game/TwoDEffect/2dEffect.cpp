//
// Created by traw-GG on 11.07.2025.
//

#include "2dEffect.h"
#include "util/patch.h"

void C2dEffect::InjectHooks() {
    //CHook::Write(g_libGTASA + (VER_x32 ? 0x676F1C : 0x84BE90), &g2dEffectPluginOffset);
}

uint32 C2dEffect::get2dEffectPlugin() {
    g2dEffectPluginOffset = (uint32)(g_libGTASA + (VER_x32 ? 0x00A1FB18 : 0xCC0A00));
    return g2dEffectPluginOffset;
}
