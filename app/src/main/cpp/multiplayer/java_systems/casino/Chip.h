//
// Created on 06.12.2022.
//

#pragma once

#include <jni.h>


class CChip {
public:
    static inline jobject   thiz    = nullptr;
    static inline jclass    clazz   = nullptr;

    static inline bool      bIsShow = false;

public:
    static void show(bool isBuy, int balance);
    static void hide();
};

