//
// Created on 29.06.2023.
//

#pragma once

#include <jni.h>
#include "raknet/NetworkTypes.h"
#include "GuiWrapper.h"


class CAutoShop : public CGuiWrapper<CAutoShop> {

public:
    static void Packet_UpdateAutoShop(Packet* p);
    static void updateInfo(char *name, int price, int count, float maxspeed, float acceleration, int gear);
    static void toggle(bool toggle);
};
