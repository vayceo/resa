//
// Created on 10.01.2024.
//

#pragma once

#include "GuiWrapper.h"
#include "../game/common.h"

class CBattlePass : public CGuiWrapper<CBattlePass>{
public:
    enum class ePacketType : uint8 {
        // receive
        SHOW,
        UPDATE_MAIN_ITEM,
        UPDATE_TASK_ITEM,
        UPDATE_RATE_ITEM,
        UPDATE_DATA,
        SHOW_BUY_PAGE,

        // send
        CLICK_BUY_LEVELS,
        CLICK_GIFT,
        CLICK_BUY_PAGE,
        CLICK_MAIN_ITEM,
        CLICK_TASK_ITEM,
    };

public:
    static void ReceivePacket(Packet *p);
    static void UpdateMainItem(int id, const std::string& name, const std::string& sprite, int rare, int state);
    static void UpdateTaskItem(int id, const std::string& caption, const std::string& description, int curProgess, int maxProgress, int reward);
    static void UpdateRateItem(bool isGlobal, int id, const std::string& nick, int points);
    static void UpdateData(int myPlaceByLvl, int myPlaceByPoints, int myLvl, int pointsForUp, int myChips, jlong endTime, jlong updateTasksTime);
};
