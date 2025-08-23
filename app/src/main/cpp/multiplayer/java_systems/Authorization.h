//
// Created on 25.07.2023.
//

#pragma once

#include <jni.h>
#include "GuiWrapper.h"


class CAuthorization : public CGuiWrapper<CAuthorization>{
public:

public:
    static void Update(char *nick, int id, bool toggleAutoLogin, bool email_acvive);
    static void SendLoginPacket(const char *password);
};
