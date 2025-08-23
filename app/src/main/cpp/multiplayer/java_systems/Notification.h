//
// Created on 01.02.2023.
//

#pragma once

#include <jni.h>


class CNotification {
public:
    static inline jobject thiz = nullptr;

public:
    static void show(int type, const char *text, int duration, int actionId, const char* butt1, const char* butt2);
    static void hide();
};
