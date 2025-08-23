//
// Created on 03.10.2023.
//

#pragma once


#include "GuiWrapper.h"
#include "../game/common.h"

class CTaxi : public CGuiWrapper<CTaxi> {
public:
    enum class RPC_TYPE : uint8 {
        TOGGLE,
        NOTIFICATION,
        INFO,
        APPLY_ORDER
    };

    static inline uint16 m_nOrderId{};

public:
    static void ReceivePacket(Packet* p);
    static void NewOrder(float i);
    static void UpdateInfo(int fare, int orderCounts, int salary);
};

