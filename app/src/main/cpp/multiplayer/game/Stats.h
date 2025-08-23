//
// Created by Traw-GG on 17.07.2025.
//
#pragma once

#include "common.h"
#include "Enums/eStats.h"
#include "Enums/eStatModAbilities.h"
#include <Enums/eRadioID.h>

enum eStatUpdateState : uint8 {
    STAT_UPDATE_DECREASE = 0,
    STAT_UPDATE_INCREASE = 1
};

enum eStatMessageCondition {
    STATMESSAGE_LESSTHAN = 0,
    STATMESSAGE_MORETHAN = 1
};

class CStats {
public:
    static void IncrementStat(eStats stat, float value = 1.f);
    static float GetPercentageProgress();
};
