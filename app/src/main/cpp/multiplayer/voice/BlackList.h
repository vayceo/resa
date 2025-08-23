/*
    This is a SampVoice project file
    Author: CyberMor <cyber.mor.2020@gmail.ru>
    open.mp version author: AmyrAhmady (iAmir) <hhm6@yahoo.com>

    See more here https://github.com/AmyrAhmady/sampvoice
    Original repository: https://github.com/CyberMor/sampvoice

    Copyright (c) Daniel (CyberMor) 2020 All rights reserved
*/

#pragma once

#include <list>
#include <string>

#include "Header.h"

class BlackList {

    BlackList() = delete;
    ~BlackList() = delete;
    BlackList(const BlackList&) = delete;
    BlackList(BlackList&&) = delete;
    BlackList& operator=(const BlackList&) = delete;
    BlackList& operator=(BlackList&&) = delete;

public:

    struct LockedPlayer {

        LockedPlayer() = delete;
        LockedPlayer(const LockedPlayer&) = delete;
        LockedPlayer(LockedPlayer&&) = delete;
        LockedPlayer& operator=(const LockedPlayer&) = delete;
        LockedPlayer& operator=(LockedPlayer&&) = delete;

    public:

        explicit LockedPlayer(std::string playerName, PLAYERID playerId) noexcept;

        ~LockedPlayer() noexcept = default;

    public:

        const std::string playerName;
        PLAYERID playerId { SV::kNonePlayer };

    };

    static inline std::list<LockedPlayer> blackList;
public:
    static void Free() noexcept;

    static bool Load(const std::string& filePath);
    static bool Save(const std::string& filePath);

    static void LockPlayer(PLAYERID playerId);
    static void UnlockPlayer(PLAYERID playerId);
    static void UnlockPlayer(const std::string& playerName);

    static const std::list<LockedPlayer>& RequestBlackList() noexcept;

    static bool IsPlayerBlocked(PLAYERID playerId) noexcept;
    static bool IsPlayerBlocked(const std::string& playerName) noexcept;

private:

    static inline bool initStatus;

};
