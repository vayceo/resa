//
// Created on 20.09.2023.
//

#ifndef RUSSIA_TIMECYCLE_H
#define RUSSIA_TIMECYCLE_H


#include "ColourSet.h"

class CTimeCycle {

public:
    static inline CVector m_VectorToSun[16];
    static inline uint32 m_CurrentStoredValue;

    static inline CColourSet m_CurrentColours;
    static inline RwRGBA m_BelowHorizonGrey;

public:
    static void InjectHooks();

    static int32 GetShadowStrength(void) { return m_CurrentColours.m_nShadowStrength; }
};


#endif //RUSSIA_TIMECYCLE_H
