//
// Created on 16.04.2023.
//

#pragma once

#include <jni.h>
#include "GuiWrapper.h"


class CDice : public CGuiWrapper<CDice>{
public:
    static constexpr int MAX_PLAYERS_CASINO_DICE = 5;

#pragma pack(push, 1)
    struct DicePacket
    {
        uint8_t bIsShow;
        uint8_t tableId;
        uint32_t bet;
        uint32_t bank;

        uint16_t playerID[MAX_PLAYERS_CASINO_DICE];
        uint8_t playerStat[MAX_PLAYERS_CASINO_DICE];
        uint8_t time;
        uint16_t crupId;
    };
    static_assert(sizeof(DicePacket) == 0x1C);
#pragma pack(pop)

public:
    static void Update(DicePacket *data);
    static void TempToggle(bool toggle);
};

