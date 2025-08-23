//
// Created on 03.08.2023.
//

#pragma once

#include "../game/common.h"

extern void DefinedState();
extern void DefinedState2d();


/**
 * Camera
 */

extern RwCamera* CameraCreate(RwInt32 width, RwInt32 height, RwBool zBuffer);
extern void CameraDestroy(RwCamera* camera);
extern void CameraSize(RwCamera* camera, RwRect* rect, RwReal viewWindow, RwReal aspectRatio);


/**
 * Lights
 */

extern void LightsCreate(RpWorld* world);