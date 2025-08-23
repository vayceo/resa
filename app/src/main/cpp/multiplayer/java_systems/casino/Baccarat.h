//
// Created on 19.01.2023.
//

#pragma once

#include <jni.h>
#include "GuiWrapper.h"


class CBaccarat : public CGuiWrapper<CBaccarat>{

public:
    static constexpr int BACCARAT_MAX_HISTORY = 9;

public:
    static void
    updateBaccarat(int redCard, int yellowCard, int totalPlayers, int totalRed, int totalYellow,
                   int totalGreen, int time, int betType, int betSum, int winner, int balance);

    static void tempToggle(bool toggle);

    static void updateLastWins(uint8_t* lastwins);
};
