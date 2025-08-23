//
// Created on 23.09.2023.
//

#ifndef RUSSIA_BIRDS_H
#define RUSSIA_BIRDS_H


#include "common.h"

enum class eBirdsBiome : int32 {
    BIOME_WATER  = 0,
    BIOME_DESERT = 1,
    BIOME_NORMAL = 2,
};


class CBirds {

public:
    static void Init();
    static void CreateNumberOfBirds(CVector vecStartPos, CVector vecTargetPos, int32 iBirdCount, eBirdsBiome biome, bool bCheckObstacles);
    static void Shutdown();
    static void Update();
    static void Render();
    static void HandleGunShot(const CVector* pointA, const CVector* pointB);

    static void InjectHooks();
};


#endif //RUSSIA_BIRDS_H
