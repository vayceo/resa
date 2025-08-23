//
// Created on 23.09.2023.
//

#ifndef RUSSIA_POINTLIGHTS_H
#define RUSSIA_POINTLIGHTS_H

#include "common.h"
#include <__ranges/take_view.h>

enum ePointLightType : uint8 {
    PLTYPE_POINTLIGHT = 0,
    PLTYPE_SPOTLIGHT = 1,
    PLTYPE_DARKLIGHT = 2
};

class CPointLight {
public:
    CVector         m_vecPosn;
    CVector         m_vecDirection;
    float           m_fRadius;
    float           m_fColorRed;
    float           m_fColorGreen;
    float           m_fColorBlue;
    CEntity*        m_pEntityToLight;
    ePointLightType m_nType;
    uint8           m_nFogType;
    bool            m_bGenerateShadows;
};

class CPointLights {
    static constexpr auto MAX_POINT_LIGHTS = 32;
public:
    static void  InjectHooks();
    static float GenerateLightsAffectingObject(const CVector* point, float* totalLighting, CEntity* entity);
    static float GetLightMultiplier(const CVector* point);
    static void  RemoveLightsAffectingObject();
    static bool  ProcessVerticalLineUsingCache(CVector point, float* outZ);
    static void  AddLight(uint8 lightType, CVector point, CVector direction, float radius, float red, float green, float blue, uint8 fogType = 0, bool generateExtraShadows = false, CEntity* entityAffected = nullptr);
    static void  RenderFogEffect();

    static void ResetNumLights() { NumLights = 0; }
    static auto GetActiveLights() { return aLights | std::ranges::views::take(NumLights); }

    static inline uint32 NumLights;
    static inline CPointLight aLights[MAX_POINT_LIGHTS];
};


#endif //RUSSIA_POINTLIGHTS_H
