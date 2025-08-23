//
// Created by Traw-GG on 10.08.2024.
//
#pragma once

#include <span>

#include "Vector.h"
#include "Occluder.h"

class COcclusion {
public:
    static constexpr int32 MAX_INTERIOR_OCCLUDERS            = 40;
    static constexpr int32 MAX_MAP_OCCLUDERS                 = 1'000;
    static constexpr int32 MAX_ACTIVE_OCCLUDERS              = 28;
    static constexpr int32 NUM_OCCLUDERS_PROCESSED_PER_FRAME = 16;

    static inline std::array<COccluder, MAX_MAP_OCCLUDERS> Occluders;
    static inline size_t NumOccludersOnMap;

public:
    static void InjectHooks();
};
