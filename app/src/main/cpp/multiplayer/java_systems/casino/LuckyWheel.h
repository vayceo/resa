//
// Created on 03.07.2023.
//

#pragma once

#include <jni.h>
#include "raknet/NetworkTypes.h"
#include "java_systems/GuiWrapper.h"


class CLuckyWheel : public CGuiWrapper<CLuckyWheel> {
public:
    static void packetShow(Packet* p);
    static void Show(int count, int time);
};
