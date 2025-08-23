//
// Created by Traw-GG on 13.07.2025.
//
#pragma once

#include "common.h"
#include "Vector.h"

#include "Audio/managers/AESoundManager.h"
#include "Audio/loaders/AEMP3BankLoader.h"

union CAEAudioHardwarePlayFlags {
    uint16 m_nFlags{};
    struct {
        uint16 m_bIsFrontend : 1;
        uint16 m_bIsUncompressable : 1;
        uint16 m_bIsUnduckable : 1;
        uint16 m_bIsStartPercentage : 1;
        uint16 m_bIsMusicMastered : 1;
        uint16 : 1;
        uint16 m_bIsRolledOff : 1;
        uint16 m_bIsSmoothDucking : 1;

        uint16 m_bIsForcedFront : 1;
        uint16 m_bUnpausable : 1;
    };

    void CopyFromAESound(const CAESound& sound) {
        m_bIsFrontend        = sound.GetFrontEnd();
        m_bIsUncompressable  = sound.GetUncompressable();
        m_bIsUnduckable      = sound.GetUnduckable();
        m_bIsStartPercentage = sound.GetStartPercentage();
        m_bIsMusicMastered   = sound.GetMusicMastered();
        m_bIsRolledOff       = sound.GetRolledOff();
        m_bIsSmoothDucking   = sound.GetSmoothDucking();
        m_bIsForcedFront     = sound.GetForcedFront();
        m_bUnpausable        = m_bIsFrontend ? sound.GetUnpausable() : false;
    }
};

class CAEAudioHardware {
public:
#if VER_x32
    uint8                   audioHardware[0xCF4];
#else
    uint8                   audioHardware[0xD98];
#endif

public:
    static void InjectHooks();

    void LoadSoundBank(uint16 bankId, int16 bankSlotId);
    bool IsSoundBankLoaded(uint16 bankId, int16 bankSlotId);
    void RequestVirtualChannelSoundInfo(uint16 idx, uint16 sfxId, uint16 bankSlotId);

    static inline CAEAudioHardware* AEAudioHardware;
    static CAEAudioHardware* getAEAudioHardware() {
        AEAudioHardware = (CAEAudioHardware*)(g_libGTASA + (VER_x32 ? 0x942ED0 : 0xBA9640));
        return AEAudioHardware;
    }
};
VALIDATE_SIZE(CAEAudioHardware, (VER_x32 ? 0xCF4 : 0xD98)); // Size might be bigger, but nothing is accessed beyond `0x1014`
