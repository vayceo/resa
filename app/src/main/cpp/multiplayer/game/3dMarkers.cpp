//
// Created on 27.07.2023.
//

#include "common.h"
#include "3dMarkers.h"
#include "../util/patch.h"

void C3dMarkers::PlaceMarkerSet(uint32 id, e3dMarkerType type, CVector& posn, float size, uint8 red, uint8 green, uint8 blue, uint8 alpha, uint16 pulsePeriod, float pulseFraction, int16 rotateRate) {
    PlaceMarker(id, type, posn, size, red, green, blue, static_cast<uint8>((float)alpha * 1.0f / 3.0f), pulsePeriod, pulseFraction, 1, 0.0f, 0.0f, 0.0f, false);
}

uintptr* C3dMarkers::PlaceMarker(uint32 id, e3dMarkerType type, CVector& posn, float size, uint8 red, uint8 green, uint8 blue, uint8 alpha, uint16 pulsePeriod, float pulseFraction, int16 rotateRate, float nrm_x, float nrm_y, float nrm_z, bool zCheck) {
    return CHook::CallFunction<uintptr*>(g_libGTASA + (VER_x32 ? 0x005C3670 + 1 : 0x6E7A50), id, type, posn, size, red, green, blue, alpha, pulsePeriod, pulseFraction, rotateRate, nrm_x, nrm_y, nrm_z, zCheck);
}