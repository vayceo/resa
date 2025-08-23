//
// Created by Traw-GG on 13.07.2025.
//

#include "AEAudioEntity.h"
#include "util/patch.h"

void CAEAudioEntity::InjectHooks() {
    CHook::Write(g_libGTASA + (VER_x32 ? 0x00676050 : 0x84A128), &m_pAudioEventVolumes);
}