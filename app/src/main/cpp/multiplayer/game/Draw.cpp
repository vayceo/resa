//
// Created on 21.09.2023.
//

#include "Draw.h"
#include "../util/patch.h"
#include "game.h"

void CDraw::SetFOV(float fFOV, bool isCinematic) {
    fFOV = (float)(((ms_fAspectRatio - 1.3333) * 11.0) / 0.44444) + fFOV;

    if (CFirstPersonCamera::IsEnabled())
        fFOV = 120.0;
    else if (fFOV > 100)
        fFOV = 100.0;

    ms_fFOV = fFOV;
}

void CDraw::InjectHooks() {
    CHook::Write(g_libGTASA + (VER_x32 ? 0x00676B70 : 0x84B740), &ms_fFOV);
    CHook::Write(g_libGTASA + (VER_x32 ? 0x006779A0 : 0x84D378), &ms_fFarClipZ);
    CHook::Write(g_libGTASA + (VER_x32 ? 0x00676178 : 0x84A370), &ms_fNearClipZ);
    CHook::Write(g_libGTASA + (VER_x32 ? 0x006784C0 : 0x84E9A8), &ms_fAspectRatio);

    CHook::Redirect("_ZN5CDraw6SetFOVEfb", &SetFOV);
}
