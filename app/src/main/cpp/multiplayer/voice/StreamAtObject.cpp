#include "../main.h"
#include "../game/game.h"
#include "../net/netgame.h"

#include "StreamAtObject.h"

#include "StreamInfo.h"
#include "game/Entity/Ped/Ped.h"

extern CNetGame *pNetGame;

namespace Voice {
StreamAtObject::StreamAtObject(const uint32_t color, std::string name,
                               const float distance, const WORD objectId) noexcept
    : LocalStream(StreamType::LocalStreamAtObject, color, std::move(name), distance)
    , objectId(objectId)
{}

void StreamAtObject::Tick() noexcept
{
    this->LocalStream::Tick();

    if(!pNetGame) return;

    CObjectSamp *pObject = CObjectPool::GetAt(this->objectId);
    if(!pObject) return;

    for(const auto& channel : this->GetChannels())
    {
        if(channel->HasSpeaker())
        {
            BASS_ChannelSet3DPosition(channel->GetHandle(),
                reinterpret_cast<BASS_3DVECTOR*>(&pObject->m_pEntity->GetPosition()),
                nullptr, nullptr);
        }
    }
}

void StreamAtObject::OnChannelCreate(const Channel& channel) noexcept
{
    static const BASS_3DVECTOR kZeroVector { 0, 0, 0 };

    this->LocalStream::OnChannelCreate(channel);

    if(!pNetGame) return;

    CObjectSamp *pObject = CObjectPool::GetAt(this->objectId);
    if(!pObject) return;

    BASS_ChannelSet3DPosition(channel.GetHandle(),
        reinterpret_cast<BASS_3DVECTOR*>(&pObject->m_pEntity->GetPosition()),
        &kZeroVector, &kZeroVector);
}

}
