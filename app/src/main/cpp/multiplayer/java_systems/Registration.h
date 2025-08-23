//
// Created on 30.08.2023.
//

#ifndef RUSSIA_REGISTRATION_H
#define RUSSIA_REGISTRATION_H


#include "GuiWrapper.h"

class CRegistration : public CGuiWrapper<CRegistration>{

public:
    static void Show(char *nick, int id);
    static uint32_t ChangeSkin(int skin);
};


#endif //RUSSIA_REGISTRATION_H
