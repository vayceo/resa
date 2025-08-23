//
// Created on 20.09.2023.
//

#include "PostEffects.h"
#include "../util/patch.h"

void CPostEffects::ImmediateModeRenderStatesStore() {
    CHook::CallFunction<void>(g_libGTASA + (VER_x32 ? 0x005B2948 + 1 : 0x6D6EA8));
}

void CPostEffects::ImmediateModeRenderStatesSet() {
    CHook::CallFunction<void>(g_libGTASA + (VER_x32 ? 0x005B29C0 + 1 : 0x6D6F3C));
}

void CPostEffects::ImmediateModeRenderStatesReStore() {
    CHook::CallFunction<void>(g_libGTASA + (VER_x32 ? 0x005B2A18 + 1 : 0x6D6FC0));
}

void CPostEffects::Render() {
    CHook::CallFunction<void>(g_libGTASA + (VER_x32 ? 0x005B2D68 + 1 : 0x6D72DC));
}

void CPostEffects::MobileRender() {
    CHook::CallFunction<void>(g_libGTASA + (VER_x32 ? 0x005B5F78 + 1 : 0x6DA2B8));
}
