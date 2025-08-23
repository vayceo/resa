//
// Created on 17.12.2023.
//

#pragma once

#include "GuiWrapper.h"

class CBuyPlate : public CGuiWrapper<CBuyPlate> {
public:
    enum ePacketId {
        CLICK_BU
    };

public:
    static void ReceivePacket(Packet* p);
    static void Show(std::string &sprite, int randomPrice, std::string& buyPrice);
};
