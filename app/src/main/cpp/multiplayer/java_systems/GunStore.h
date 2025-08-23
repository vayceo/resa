//
// Created on 14.07.2023.
//

#pragma once

#include <jni.h>


class CGunStore {
public:
    static inline jobject   thiz    = nullptr;
    static inline jclass    clazz   = nullptr;

public:
    static void Show();
};
