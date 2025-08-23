#include "../main.h"
#include "../game/game.h"
#include "../net/netgame.h"

#include "StreamAtPlayer.h"

#include "StreamInfo.h"
#include "game/Entity/Ped/Ped.h"

extern CNetGame *pNetGame;

namespace Voice {
StreamAtPlayer::StreamAtPlayer(const uint32_t color, std::string name,
                               const float distance, const PLAYERID playerId) noexcept
    : LocalStream(StreamType::LocalStreamAtPlayer, color, std::move(name), distance)
    , playerId(playerId)
{}

void StreamAtPlayer::Tick() noexcept
{
    this->LocalStream::Tick();

    if(!pNetGame) return;

    CRemotePlayer *pPlayer = CPlayerPool::GetAt(this->playerId);
    if(!pPlayer) return;

    CPedSamp *pPlayerPed = pPlayer->GetPlayerPed();
    if(!pPlayerPed) return;

    for(const auto& channel : this->GetChannels())
    {
        if(channel->HasSpeaker())
        {
            BASS_ChannelSet3DPosition(channel->GetHandle(),
                reinterpret_cast<BASS_3DVECTOR*>(&pPlayerPed->m_pPed->GetPosition()),
                nullptr, nullptr);
        }
    }
}

void StreamAtPlayer::OnChannelCreate(const Channel& channel) noexcept
{
    static const BASS_3DVECTOR kZeroVector { 0, 0, 0 };

    this->LocalStream::OnChannelCreate(channel);

    if(!pNetGame) return;

    CRemotePlayer *pPlayer = CPlayerPool::GetSpawnedPlayer(this->playerId);
    if(!pPlayer) return;

    BASS_ChannelSet3DPosition(channel.GetHandle(),
        reinterpret_cast<BASS_3DVECTOR*>(&pPlayer->GetPlayerPed()->m_pPed->GetPosition()),
        &kZeroVector, &kZeroVector);
}

}