//
// Created on 14.04.2023.
//

#pragma once

#include "tHandlingData.h"
#include "tBikeHandlingData.h"
#include "tFlyingHandlingData.h"
#include "tBoatHandlingData.h"
#include <vector>

constexpr auto ACCEL_CONST = 1.f / (50.f * 50.f); // This number 50 seems to be coming up a lot...;

// 0xC2B9BC - Used for velocity conversion from file to game units
constexpr auto VELOCITY_CONST = 0.277778f / 50.f;


class cHandlingDataMgr {
public:
    float m_fCoefficientOfRestitution;
    float m_fWheelFriction;
    float m_aResistanceTable[3];

    static inline std::unordered_map<int, tHandlingData>       m_aVehicleHandling;
    static inline std::unordered_map<int, tBikeHandlingData>   m_aBikeHandling;
    static inline std::unordered_map<int, tFlyingHandlingData> m_aFlyingHandling;
    static inline std::unordered_map<int, tBoatHandlingData>   m_aBoatHandling;

public:
    static void InjectHooks();

    cHandlingDataMgr();

    /// Process handling.cfg
    static void LoadHandlingData();

    static tFlyingHandlingData* GetFlyingPointer(uint8_t handlingId);
    static tBoatHandlingData*   GetBoatPointer(uint8_t handlingId);
    static tHandlingData*       GetVehiclePointer(uint32_t handlingId);
    static tBikeHandlingData*   GetBikeHandlingPointer(uint32_t handlingId);

    static int FindExactWord(const char *name, const char *nameTable, unsigned int entrySize,
                      unsigned int entryCount);

    static void ConvertDataToGameUnits(tHandlingData *h);

    static int GetHandlingId(const char *nameToFind);
};

//static_assert(sizeof(cHandlingDataMgr) == 0xC624, "Invalid size cHandlingDataMgr");