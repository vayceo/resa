//
// Created on 18.09.2023.
//

#include "SharedPreferences.h"
#include "util/CJavaWrapper.h"

void CSharedPreferences::GetString(char* key, char *in) {
    JNIEnv* env = CJavaWrapper::GetEnv();
    jstring jKey = env->NewStringUTF(key);

    auto method = env->GetStaticMethodID(clazz, "getString", "(Ljava/lang/String;)Ljava/lang/String;");
    auto result = (jstring) env->CallStaticObjectMethod(clazz, method, jKey);
    env->DeleteLocalRef(jKey);

    const char* value = env->GetStringUTFChars(result, nullptr);
    strcpy(in, value);

    env->DeleteLocalRef(result);
}

int CSharedPreferences::GetInt(char* key) {
    JNIEnv* env = CJavaWrapper::GetEnv();
    jstring jKey = env->NewStringUTF(key);

    auto method = env->GetStaticMethodID(clazz, "getInt", "(Ljava/lang/String;)I");
    auto result = env->CallStaticIntMethod(clazz, method, jKey);

    return result;
}