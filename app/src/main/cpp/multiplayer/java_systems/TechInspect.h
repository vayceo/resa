//
// Created on 05.02.2023.
//

#pragma once

#include <jni.h>
#include "GuiWrapper.h"


class CTechInspect : public CGuiWrapper<CTechInspect>{
public:
    static void Show(char *name, int gen, int candl, int brake, int starter, int nozzles, int price, int carid);
};

