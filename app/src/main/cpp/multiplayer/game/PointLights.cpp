//
// Created by traw-GG on 23.09.2023.
//

#include "PointLights.h"
#include "../util/patch.h"

void CPointLights::InjectHooks() {
    CHook::Write(g_libGTASA + (VER_x32 ? 0x006770BC : 0x84C1C8), &NumLights);
    CHook::Write(g_libGTASA + (VER_x32 ? 0x00676FEC : 0x84C028), &aLights);
}

void CPointLights::RenderFogEffect() {
    CHook::CallFunction<void>(g_libGTASA + (VER_x32 ? 0x005B19D0 + 1 : 0x6D6068));
}

void CPointLights::AddLight(uint8 lightType, CVector point, CVector direction, float radius, float red, float green, float blue, uint8 fogType, bool generateExtraShadows, CEntity* entityAffected) {
    CHook::CallFunction<void>("_ZN12CPointLights8AddLightEh7CVectorS0_ffffhbP7CEntity", lightType, point, direction, radius, red, green, blue, fogType, generateExtraShadows, entityAffected);
}
