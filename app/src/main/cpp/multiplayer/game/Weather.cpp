//
// Created on 20.09.2023.
//

#include "Weather.h"
#include "../util/patch.h"

void CWeather::RenderRainStreaks() {
    CHook::CallFunction<void>("_ZN8CWeather17RenderRainStreaksEv");
}

void CWeather::InjectHooks() {
    CHook::Write(g_libGTASA + (VER_x32 ? 0x006798D0 : 0x8511C8), &bScriptsForceRain);
    CHook::Write(g_libGTASA + (VER_x32 ? 0x0067616C : 0x84A358), &Wind);
    CHook::Write(g_libGTASA + (VER_x32 ? 0x00678C44 : 0x84F8B8), &WindDir);
    CHook::Write(g_libGTASA + (VER_x32 ? 0x00679788 : 0x850F28), &Foggyness);
    CHook::Write(g_libGTASA + (VER_x32 ? 0x00679A18 : 0x851450), &CloudCoverage);
    CHook::Write(g_libGTASA + (VER_x32 ? 0x006764E0 : 0x84AA30), &Rainbow);
    CHook::Write(g_libGTASA + (VER_x32 ? 0x00678378 : 0x84E718), &ExtraSunnyness);
    CHook::Write(g_libGTASA + (VER_x32 ? 0x0067609C : 0x84A1C0), &SunGlare);
    CHook::Write(g_libGTASA + (VER_x32 ? 0x006771E4 : 0x84C410), &WetRoads);
    CHook::Write(g_libGTASA + (VER_x32 ? 0x00679F4C : 0x851EA8), &Rain);
    CHook::Write(g_libGTASA + (VER_x32 ? 0x00676C88 : 0x84B970), &InTunnelness);
    CHook::Write(g_libGTASA + (VER_x32 ? 0x00676FF8 : 0x84C040), &OldWeatherType);
    CHook::Write(g_libGTASA + (VER_x32 ? 0x00676E6C : 0x84BD30), &NewWeatherType);
    CHook::Write(g_libGTASA + (VER_x32 ? 0x006788E0 : 0x84F1E8), &ForcedWeatherType);
    CHook::Write(g_libGTASA + (VER_x32 ? 0x006760FC : 0x84A278), &UnderWaterness);
    CHook::Write(g_libGTASA + (VER_x32 ? 0x006783AC : 0x84E780), &InterpolationValue);
}
