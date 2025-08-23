//
// Created on 30.07.2023.
//

#pragma once

#include "../util/CJavaWrapper.h"
#include "../vendor/raknet/NetworkTypes.h"


template<class T>
class CGuiWrapper {
public:
    static inline   jobject thiz    = nullptr;
    static inline   jclass  clazz   = nullptr;

    static inline   bool    bIsShow = false;

public:

    /*
     * the layout should already be deleted.
     * Delete only thiz
     */
    static void DeleteCppObject() {
        auto env = g_pJavaWrapper->GetEnv();

        env->DeleteGlobalRef(thiz);
        thiz = nullptr;

        bIsShow = false;
    }

    static void Constructor() {
        if(thiz == nullptr) {
            auto env = g_pJavaWrapper->GetEnv();

            auto constructor = env->GetMethodID(clazz, "<init>","()V");
            auto newObj = env->NewObject(clazz, constructor);
            thiz = env->NewGlobalRef(newObj);
            env->DeleteLocalRef(newObj); // fuck stupid jni memory leek

            bIsShow = true;
        }
    }

    /*
     * Destroy full. Layout and object
     */
    static void Destroy() {
        if (thiz == nullptr)
            return;

        bIsShow = false;

        auto env = g_pJavaWrapper->GetEnv();

        auto method = env->GetMethodID(clazz, "destroy", "()V");
        env->CallVoidMethod(thiz, method);

        env->DeleteGlobalRef(thiz);
        thiz = nullptr;
    }
};
