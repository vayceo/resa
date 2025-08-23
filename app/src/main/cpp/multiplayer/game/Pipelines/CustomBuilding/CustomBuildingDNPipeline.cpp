//
// Created on 25.11.2023.
//

#include "CustomBuildingDNPipeline.h"
#include "../../../util/patch.h"

void CCustomBuildingDNPipeline::InjectHooks() {
    CHook::Write(g_libGTASA + (VER_x32 ? 0x676E88 : 0x84BD68), &m_fDNBalanceParam);
}
