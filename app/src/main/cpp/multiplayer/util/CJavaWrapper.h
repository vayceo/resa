#pragma once

#include <jni.h>

#include <string>

#define EXCEPTION_CHECK(env) \
	if ((env)->ExceptionCheck()) \ 
	{ \
		(env)->ExceptionDescribe(); \
		(env)->ExceptionClear(); \
		return; \
	}

class CJavaWrapper
{
	jmethodID j_Vibrate;

	jmethodID s_ShowClientSettings;

	jmethodID s_setPauseState;

	jmethodID s_ExitGame;


public:
	static JNIEnv* GetEnv();

    static void* NVThreadSpawnProc(void* arg);
	void ShowClientSettings();

	void SetPauseState(bool a1);

	int RegisterSexMale;
	int RegisterSkinValue;
	int RegisterSkinId;

	CJavaWrapper(JNIEnv* env, jobject activity);
	~CJavaWrapper();

	void ClearScreen();
	void ExitGame();

	void Vibrate(int milliseconds);

    void SendBuffer(const std::string& text) const;
    void OpenUrl(const std::string& url) const;

	jobject activity;

    void hideLoadingScreen();

    void CheckSignature();
};

extern CJavaWrapper* g_pJavaWrapper;