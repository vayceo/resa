//
// Created on 18.01.2024.
//

#pragma once

#include "GuiWrapper.h"

class CBusStation : public CGuiWrapper<CBusStation>{

public:
    static void ReceivePacket(Packet* p);
    static void Update(int time);
};
