//
// Created on 09.07.2023.
//

#pragma once


#include <jni.h>
#include "GuiWrapper.h"


class CTreasure : public CGuiWrapper<CTreasure> {
public:

public:
    static void Show(int treasureId, bool available);
    static void open(int prizeId);
};


