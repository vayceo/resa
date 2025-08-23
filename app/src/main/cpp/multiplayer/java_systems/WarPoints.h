//
// Created on 22.01.2024.
//

#pragma once

#include "GuiWrapper.h"

class CWarPoints : public CGuiWrapper<CWarPoints> {

public:
    static void Update(int time, int myScore, int enemyScore);
};
