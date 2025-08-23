//
// Created on 17.03.2024.
//

#pragma once

#include "GuiWrapper.h"
#include "game/common.h"

class CMilk : public CGuiWrapper<CMilk>{
public:
    enum class eRpcId: uint8 {
        // receive
        SHOW,

        // send
        ON_CLOSE
    };

public:
    static void ReceivePacket(Packet* p);
};
