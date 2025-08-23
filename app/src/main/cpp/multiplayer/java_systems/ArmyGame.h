//
// Created on 28.07.2023.
//

#pragma once

#include <jni.h>


class CArmyGame {
public:
    static inline jobject   thiz    = nullptr;
    static inline jclass    clazz   = nullptr;

public:
    static void Show();
};
