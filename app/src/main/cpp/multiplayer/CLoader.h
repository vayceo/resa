//
// Created on 24.01.2023.
//

#ifndef RUSSIA_CLOADER_H
#define RUSSIA_CLOADER_H

#include <jni.h>

class CLoader {

public:
    static void loadBassLib();

    static void initCrashLytics();
    static void loadSetting();

    static void initJavaClasses(JavaVM *pjvm);
};


#endif //RUSSIA_CLOADER_H
