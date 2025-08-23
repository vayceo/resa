//
// Created on 19.09.2023.
//

#pragma once

#include "common.h"

struct ObjectProperties {
    int id {17581};
    CVector position {};
    CVector rotation {};
};

class CSnapShots {

public:
    static RwTexture *CreatePedSnapShot(int iModel, unsigned int dwColor, CVector *vecRot, CVector* offset);
    static RwTexture *CreateVehicleSnapShot(uint32 iModel, uint32_t dwColor, CVector *vecRot, CVector *offset);

    static RwTexture *CreateObjectSnapShot(int modelId, CVector *vecRot, CVector *offset);

    static RwTexture *CreateMainPed();
};

