//
// Created on 14.01.2023.
//

#pragma once

#include <jni.h>


class CMedic {
public:
    static inline jobject   thiz    = nullptr;
    static inline jclass    clazz   = nullptr;
    static inline bool      bIsShow = false;

public:
    static void showPreDeath(char* nick, int id);
    //void hidePreDeath();
    static void showMedGame(char *nick);
    static void hide();

    static void ConstructIfNeed();
};
