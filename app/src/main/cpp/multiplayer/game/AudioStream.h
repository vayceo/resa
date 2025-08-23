#pragma once

#include "..//vendor/bass/bass.h"
#include "game/Enums/eSoundAttachedTo.h"

class CAudioStream
{
public:
    HSTREAM 	m_hStream;
	CVector 	m_vPos{};
    int         m_nBaseInterior;
	int 		m_numCurrInterior;
	float 		m_fDistance;
	float 		m_fVolume {0.7};
	std::string m_szUrl{};
    uint32_t    m_qwCreatedTimeInSecond{};
    bool        m_bNeedSync{true};

	eSoundAttachedTo m_iAttachType;
	int m_iAttachedTo{};
	bool m_bReplay{};

public:
	void PlayStream();
	void StopStream();
    void PlayLocalStream(bool use3d);
	void ProcessAttached();

	CAudioStream(CVector* pPos, int iInterior, float fDistance, std::string szUrl, bool needReplay, float volume, uint32 creationTime);
	~CAudioStream();

	void Process(RwMatrix* pMatListener);
    void Attach(eSoundAttachedTo toType, int toId);
	void SetVolume(float fValue);


};