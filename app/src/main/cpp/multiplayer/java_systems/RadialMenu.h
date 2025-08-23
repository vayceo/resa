//
// Created on 22.07.2023.
//

#pragma once

#include <jni.h>

class CRadialMenu {
public:
    static inline jobject   thiz    = nullptr;
    static inline jclass    clazz   = nullptr;
    static inline bool      bIsShow = false;

public:
    static void Show();
    static void Update();
};
