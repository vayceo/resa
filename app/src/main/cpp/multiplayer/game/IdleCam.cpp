//
// Created on 01.05.2023.
//

#include "IdleCam.h"
#include "util/patch.h"

CIdleCam gIdleCam;

void CIdleCam::InjectHooks() {
    CHook::Write(g_libGTASA + (VER_x32 ? 0x006781D8 : 0x84E3D8), &gIdleCam);
}
