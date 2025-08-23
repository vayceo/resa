//
// Created on 21.09.2023.
//

#ifndef RUSSIA_BRIGHTLIGHTS_H
#define RUSSIA_BRIGHTLIGHTS_H


#include "Core/Vector.h"
#include "common.h"

enum eBrightLightColor : uint8 {
    BRIGHTLIGHT_NONE         = 0,
    BRIGHTLIGHT_GREEN_BIG    = 1,
    BRIGHTLIGHT_YELLOW_BIG   = 2,
    BRIGHTLIGHT_RED_BIG      = 3,
    BRIGHTLIGHT_GREEN_SMALL  = 13,
    BRIGHTLIGHT_YELLOW_SMALL = 14,
    BRIGHTLIGHT_RED_SMALL    = 15
};

class CBrightLights {

public:
    static void InjectHooks();

    static void Init();
    static void RenderOutGeometryBuffer();
    static void Render();
    static void RegisterOne(CVector posn, CVector top, CVector right, CVector at, eBrightLightColor color, uint8 arg5, uint8 arg6, uint8 arg7);
};


#endif //RUSSIA_BRIGHTLIGHTS_H
