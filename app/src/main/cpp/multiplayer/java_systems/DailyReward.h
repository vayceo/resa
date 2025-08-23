//
// Created on 31.01.2023.
//

#pragma once

#include <jni.h>
#include "GuiWrapper.h"


class CDailyReward : public CGuiWrapper<CDailyReward> {

public:
    static void Show(int day, int second);
    static void ReceivePacket(Packet *p);
};


