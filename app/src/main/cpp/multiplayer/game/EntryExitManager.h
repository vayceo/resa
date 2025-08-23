//
// Created by Traw-GG on 09.08.2024.
//
#pragma once

#include "Vector.h"
#include "Vector2D.h"

#include "Pool.h"
#include "Core/QuadTreeNode.h"
#include "EntryExit.h"

class CEntity;

enum ExitEnterState : int32 {
    EXIT_ENTER_STATE_0,
    EXIT_ENTER_STATE_1,
    EXIT_ENTER_STATE_2,
    EXIT_ENTER_STATE_3,
    EXIT_ENTER_STATE_4,
};

typedef CPool<CEntryExit> CEntryExitsPool;

class CEntryExitManager {
public:
    static inline CQuadTreeNode*    mp_QuadTree;
    static inline CEntryExitsPool*  mp_poolEntryExits;

    static inline bool              ms_bBurglaryHousesEnabled;
    static inline uint32            ms_entryExitStackPosn;
    static inline int8              ms_bDisabled;
    static inline int32             ms_exitEnterState;

public:
    static void InjectHooks();

    static void Init();
};
