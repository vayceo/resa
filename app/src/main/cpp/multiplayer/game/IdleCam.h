//
// Created on 01.05.2023.
//

#pragma once

#include "common.h"

class CIdleCam {
public:
    CEntity  *pTarget;
    CVector     positionToSlerpFrom;
    float       timeSlerpStarted;
    float       slerpDuration;
    CVector     lastIdlePos;
    float       slerpTime;
    float       timeControlsIdleForIdleToKickIn;
    float       timeIdleCamStarted;
    int32       lastFrameProcessed;
    float       timeLastTargetSelected;
    float       timeMinimumToLookAtSomething;
    float       timeTargetEntityWasLastVisible;
    float       timeToConsiderNonVisibleEntityAsOccluded;
    float       distTooClose;
    float       distStartFOVZoom;
    float       distTooFar;
    int32       targetLOSFramestoReject;
    int32       targetLOSCounter;
    int32       zoomState;
    float       zoomFrom;
    float       zoomTo;
    float       timeZoomStarted;
    float       zoomNearest;
    float       zoomFarthest;
    float       curFOV;
    float       durationFOVZoom;
    bool        bForceAZoomOut;
    bool        bHasZoomedIn;
    uint8       pad0[2];
    float       timeBeforeNewZoomIn;
    float       timeLastZoomIn;
    float       increaseMinimumTimeFactorforZoomedIn;
    float       degreeShakeIdleCam;
    float       shakeBuildUpTime;
    int32       lastTimePadTouched;
    int32       idleTickerFrames;
    uintptr     *pCam; // CCam

public:
    static void InjectHooks();
};

VALIDATE_SIZE(CIdleCam, (VER_x32 ? 0x9C : 0xA8));

extern CIdleCam gIdleCam;