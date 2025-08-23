#include "../main.h"
#include "../game/game.h"
#include "netgame.h"
#include "ActorPool.h"
#include "../game/Entity/Ped/Ped.h"

void CActorPool::Free()
{
    auto ids = GetAllIds();
    for (auto& id : ids) {
        Delete(id);
    }
}

void CActorPool::Spawn(uint16_t actorId, int iSkin, CVector vecPos, float fRotation, float fHealth, float bInvulnerable)
{
	if (GetAt(actorId))
	{
		Delete(actorId);
	}
	try {
		auto newActor = new CActorPed((uint16_t) iSkin, vecPos, fRotation, fHealth, bInvulnerable);

		list[actorId] = newActor;
	}
	catch (...) {

	}
}

void CActorPool::Spawn(uint16_t actorId, int iSkin, CVector vecPos, float fRotation, float fHealth, float bInvulnerable, char* name, float armour, int weaponId)
{
    if (GetAt(actorId))
    {
        Delete(actorId);
    }
    try {
        auto newActor = new CActorPed((uint16_t) iSkin, vecPos, fRotation, fHealth, bInvulnerable);

        newActor->SetName(name);
        newActor->m_pPed->m_fArmour = armour;
        newActor->m_pPed->GiveWeapon(weaponId, 99999);

        list[actorId] = newActor;
    }
    catch (...) {

    }
}

CActorPed *CActorPool::FindActorFromGta(CEntity *entity) {
    for(auto & pair : list) {
        auto &pActor = pair.second;
		if(pActor->m_pPed == (CPed*)entity)
			return pActor;
	}
	return nullptr;
}

ActorId CActorPool::FindActorIdFromGta(CEntity *entity) {
    for(auto & pair : list) {
        auto &pActor = pair.second;
        if(pActor->m_pPed == (CPed*)entity)
            return pair.first;
    }

	return INVALID_PLAYER_ID;
}

void CActorPool::Delete(uint16_t actorId) {
    if(GetAt(actorId)) {
        delete list[actorId];
        list.erase(actorId);
    }
}

void CActorPool::SetName(uint16 actorId, const char* name) {
	auto pActor = GetAt(actorId);

	if(pActor)
		pActor->SetName(name);
}

void CActorPool::SetArmour(uint16 actorId, float armour) {
    auto pActor = GetAt(actorId);

    if(pActor)
        pActor->m_pPed->m_fArmour = armour;
}

char* CActorPool::GetName(uint16 actorId) {
	auto pActor = GetAt(actorId);

	if(pActor)
		return pActor->m_szName;

	return " ";
}
