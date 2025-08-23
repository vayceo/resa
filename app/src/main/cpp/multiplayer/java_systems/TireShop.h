//
// Created by Лихватов Иван on 20.03.2023.
//

#pragma once

#include <jni.h>
#include "GuiWrapper.h"


class CTireShop : public CGuiWrapper<CTireShop> {

public:
    static void show(uint32_t price);
    static void hide();

    static float GetCurrentValue(int valueType);
};

