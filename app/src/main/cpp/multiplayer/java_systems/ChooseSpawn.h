//
// Created on 03.07.2023.
//

#pragma once

#include <jni.h>
#include "raknet/NetworkTypes.h"
#include "GuiWrapper.h"


class CChooseSpawn : public CGuiWrapper<CChooseSpawn>{
public:

public:
    static void Update(int organization, int station, int exit, int garage, int house);
    static void packetToggle(Packet *p);
};
