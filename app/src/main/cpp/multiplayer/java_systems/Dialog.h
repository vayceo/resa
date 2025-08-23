//
// Created on 01.02.2023.
//

#pragma once

#include <jni.h>
#include "raknet/NetworkTypes.h"


class CDialog {
public:
    static inline jobject   thiz    = nullptr;
    static inline bool      bIsShow = false;

public:
    static void show(int id, int style, char *caption, char *info, char *button1, char *button2);

    static void rpcShowPlayerDialog(RPCParameters *rpcParams);

    static void hide();
};


