//
// Created by Лихватов Иван on 26.04.2023.
//

#pragma once

#include <jni.h>
#include "GuiWrapper.h"


class CTheftAuto : public CGuiWrapper<CTheftAuto> {
public:
    static void startRendering();
    static void show();
};
