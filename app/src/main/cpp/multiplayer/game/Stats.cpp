//
// Created by Traw-GG on 17.07.2025.
//
#include "Stats.h"
#include "util/patch.h"

void CStats::IncrementStat(eStats stat, float value) {
    CHook::CallFunction<void>("_ZN6CStats13IncrementStatEtf", stat, value);
}

float CStats::GetPercentageProgress() {
    return CHook::CallFunction<float>("_ZN6CStats21GetPercentageProgressEv");
}