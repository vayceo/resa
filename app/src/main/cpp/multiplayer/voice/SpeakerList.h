#pragma once

#include "../gui/gui.h"

#include "Stream.h"
#include <unordered_map>

namespace Voice {


class SpeakerList {
    SpeakerList() = delete;
    ~SpeakerList() = delete;
    SpeakerList(const SpeakerList&) = delete;
    SpeakerList(SpeakerList&&) = delete;
    SpeakerList& operator=(const SpeakerList&) = delete;
    SpeakerList& operator=(SpeakerList&&) = delete;

public:
    static bool Init() noexcept;
    static void Free() noexcept;

    static void Show() noexcept;
    static bool IsShowed() noexcept;
    static void Hide() noexcept;

    static void Render();
    static void Draw(CVector* vec, float fDist);

    static std::array<std::unordered_map<Stream*, StreamInfo>, MAX_PLAYERS> playerStreams;
public:
    static void OnSpeakerPlay(const Stream& stream, uint16_t speaker) noexcept;
    static void OnSpeakerStop(const Stream& stream, uint16_t speaker) noexcept;

private:
    static bool initStatus;
    static bool showStatus;

    static RwTexture* tSpeakerIcon;

};
}