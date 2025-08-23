//
// Created on 14.10.2023.
//

#pragma once

#include "GuiWrapper.h"

class CRace : public CGuiWrapper<CRace> {
public:
    enum class RPC_TYPE : int8 {
        TOGGLE,
        UPDATE
    };

public:
    static void ReceivePacket(Packet* p);
    static void Update(uint8 curPos, uint8 maxPos, uint8 curChecks, uint8 maxChecks, uint32 time);
};