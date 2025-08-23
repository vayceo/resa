#include "../main.h"
#include "game.h"
#include "../net/netgame.h"
#include "common.h"

#include "..//CDebugInfo.h"

#include "CActorPed.h"

#include "game/Entity/Ped/Ped.h"
#include "game/Streaming.h"

CActorPed::CActorPed(uint16_t usModel, CVector vecPosition, float fRotation, float fHealth, bool bInvulnerable)
{
	if (!CStreaming::TryLoadModel(usModel))
		throw std::runtime_error("Model not loaded");

	if (!IsPedModel(usModel)) 
	{
		usModel = 0;
	}

	ScriptCommand(&create_actor, 22, usModel, vecPosition.x, vecPosition.y, vecPosition.z, &m_dwGTAId);

	m_pPed = GamePool_Ped_GetAt(m_dwGTAId);

	ForceTargetRotation(fRotation);
	m_pPed->Teleport(vecPosition, false);

	m_pPed->m_fHealth = fHealth;

	if (bInvulnerable) 
	{
		ScriptCommand(&set_actor_immunities, m_dwGTAId, 1, 1, 1, 1, 1);
	}
	else 
	{
		ScriptCommand(&set_actor_immunities, m_dwGTAId, 0, 0, 0, 0, 0);
	}
}

CActorPed::~CActorPed()
{
	Destroy();
}

bool IsValidGamePed(CPed* pPed)
{
	
	//IsPedPointerValid(CPed *) � 0x00435614
	if (((bool (*)(CPed*))(g_libGTASA + (VER_x32 ? 0x004A72C4 + 1 : 0x59DE5C)))(pPed)) {
		return true;
	}
	return false;
}

void CActorPed::Destroy()
{
	auto modelId = m_pPed->m_nModelIndex;

	if (IsValidGamePed(m_pPed)) 
	{
		((void (*)(CPed*))(g_libGTASA + (VER_x32 ? 0x004CE6A0 + 1 : 0x5CDC64)))(m_pPed);
	}

	m_pPed = nullptr;
	m_dwGTAId = 0;

	CStreaming::RemoveModelIfNoRefs(modelId);
}

void CActorPed::SetDead()
{
	
	if (!m_pPed) return;
	if (!GamePool_Ped_GetAt(m_dwGTAId)) return;

	if (!IsValidGamePed(m_pPed)) 
	{
		return;
	}

	RwMatrix matEntity;
	m_pPed->GetMatrix(&matEntity);

    CVector pos(matEntity.pos.x, matEntity.pos.y, matEntity.pos.z);
	m_pPed->Teleport(pos, false);

	ScriptCommand(&kill_actor, m_dwGTAId);
}

void CActorPed::ForceTargetRotation(float fRotation)
{
	
	if (!m_pPed) return;
	if (!GamePool_Ped_GetAt(m_dwGTAId)) return;

	if (!IsValidGamePed(m_pPed)) 
	{
		return;
	}

	m_pPed->m_fCurrentRotation = DegToRad(fRotation);
	m_pPed->m_fAimingRotation = DegToRad(fRotation);

	ScriptCommand(&set_actor_z_angle, m_dwGTAId, fRotation);
}

void CActorPed::ApplyAnimation(char* szAnimName, char* szAnimFile, float fDelta, int bLoop, int bLockX, int bLockY, int bFreeze, int uiTime)
{
	if (!m_pPed) return;
	if (!GamePool_Ped_GetAt(m_dwGTAId)) return;

	if(!CGame::IsAnimationLoaded(szAnimFile))
	{
		CGame::RequestAnimation(szAnimFile);
		ScriptCommand(&apply_animation, m_dwGTAId, szAnimName, szAnimFile, fDelta, bLoop, bLockX, bLockY, bFreeze, uiTime);
	}

	ScriptCommand(&apply_animation, m_dwGTAId, szAnimName, szAnimFile, fDelta, bLoop, bLockX, bLockY, bFreeze, uiTime);
}

void CActorPed::SetName(const char *name) {
	char utf_nick[255];
	cp1251_to_utf8(utf_nick, name);

	strcpy(m_szName, utf_nick);
}
