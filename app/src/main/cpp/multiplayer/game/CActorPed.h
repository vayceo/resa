#pragma once

class CActorPed
{
public:
	CActorPed(uint16_t usModel, CVector vecPosition, float fRotation, float fHealth, bool bInvulnerable);
	~CActorPed();

	void Destroy();
	void SetName(const char* name);
	void SetDead();
	void ForceTargetRotation(float fRotation);
	void ApplyAnimation(char* szAnimName, char* szAnimFile, float fDelta, int bLoop, int bLockX, int bLockY, int bFreeze, int uiTime);

public:
	CPed* 	m_pPed{};
    uintptr 	m_dwGTAId{};
	char 		m_szName[MAX_PLAYER_NAME * 2] = ""; // utf x2?
};