//
// Created on 16.02.2023.
//

#pragma once

#include <jni.h>
#include "GuiWrapper.h"


class CDonate : public CGuiWrapper<CDonate>{
public:
    static constexpr int CLICK_ID_TREASURE = 11;

public:
    static void updateBalance(int balance);
    static void addToMyItem(int type, int value, int id);
    static void resetStash();
};
