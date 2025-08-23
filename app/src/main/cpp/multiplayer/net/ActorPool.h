#pragma once

#include "../game/CActorPed.h"
#include "NetPool.h"

class CActorPool : public CNetPool<CActorPed*>
{
public:
    static void Free();

    static void Spawn(uint16_t actorId, int iSkin, CVector vecPos, float fRotation, float fHealth, float bInvulnerable);
	static void Spawn(uint16_t actorId, int iSkin, CVector vecPos, float fRotation, float fHealth, float bInvulnerable, char* name, float armour, int weaponId);
    static void Delete(uint16_t actorId);

	static void SetName(uint16 actorId, const char* name);
	static char* GetName(uint16 actorId);

	CActorPed* FindActorFromGta(CEntity* entity);
	static ActorId FindActorIdFromGta(CEntity* entity);

    static void SetArmour(uint16 actorId, float armour);
};