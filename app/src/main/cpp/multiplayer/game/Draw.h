//
// Created on 21.09.2023.
//

#ifndef RUSSIA_DRAW_H
#define RUSSIA_DRAW_H


class CDraw {
public:
    static inline float ms_fFOV = 45.0f; // default 45.0
//    static float& ms_fLODDistance;
    static inline float ms_fNearClipZ;
    static inline float ms_fFarClipZ;
    static inline float ms_fAspectRatio;
//
//    static uint8& FadeRed;
//    static uint8& FadeGreen;
//    static uint8& FadeBlue;
//    static uint8& FadeValue;

public:
    static void InjectHooks();

    static void SetFOV(float fFOV, bool isCinematic);
    static float GetFOV() { return ms_fFOV; }
//
//    static void SetNearClipZ(float nearClip) { ms_fNearClipZ = nearClip; }
    static float GetNearClipZ() { return ms_fNearClipZ; }

    static void SetFarClipZ(float farClip) { ms_fFarClipZ = farClip; }
    static float GetFarClipZ() { return ms_fFarClipZ; }

//    static float GetAspectRatio() { return ms_fAspectRatio; }
//    static void SetAspectRatio(float ratio) { ms_fAspectRatio = ratio; }
//
//    static void CalculateAspectRatio();
//
//    // @notsa
//    static bool IsFading() { return FadeValue != 0u; }
};


#endif //RUSSIA_DRAW_H
