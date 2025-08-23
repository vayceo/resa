#include "../main.h"
#include "../game/game.h"
#include "netgame.h"

void CObjectPool::Free()
{
    auto ids = GetAllIds();
    for (auto& id : ids) {
        Delete(id);
    }
}

bool CObjectPool::Delete(uint16_t objectId)
{
	if(!GetAt(objectId))
		return false;

	delete list[objectId];
	list.erase(objectId);

	return true;
}

bool CObjectPool::New(uint16_t objectId, int iModel, CVector vecPos, CVector vecRot, float fDrawDistance)
{
	if(GetAt(objectId))
		Delete(objectId);

	list[objectId] = CGame::NewObject(iModel, vecPos.x, vecPos.y, vecPos.z, vecRot, fDrawDistance);
	return true;
}

CObjectSamp *CObjectPool::GetObjectFromGtaPtr(CEntity *pGtaObject)
{
	for(auto &pair : list) {
		auto pObject = pair.second;

		if(pObject->m_pEntity == pGtaObject)
		{
			return pObject;
		}
	}
	return nullptr;
}

uint16_t CObjectPool::FindIDFromGtaPtr(CEntity* pGtaObject)
{
	for(auto &pair : list) {
		auto pObject = pair.second;

		if(pObject->m_pEntity == pGtaObject)
		{
			return pair.first;
		}
	}

	return INVALID_OBJECT_ID;
}

void CObjectPool::Process()
{
	static unsigned long s_ulongLastCall = 0;
	if (!s_ulongLastCall) s_ulongLastCall = GetTickCount();
	unsigned long ulongTick = GetTickCount();
	float fElapsedTime = ((float)(ulongTick - s_ulongLastCall)) / 1000.0f;
	// Get elapsed time in seconds

	for(auto &pair : list) {
		auto pObject = pair.second;

		pObject->Process(fElapsedTime);
	}

	s_ulongLastCall = ulongTick;
}
