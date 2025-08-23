#pragma once

#include "util/patch.h"
#include "Vector.h"

class CVehicleSamp;

enum eDoorState : uint8 {
    DOOR_NOTHING,
    DOOR_HIT_MAX_END,
    DOOR_HIT_MIN_END,
    DOOR_POP_OPEN,
    DOOR_SLAM_SHUT
};

class CDoor {
public:
    float      m_fOpenAngle;
    float      m_fClosedAngle;
    int16      m_nDirn; // Def a bitfield. 2 x 4 bits + some more
    uint8      m_nAxis; // X, Y, Z - Right/left, front/back, up/down
    eDoorState m_nDoorState;
    float      m_fAngle;
    float      m_fPrevAngle;
    float      m_fAngVel;    // Angular velocity

    static float& DOOR_SPEED_MAX_CAPPED; // 0.5f

public:
    bool  Process(CVehicleSamp* vehicle, CVector& arg1, CVector& arg2, Const CVector& arg3);
    bool  ProcessImpact(CVehicleSamp* vehicle, CVector& arg1, CVector& arg2, Const CVector& arg3);
    void  Open(float openRatio);
    float GetAngleOpenRatio();
    bool  IsClosed();
    bool  IsFullyOpen();

    // NOTSA
    void    OpenFully()  { m_fAngle = m_fOpenAngle; }
    void    CloseFully() { m_fAngle = m_fClosedAngle; }
    CVector GetRotation() const;
    void    UpdateFrameMatrix(CMatrix& mat);
};

VALIDATE_SIZE(CDoor, 0x18);
