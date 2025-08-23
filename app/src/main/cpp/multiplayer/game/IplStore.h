//
// Created on 15.11.2023.
//

#pragma once

#include "common.h"
#include "IplDef.h"
#include "Core/Pool.h"
#include "Core/QuadTreeNode.h"

typedef CPool<IplDef> CIplPool;

class CIplStore {
public:
    static inline CQuadTreeNode*    ms_pQuadTree;
    static inline CIplPool*         ms_pPool;

public:
    static void InjectHooks();

    static void Initialise();
    static void Shutdown();

    static int32 AddIplSlot(const char* name);
    static void AddIplsNeededAtPosn(const CVector* posn);
    static void EnsureIplsAreInMemory(const CVector* posn);
    static void LoadIpls(CVector posn, bool bAvoidLoadInPlayerVehicleMovingDirection);

    static void RemoveIpl(int32 iplSlotIndex);
};

