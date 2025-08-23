//
// Created on 01.05.2023.
//

#pragma once

#include <jni.h>
#include "GuiWrapper.h"


class CActionsPed : public CGuiWrapper<CActionsPed>{
public:
    static inline int           m_dwProgress    = 0;
    static inline uint16_t      m_nSelectedId   = INVALID_PLAYER_ID;
    static inline bool          bPressed        = false;

public:
    static uint16_t findSelectEntity();
    static void drawProgress();
    static void showActions();
};
