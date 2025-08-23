//
// Created on 04.10.2023.
//

#pragma once

#include "GuiWrapper.h"

class CMagicStore : public CGuiWrapper<CMagicStore> {
public:
    enum class PacketType : uint8_t {
        TOGGLE = 0,
        UPDATE_BALANCE,
        CLICK
    };

public:
    static void ReceivePacket(Packet* p);

    static void UpdateBalance(int bronze, int silver, int gold);
};
