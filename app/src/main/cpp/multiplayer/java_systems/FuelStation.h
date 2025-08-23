//
// Created on 22.07.2023.
//

#pragma once

#include <jni.h>


class CFuelStation {
public:
    static inline jobject   thiz    = nullptr;
    static inline jclass    clazz   = nullptr;

public:
    static void Show(int type, int price1, int price2, int price3, int price4, int price5,
                     int maxCount);
};
