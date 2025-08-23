//
// Created on 01.04.2024.
//

#pragma once
#include "../util/CJavaWrapper.h"
#include "../game/common.h"
#include "../net/netgame.h"
#include <string>

class CJavaGui {
public:
    static inline jclass clazz;

    enum class eActionId {
        ACTION_CREATE     = -1,
        ACTION_DESTROY    = -2,

        // если нужно временно скрыть не удаляя
        ACTION_SHOW       = -3,
        ACTION_HIDE       = -4,

        ACTION_ONCLOSED   = -5,
        ACTION_GOTOP      = -6
    };

public:
    static void ReceivePacket(Packet* p);

    static void ExecuteAction(int uiId, eActionId actionId);
    static void Create(int uiId);
    static void TempToggle(int uiId, bool toggle);
    static void Destroy(int uiId);
};
