//
// Created on 11.02.2023.
//
#pragma once

#include <jni.h>
#include "GuiWrapper.h"


class CSpeedometr : public CGuiWrapper<CSpeedometr>{

public:
    enum
    {
        BUTTON_ENGINE,
        BUTTON_LIGHT,
        BUTTON_TURN_LEFT,
        BUTTON_TURN_RIGHT,
        BUTTON_TURN_ALL
    };

    static inline float fFuel = 0.f;
    static inline int iMilliage = 0;

public:
    static void UpdateSpeed();
    static void UpdateInfo();
};
