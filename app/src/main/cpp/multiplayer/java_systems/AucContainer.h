//
// Created on 10.12.2022.
//

#pragma once

#include <jni.h>
#include "GuiWrapper.h"


class CAucContainer : public CGuiWrapper<CAucContainer> {

public:
    static void Show(int id, int type, int price);
};



