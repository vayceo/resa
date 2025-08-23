//
// Created on 27.07.2023.
//

#pragma once


#include "common.h"

enum e3dMarkerType : uint16 {
    MARKER3D_ARROW = 0,
    MARKER3D_CYLINDER = 1,
    MARKER3D_TUBE = 2,
    MARKER3D_ARROW2 = 3,
    MARKER3D_TORUS = 4,
    MARKER3D_CONE = 5,
    MARKER3D_CONE_NO_COLLISION = 6,

    // Add stuff above this
    MARKER3D_COUNT,

    // Sentinel value (Used for markers not in use)
    MARKER3D_NA = 257
};


class C3dMarkers {

    static uintptr *
    PlaceMarker(uint32 id, e3dMarkerType type, CVector &posn, float size, uint8 red, uint8 green,
                uint8 blue, uint8 alpha, uint16 pulsePeriod, float pulseFraction, int16 rotateRate,
                float nrm_x, float nrm_y, float nrm_z, bool zCheck);

public:
    static void
    PlaceMarkerSet(uint32 id, e3dMarkerType type, CVector &posn, float size, uint8 red, uint8 green,
                   uint8 blue, uint8 alpha, uint16 pulsePeriod, float pulseFraction,
                   int16 rotateRate);
};
