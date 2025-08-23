/*
    Plugin-SDK file
    Authors: GTA Community. See more here
    https://github.com/DK22Pac/plugin-sdk
    Do not delete this comment block. Respect others' work!
*/
#pragma once

#include "common.h"

class AnimBlendFrameData;
class CPed;

// Return flags from MoveLimb() function
enum MoveLimbResult { CANT_REACH_TARGET, HAVENT_REACHED_TARGET, REACHED_TARGET };

struct LimbOrientation {
public:
    float m_fYaw{0.0f};
    float m_fPitch{0.0f};

    LimbOrientation() = default;
    LimbOrientation(float yaw, float pitch) : m_fYaw(yaw), m_fPitch(pitch) { }
};
VALIDATE_SIZE(LimbOrientation, 0x8);

struct LimbMovementInfo {
    float maxYaw, minYaw;
    float yawD;
    float maxPitch, minPitch;
    float pitchD;
};
VALIDATE_SIZE(LimbMovementInfo, 0x18);

class CPedIK {
public:
    CPed* m_pPed{nullptr};
    LimbOrientation m_TorsoOrient{};
    float m_fSlopePitch{0.0f};
    float m_fSlopePitchLimitMult{0.0f};
    float m_fSlopeRoll{0.0f};
    float m_fBodyRoll{0.0f};

    union {
        uint32 m_nFlags{0};
        struct {
            bool bGunReachedTarget : 1;
            bool bTorsoUsed : 1;
            bool bUseArm : 1;
            bool bSlopePitch : 1;
            bool bUnk : 1;
        };
    };

    static inline const CVector XaxisIK = CVector(1.0f, 0.0f, 0.0f); // 0x8D232C
    static inline const CVector YaxisIK = CVector(0.0f, 1.0f, 0.0f); // 0x8D2338
    static inline const CVector ZaxisIK = CVector(0.0f, 0.0f, 1.0f); // 0x8D2344

    static inline LimbMovementInfo ms_torsoInfo = { DEGTORAD(50.0f), DEGTORAD(-50.0f), DEGTORAD(15.0f), DEGTORAD(45.0f), DEGTORAD(-45.0f), DEGTORAD(7.0f) };
    static inline LimbMovementInfo ms_headInfo = { DEGTORAD(90.0f), DEGTORAD(-90.0f), DEGTORAD(10.0f), DEGTORAD(45.0f), DEGTORAD(-45.0f), DEGTORAD(5.0f) };
    static inline LimbMovementInfo ms_headRestoreInfo = { DEGTORAD(90.0f), DEGTORAD(-90.0f), DEGTORAD(10.0f), DEGTORAD(45.0f), DEGTORAD(-45.0f), DEGTORAD(5.0f) };
    static inline LimbMovementInfo ms_upperArmInfo = { DEGTORAD(20.0f), DEGTORAD(-100.0f), DEGTORAD(20.0f), DEGTORAD(70.0f), DEGTORAD(-70.0f), DEGTORAD(10.0f) };
    static inline LimbMovementInfo ms_lowerArmInfo = { DEGTORAD(80.0f), DEGTORAD(0.0f), DEGTORAD(20.0f), DEGTORAD(90.0f), DEGTORAD(-90.0f), DEGTORAD(5.0f) };

public:
    static void InjectHooks();

//    CPedIK* Constructor(CPed* ped) {
//        CPedIK(ped);
//        return this;
//    }

public:
    CPedIK() = default; // NOTSA
    explicit CPedIK(CPed* ped) : m_pPed(ped) {} // 0x5FD8C0

    void RotateTorso(AnimBlendFrameData* bone, LimbOrientation& orientation, bool flag);
    void RotateTorsoForArm(const CVector& direction);
    bool PointGunInDirection(float zAngle, float distance, bool flag, float normalize);
    void PointGunAtPosition(const CVector& posn, float normalize);
    void PitchForSlope();
    MoveLimbResult MoveLimb(LimbOrientation& limb, float targetYaw, float targetPitch, LimbMovementInfo& moveInfo);
    MoveLimbResult MoveLimb(LimbOrientation& limb, float targetYaw, float targetPitch, LimbMovementInfo& moveInfo, float normalize);

    static RwMatrixTag* GetWorldMatrix(RwFrame* frame, RwMatrixTag* transformMat);
};
VALIDATE_SIZE(CPedIK, (VER_x32 ? 0x20 : 0x28));
