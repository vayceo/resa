#include "../main.h"
#include "../game/game.h"
#include "netgame.h"
#include "../game/Entity/Ped/Ped.h"

void CPlayerPool::Free()
{
    auto ids = GetAllIds();
    for (auto& id : ids) {
        Delete(id, 0);
    }
}

#include "..//chatwindow.h"
#include "..//game/game.h"
#include "..//net/netgame.h"
#include "playerpool.h"

void CPlayerPool::ApplyCollisionChecking()
{
    for(auto & pair : spawnedPlayers) {
        auto pPed = pair.second->GetPlayerPed()->m_pPed;
        if(!pPed->IsInVehicle())
        {
            m_bCollisionChecking[pair.first] = pPed->m_bCollisionProcessed;
            pPed->SetCollisionChecking(true);
        }
    }
}

void CPlayerPool::ResetCollisionChecking()
{
    for(auto & pair : spawnedPlayers) {
        auto pPed = pair.second->GetPlayerPed()->m_pPed;
        if(!pPed->IsInVehicle())
        {
            m_bCollisionChecking[pair.first] = pPed->m_bCollisionProcessed;
            pPed->SetCollisionChecking(false);
        }
    }
}

void CPlayerPool::UpdateScore(PLAYERID playerId, int iScore)
{
	if (playerId == m_LocalPlayerID)
	{
		m_iLocalPlayerScore = iScore;
	}
	else
	{
		if (playerId > MAX_PLAYERS - 1) { return; }
		m_iPlayerScores[playerId] = iScore;
	}
}

void CPlayerPool::UpdatePing(PLAYERID playerId, uint32_t dwPing)
{
	if (playerId == m_LocalPlayerID)
	{
		m_dwLocalPlayerPing = dwPing;
	}
	else
	{
		if (playerId > MAX_PLAYERS - 1) { return; }
		m_dwPlayerPings[playerId] = dwPing;
	}
}

bool CPlayerPool::Process()
{
	for(auto & pair : spawnedPlayers) {
        auto pPlayer = pair.second;
        pPlayer->Process();
	}

	CLocalPlayer::Process();
	return true;
}

bool CPlayerPool::New(PLAYERID playerId, char *szPlayerName, bool IsNPC)
{
    if(GetAt(playerId))
        Delete( playerId, 0 );

	auto newPlayer = list[playerId] = new CRemotePlayer();

    strcpy(m_szPlayerNames[playerId], szPlayerName);
    newPlayer->SetID(playerId);
    newPlayer->SetNPC(IsNPC);
    return true;

}

bool CPlayerPool::Delete(PLAYERID playerId, uint8_t byteReason)
{
	if(!GetAt(playerId))
		return false;

	if(CLocalPlayer::IsSpectating() && CLocalPlayer::m_SpectateID == playerId)
        CLocalPlayer::ToggleSpectating(false);

	delete list[playerId];

	list.erase(playerId);

	return true;
}

PLAYERID CPlayerPool::FindRemotePlayerIDFromGtaPtr(CEntity * pActor)
{
	for(auto & pair : spawnedPlayers) {
		auto pPed = pair.second->GetPlayerPed()->m_pPed;
		if (pPed == pActor)
			return pair.first;
	}
	return INVALID_PLAYER_ID;
}

