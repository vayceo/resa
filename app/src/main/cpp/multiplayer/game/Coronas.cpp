//
// Created on 21.05.2023.
//

#include "Coronas.h"
#include "../util/patch.h"
#include "TxdStore.h"


// Creates corona by texture
void CCoronas::RegisterCorona(uint32 id, CEntity* attachTo, uint8 red, uint8 green, uint8 blue, uint8 alpha, const CVector* posn, float radius, float farClip, RwTexture* texture, eCoronaFlareType flareType, bool enableReflection, bool checkObstacles, int32 _param_not_used, float angle, bool longDistance, float nearClip, uint8 fadeState, float fadeSpeed, bool onlyFromBelow, bool reflectionDelay) {
    CHook::CallFunction<void>(g_libGTASA + (VER_x32 ? 0x005A3AAC + 1 : 0x6C71D0), id, attachTo, red, green, blue, alpha, posn, radius, farClip, texture, flareType, enableReflection, checkObstacles, _param_not_used, angle, longDistance, nearClip, fadeState, fadeSpeed, onlyFromBelow, reflectionDelay);
}

// Creates corona by type
void CCoronas::RegisterCorona(uint32 id, CEntity* attachTo, uint8 red, uint8 green, uint8 blue, uint8 alpha, const CVector* posn, float radius, float farClip, eCoronaType coronaType, eCoronaFlareType flareType, bool enableReflection, bool checkObstacles, int32 _param_not_used, float angle, bool longDistance, float nearClip, uint8 fadeState, float fadeSpeed, bool onlyFromBelow, bool reflectionDelay) {
    CHook::CallFunction<void>(g_libGTASA + (VER_x32 ? 0x005A3A20 + 1 : 0x6C7174), id, attachTo, red, green, blue, alpha, posn, radius, farClip, coronaType, flareType, enableReflection, checkObstacles, _param_not_used, angle, longDistance, nearClip, fadeState, fadeSpeed, onlyFromBelow, reflectionDelay);
}

void CCoronas::UpdateCoronaCoors(uint32 id, const CVector* posn, float farClip, float angle) {
    CHook::CallFunction<void>("_ZN8CCoronas17UpdateCoronaCoorsEjRK7CVectorff", id, posn, farClip, angle);
}

void (*CCoronas__Init)();
void CCoronas::InjectHooks() {
    CHook::Write(g_libGTASA + (VER_x32 ? 0x00679A84 : 0x851528), &SunScreenX);
    CHook::Write(g_libGTASA + (VER_x32 ? 0x00679784 : 0x850F20), &SunScreenY);

    CHook::Write(g_libGTASA + (VER_x32 ? 0x00679F40 : 0x851E90), &SunBlockedByClouds);
    CHook::Write(g_libGTASA + (VER_x32 ? 0x00675F6C : 0x849F60), &MoonSize);

    CHook::Write(g_libGTASA + (VER_x32 ? 0x00679F78 : 0x851F08), &gpCoronaTexture);
   // CHook::InlineHook("_ZN8CCoronas4InitEv", &CCoronas::Init, &CCoronas__Init);
}

// Initialises coronas
// 0x6FAA70

void CCoronas::Init() {
    {
        CCoronas__Init();
        Log("CCoronas::Init()");

        CTxdStore::ScopedTXDSlot txd{"particle"};
        //for (auto&& [tex, name, maskName] : rng::zip_view{ gpCoronaTexture, aCoronastar, coronaTexturesAlphaMasks }) { // TODO: C++23
        //    if (!tex) {
        //        tex = RwTextureRead(name, maskName);
        //    }
        //}
//        for (auto i = 0; i < CORONA_TEXTURES_COUNT; i++) {
//           gpCoronaTexture[i] = CUtil::LoadTextureFromDB("txd", "coronamoon");
//
//        }
    }

    for (auto & corona : aCoronas) {
        corona = CRegisteredCorona{}; // ������������� �������� �� ���������
    }
}

void CCoronas::Render() {
    CHook::CallFunction<void>(g_libGTASA + (VER_x32 ? 0x005A23B8 + 1 : 0x6C5CD4) );
}

void CCoronas::RenderReflections() {
    CHook::CallFunction<void>("_ZN8CCoronas17RenderReflectionsEv");
}

void CCoronas::RenderSunReflection() {
    CHook::CallFunction<void>("_ZN8CCoronas19RenderSunReflectionEv");
}
