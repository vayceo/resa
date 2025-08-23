#pragma once

#include "..//vendor/bass/bass.h"
#include <thread>
#include <queue>
#include "../game/AudioStream.h"

#include "game/Enums/eSoundAttachedTo.h"
#include "NetPool.h"

class CAudioStreamPool : public CNetPool<CAudioStream*>
{
public:
	static constexpr float DEFAULT_VOLUME = 0.7;

private:
	static inline CAudioStream* m_pIndividual;

	static inline bool bShutdownThread{};

public:
    static void PostToAudioThread(std::function<void()> task);
    static void ProcessAudioThreadTasks();
    static inline std::queue<std::function<void()>> tasks;
    static inline std::mutex mtx;

	static void AddStream(int iID, CVector* pPos, int iInterior, float fDistance, const char* szUrl, bool needRepeat, uint32 creationTime);
	static void DeleteStreamByID(int iID);

	static void PlayIndividualStream(std::string szUrl, bool needReplay = false);
	static void StopIndividualStream();

	static void Process();

    static void PlayIndividualStreamWithPos(std::string szUrl, CVector *pos, float &dist, bool needReplay= false);
	static void AttachTo(int streamId, eSoundAttachedTo type, int id);
	static void SetVolume(int streamId, float volume);

	static void Free();
	static void Init();

    static void AddStaticStream(float x, float y, float z, int iInterior, float fDistance, const char *szUrl, bool needRepeat);

    static void DeleteStreamNoThreadSafe(int streamId);
};