//
// Created on 04.05.2023.
//

#include "World.h"
#include "util/patch.h"
#include "game.h"

// inlined
CPlayerInfoGta& FindPlayerInfo(int32 playerId) {
    return CWorld::Players[playerId < 0 ? CWorld::PlayerInFocus : playerId];
}

// Returns player ped
// 0x56E210
CPlayerPed* FindPlayerPed(int32 playerId) {
    if(playerId == -1)
        playerId = CWorld::PlayerInFocus;

    return FindPlayerInfo(playerId).m_pPed;
}

bool PlayerIsEnteringCar()
{
    // FIXME: не работает. функция вызывается первый раз еще до создания педа. не критично, нужна лишь для скрытия кнопки прицела при входе в авто?
//    auto pPed = pGame.FindPlayerPed()->m_pPed;
//    if ( pPed->GetTaskManager().CTaskManager::FindActiveTaskByType(TASK_COMPLEX_ENTER_CAR_AS_DRIVER) )
//        return true;
//
//    return pPed->GetTaskManager().CTaskManager::FindActiveTaskByType(TASK_COMPLEX_ENTER_CAR_AS_PASSENGER) != nullptr;
    return false;
}

// Returns player vehicle
// 0x56E0D0
CVehicle* FindPlayerVehicle(int32 playerId, bool bIncludeRemote) {
    CPlayerPed* player = FindPlayerPed(playerId);
    if (!player || !player->bInVehicle)
        return nullptr;

    return player->pVehicle;
}
