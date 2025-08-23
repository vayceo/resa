//
// Created on 13.07.2023.
//

#pragma once

#include <jni.h>


class CSamwill {
public:
    static inline jobject   thiz    = nullptr;
    static inline jclass    clazz   = nullptr;

public:
    static void Show();
};
