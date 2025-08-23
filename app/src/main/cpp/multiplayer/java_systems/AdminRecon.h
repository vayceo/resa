//
// Created on 10.01.2023.
//

#pragma once

#include "GuiWrapper.h"


class CAdminRecon : public CGuiWrapper<CAdminRecon> {

public:
    static inline PLAYERID  iPlayerID = INVALID_PLAYER_ID;

public:
    static void Update(int targetID);

    enum eButtons {
        EXIT_BUTTON,
        STATS_BUTTON,
        FREEZE_BUTTON,
        UNFREEZE_BUTTON,
        SPAWN_BUTTON,
        SLAP_BUTTON,
        REFRESH_BUTTON,
        MUTE_BUTTON,
        JAIL_BUTTON,
        KICK_BUTTON,
        BAN_BUTTON,
        WARN_BUTTON,
        NEXT_BUTTON,
        PREV_BUTTON,
        FLIP_BUTTON
    };

    static void tempToggle(bool toggle);
};

