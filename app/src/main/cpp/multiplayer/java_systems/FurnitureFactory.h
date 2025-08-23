//
// Created on 12.07.2023.
//

#pragma once

#include <jni.h>


class CFurnitureFactory {
public:
    static inline jobject   thiz    = nullptr;
    static inline jclass    clazz   = nullptr;

public:
    static void Show(int furnitureType);
    static inline void Destroy();
};
