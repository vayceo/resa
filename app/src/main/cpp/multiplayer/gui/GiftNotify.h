//
// Created on 30.05.2023.
//

#pragma once


#include <jni.h>


class CGiftNotify {
public:
    static inline jobject       thiz            = nullptr;
    static inline jclass        clazz           = nullptr;
   // static inline char         m_pGiftName[100] {};
   // static inline RwTexture*   m_pGiftBoxTex    = nullptr;
   // static inline bool         m_bIsShow        = false;

public:
    //static void Render();
    static void show(char *prizeName, int rare = 0);
};



