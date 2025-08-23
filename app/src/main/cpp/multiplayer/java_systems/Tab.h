#pragma once

#include <iostream>
#include <jni.h>
#include "GuiWrapper.h"


class CTab : public CGuiWrapper<CTab>{
public:
    static void update();
    static void setStat(int id, char *name, int score, int ping);

    static void Show();
};



