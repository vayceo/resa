//
// Created on 18.09.2023.
//

#pragma once

#include <jni.h>

class CSharedPreferences {
public:
    static inline jclass clazz;

public:
    static void GetString(char *key, char *in);
    static int GetInt(char *key);
};