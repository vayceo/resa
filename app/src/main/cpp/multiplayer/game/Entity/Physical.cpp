//
// Created on 20.04.2023.
//
#include "../common.h"
#include "Physical.h"
#include "game/Timer.h"
#include "util/patch.h"
#include "../World.h"
#include "game/RepeatSector.h"
#include "game/Models/ModelInfo.h"

void CPhysical::ApplyTurnSpeed()
{
    if (physicalFlags.bDontApplySpeed) {
        ResetTurnSpeed();
    }
    else
    {
        CVector vecTurnSpeedTimeStep = CTimer::GetTimeStep() * m_vecTurnSpeed;
        CVector vecCrossProduct;
        CrossProduct(&vecCrossProduct, &vecTurnSpeedTimeStep, &GetRight());
        GetRight() += vecCrossProduct;
        CrossProduct(&vecCrossProduct, &vecTurnSpeedTimeStep, &GetForward());
        GetForward() += vecCrossProduct;
        CrossProduct(&vecCrossProduct, &vecTurnSpeedTimeStep, &GetUp());
        GetUp() += vecCrossProduct;
        if (!physicalFlags.bInfiniteMass && !physicalFlags.bDisableMoveForce) {
            CVector vecNegativeCentreOfMass = m_vecCentreOfMass * -1.0f;
            CVector vecCentreOfMassMultiplied = Multiply3x3(GetMatrix(), vecNegativeCentreOfMass);
            GetPosition() += CrossProduct(vecTurnSpeedTimeStep, vecCentreOfMassMultiplied);
        }
    }
}

void CPhysical::ApplyMoveForce(float x, float y, float z)
{
    return ApplyMoveForce(CVector(x, y ,z));
}

bool CPhysical::IsAdded()
{
    if(this)
    {
        if(*(uintptr*)this == g_libGTASA + (VER_x32 ? 0x00667D14 : 0x830098)) // CPlaceable
            return false;

        if(m_pMovingList)
            return true;
    }

    return false;
}

// 0x5429F0
void CPhysical::ApplyMoveForce(CVector force)
{
    if (!physicalFlags.bInfiniteMass && !physicalFlags.bDisableMoveForce) {
        if (physicalFlags.bDisableZ)
            force.z = 0.0f;
        m_vecMoveSpeed += force / m_fMass;
    }
}

void CPhysical::ApplyTurnForce(CVector force, CVector point)
{
    if (!physicalFlags.bDisableTurnForce)
    {
        CVector vecCentreOfMassMultiplied{};
        if (!physicalFlags.bInfiniteMass)
            vecCentreOfMassMultiplied = Multiply3x3(GetMatrix(), m_vecCentreOfMass);

        if (physicalFlags.bDisableMoveForce) {
            point.z = 0.0f;
            force.z = 0.0f;
        }
        CVector vecDifference = point - vecCentreOfMassMultiplied;
        m_vecTurnSpeed += CrossProduct(vecDifference, force) / m_fTurnMass;
    }
}

void CPhysical::ApplyMoveSpeed()
{
    if (physicalFlags.bDontApplySpeed || physicalFlags.bDisableMoveForce)
        ResetMoveSpeed();
    else
        GetPosition() += CTimer::GetTimeStep() * m_vecMoveSpeed;
}

bool CPhysical::ApplySpringCollisionAlt(float fSuspensionForceLevel, CVector& direction, CVector& collisionPoint, float fSpringLength, float fSuspensionBias, CVector& normal, float& fSpringForceDampingLimit) {
#ifdef USE_DEFAULT_FUNCTIONS
    return ((bool(__thiscall*)(CPhysical*, float, CVector&, CVector&, float, float, CVector&, float&))0x543D60)(this, fSuspensionForceLevel, direction, collisionPoint, fSpringLength, fSuspensionBias, normal, fSpringForceDampingLimit);
#else
    float fSpringStress = 1.0f - fSpringLength;
    if (fSpringStress <= 0.0f)
        return true;
    if (DotProduct(direction, normal) > 0.0f)
        normal *= -1.0f;
    float fTimeStep = CTimer::ms_fTimeStep;
    if (CTimer::ms_fTimeStep >= 3.0f)
        fTimeStep = 3.0f;
    fSpringForceDampingLimit = fSpringStress * (fTimeStep * m_fMass) * fSuspensionForceLevel * fSuspensionBias * 0.016f;
    if (physicalFlags.bMakeMassTwiceAsBig)
        fSpringForceDampingLimit *= 0.75f;
    ApplyForce(fSpringForceDampingLimit * normal, collisionPoint, true);
    return true;
#endif
}

bool CPhysical::ApplySpringDampening(float fDampingForce, float fSpringForceDampingLimit, CVector& direction, CVector& collisionPoint, CVector& collisionPos) {
#ifdef USE_DEFAULT_FUNCTIONS
    return ((bool(__thiscall*)(CPhysical*, float, float, CVector&, CVector&, CVector&))0x543E90)(this, fDampingForce, fSpringForceDampingLimit, direction, collisionPoint, collisionPos);
#else
    float fCollisionPosDotProduct = DotProduct(collisionPos, direction);
    CVector vecCollisionPointSpeed = GetSpeed(collisionPoint);
    float fCollisionPointSpeedDotProduct = DotProduct(vecCollisionPointSpeed, direction);
    float fTimeStep = CTimer::ms_fTimeStep;
    if (CTimer::ms_fTimeStep >= 3.0f)
        fTimeStep = 3.0f;
    float fDampingForceTimeStep = fTimeStep * fDampingForce;
    if (physicalFlags.bMakeMassTwiceAsBig)
        fDampingForceTimeStep *= 2.0f;
    fDampingForceTimeStep = clamp<float>(fDampingForceTimeStep, -DAMPING_LIMIT_IN_FRAME, DAMPING_LIMIT_IN_FRAME);
    float fDampingSpeed = -(fDampingForceTimeStep * fCollisionPosDotProduct);
    if (fDampingSpeed > 0.0 && fDampingSpeed + fCollisionPointSpeedDotProduct > 0.0f) {
        if (fCollisionPointSpeedDotProduct >= 0.0f)
            fDampingSpeed = 0.0f;
        else
            fDampingSpeed = -fCollisionPointSpeedDotProduct;
    }
    else if (fDampingSpeed < 0.0f && fDampingSpeed + fCollisionPointSpeedDotProduct < 0.0f) {
        if (fCollisionPointSpeedDotProduct <= 0.0f)
            fDampingSpeed = 0.0f;
        else
            fDampingSpeed = -fCollisionPointSpeedDotProduct;
    }

    CVector center = Multiply3x3(GetMatrix(), m_vecCentreOfMass);
    CVector distance = collisionPoint - center;
    float fSpringForceDamping = GetMass(distance, direction) * fDampingSpeed;
    fSpringForceDampingLimit = fabs(fSpringForceDampingLimit) * DAMPING_LIMIT_OF_SPRING_FORCE;
    if (fSpringForceDamping > fSpringForceDampingLimit)
        fSpringForceDamping = fSpringForceDampingLimit;
    ApplyForce(fSpringForceDamping * direction, collisionPoint, true);
    return true;
#endif
}

void CPhysical::ApplyForce(CVector vecForce, CVector point, bool bUpdateTurnSpeed)
{
#ifdef USE_DEFAULT_FUNCTIONS
    ((void(__thiscall*)(CPhysical*, CVector, CVector, bool))0x542B50)(this, vecForce, point, bUpdateTurnSpeed);
#else
    CVector vecMoveSpeedForce = vecForce;
    if (physicalFlags.bDisableZ)
        vecMoveSpeedForce.z = 0.0f;
    if (!physicalFlags.bInfiniteMass && !physicalFlags.bDisableMoveForce)
        m_vecMoveSpeed += vecMoveSpeedForce / m_fMass;

    if (!physicalFlags.bDisableTurnForce && bUpdateTurnSpeed) {
        CVector vecCentreOfMassMultiplied;
        float fTurnMass = m_fTurnMass;
        if (physicalFlags.bInfiniteMass)
            fTurnMass += m_vecCentreOfMass.z * m_fMass * m_vecCentreOfMass.z * 0.5f;
        else
            vecCentreOfMassMultiplied = Multiply3x3(GetMatrix(), m_vecCentreOfMass);

        if (physicalFlags.bDisableMoveForce) {
            point.z = 0.0f;
            vecForce.z = 0.0f;
        }

        CVector distance = point - vecCentreOfMassMultiplied;
        m_vecTurnSpeed += CrossProduct(distance, vecForce) / fTurnMass;
    }
#endif
}

CVector CPhysical::GetSpeed(CVector point)
{
#ifdef USE_DEFAULT_FUNCTIONS
    CVector outSpeed;
    ((CVector *(__thiscall*)(CPhysical*, CVector*, CVector))0x542CE0)(this, outSpeed, point);
    return outSpeed;
#else
    CVector vecCentreOfMassMultiplied;
    if (!physicalFlags.bInfiniteMass)
        vecCentreOfMassMultiplied = Multiply3x3(GetMatrix(), m_vecCentreOfMass);

    CVector distance = point - vecCentreOfMassMultiplied;
    CVector vecTurnSpeed = m_vecTurnSpeed + m_vecFrictionTurnSpeed;
    CVector speed = CrossProduct(vecTurnSpeed, distance);
    speed += m_vecMoveSpeed + m_vecFrictionMoveSpeed;
    return speed;
#endif
}

void CPhysical::ApplyFrictionForce(CVector vecMoveForce, CVector point)
{
    CVector vecTheMoveForce = vecMoveForce;

    if (physicalFlags.bDisableZ)
    {
        vecTheMoveForce.z = 0.0f;
    }

    if (!physicalFlags.bInfiniteMass && !physicalFlags.bDisableMoveForce)
    {
        m_vecFrictionMoveSpeed += vecTheMoveForce / m_fMass;
    }

    CVector vecCentreOfMassMultiplied{};
    if (!physicalFlags.bDisableTurnForce)
    {
        float fTurnMass = m_fTurnMass;
        if (physicalFlags.bInfiniteMass)
            fTurnMass += m_vecCentreOfMass.z * m_fMass * m_vecCentreOfMass.z * 0.5f;
        else
            vecCentreOfMassMultiplied = Multiply3x3(GetMatrix(), m_vecCentreOfMass);

        if (physicalFlags.bDisableMoveForce)
        {
            point.z = 0.0f;
            vecMoveForce.z = 0.0f;
        }

        CVector vecDifference = point - vecCentreOfMassMultiplied;
        CVector vecMoveForceCrossProduct = CrossProduct(vecDifference, vecMoveForce);

        m_vecFrictionTurnSpeed += vecMoveForceCrossProduct / fTurnMass;
    }
}

void CPhysical::ApplyFrictionMoveForce(CVector moveForce)
{
    if (!physicalFlags.bInfiniteMass && !physicalFlags.bDisableMoveForce)
    {
        if (physicalFlags.bDisableZ)
        {
            moveForce.z = 0.0f;
        }
        m_vecFrictionMoveSpeed += moveForce / m_fMass;
    }
}

bool CPhysical::ApplyFriction(CPhysical* entity, float fFriction, CColPoint& colPoint)
{
    if (physicalFlags.bDisableTurnForce && entity->physicalFlags.bDisableTurnForce)
    {
        float fThisSpeedDotProduct = DotProduct(&m_vecMoveSpeed, &colPoint.m_vecNormal);
        float fEntitySpeedDotProduct = DotProduct(&entity->m_vecMoveSpeed, &colPoint.m_vecNormal);

        CVector vecThisSpeedDifference = m_vecMoveSpeed - (fThisSpeedDotProduct * colPoint.m_vecNormal);
        CVector vecEntitySpeedDifference = entity->m_vecMoveSpeed - (fEntitySpeedDotProduct * colPoint.m_vecNormal);

        float fThisSpeedMagnitude = vecThisSpeedDifference.Magnitude();
        float fEntitySpeedMagnitude = vecEntitySpeedDifference.Magnitude();

        float fEntityMass = entity->m_fMass;
        float fThisMass = m_fMass;

        CVector vecMoveDirection = vecThisSpeedDifference / fThisSpeedMagnitude;
        float fSpeed = (fEntityMass * fEntitySpeedMagnitude + fThisMass * fThisSpeedMagnitude) / (fEntityMass + fThisMass);
        if (fThisSpeedMagnitude > fSpeed) {
            float fThisSpeed = fThisMass * (fSpeed - fThisSpeedMagnitude);
            float fEntitySpeed = fEntityMass * (fSpeed - fEntitySpeedMagnitude);
            float fFrictionTimeStep = -(CTimer::GetTimeStep() * fFriction);
            if (fThisSpeed < fFrictionTimeStep)
            {
                fThisSpeed = fFrictionTimeStep;
            }

            // BUG: Both if conditions are the same.
            if (fThisSpeed < fFrictionTimeStep)
            {
                fThisSpeed = fFrictionTimeStep;
            }

            ApplyFrictionMoveForce(vecMoveDirection * fThisSpeed);
            entity->ApplyFrictionMoveForce(vecMoveDirection * fEntitySpeed);
            return true;
        }
        return false;
    }

    if (physicalFlags.bDisableTurnForce)
    {
        if (entity->IsVehicle())
        {
            return false;
        }

        CVector vecDistanceToPoint = colPoint.m_vecPoint - entity->GetPosition();
        CVector vecEntitySpeed = entity->GetSpeed(vecDistanceToPoint);

        float fThisSpeedDotProduct = DotProduct(&m_vecMoveSpeed, &colPoint.m_vecNormal);
        float fEntitySpeedDotProduct = DotProduct(&vecEntitySpeed, &colPoint.m_vecNormal);

        CVector vecThisSpeedDifference = m_vecMoveSpeed - (fThisSpeedDotProduct * colPoint.m_vecNormal);
        CVector vecEntitySpeedDifference = vecEntitySpeed - (fEntitySpeedDotProduct * colPoint.m_vecNormal);

        float fThisSpeedMagnitude = vecThisSpeedDifference.Magnitude();
        float fEntitySpeedMagnitude = vecEntitySpeedDifference.Magnitude();

        CVector vecMoveDirection = vecThisSpeedDifference * (1.0f / fThisSpeedMagnitude);
        CVector vecEntityCentreOfMassMultiplied = Multiply3x3(entity->GetMatrix(), entity->m_vecCentreOfMass);
        CVector vecEntityDifference = vecDistanceToPoint - vecEntityCentreOfMassMultiplied;
        CVector vecEntitySpeedCrossProduct = CrossProduct(vecEntityDifference, vecMoveDirection);
        float squaredMagnitude = vecEntitySpeedCrossProduct.SquaredMagnitude();
        float fEntityCollisionMass = 1.0f / ((squaredMagnitude) / entity->m_fTurnMass + 1.0f / entity->m_fMass);
        float fThisMass = m_fMass;
        float fSpeed = (fEntitySpeedMagnitude * fEntityCollisionMass + fThisMass * fThisSpeedMagnitude) / (fEntityCollisionMass + fThisMass);
        if (fThisSpeedMagnitude > fSpeed) {
            float fThisSpeed = fThisMass * (fSpeed - fThisSpeedMagnitude);
            float fEntitySpeed = fEntityCollisionMass * (fSpeed - fEntitySpeedMagnitude);
            float fFrictionTimeStep = CTimer::GetTimeStep() * fFriction;
            float fFrictionTimeStepNegative = -fFrictionTimeStep;
            if (fThisSpeed < fFrictionTimeStepNegative)
            {
                fThisSpeed = fFrictionTimeStepNegative;
            }

            if (fEntitySpeed > fFrictionTimeStep)
            {
                fEntitySpeed = fFrictionTimeStep;
            }

            ApplyFrictionMoveForce(vecMoveDirection * fThisSpeed);
            if (!entity->physicalFlags.bDisableCollisionForce)
            {
                entity->ApplyFrictionForce(vecMoveDirection * fEntitySpeed, vecDistanceToPoint);
                return true;
            }
            return true;
        }
        return false;
    }

    if (!entity->physicalFlags.bDisableTurnForce)
    {
        CVector vecDistanceToPointFromThis = colPoint.m_vecPoint - GetPosition();
        CVector vecThisSpeed = GetSpeed(vecDistanceToPointFromThis);

        CVector vecDistanceToPoint = colPoint.m_vecPoint - entity->GetPosition();
        CVector vecEntitySpeed = entity->GetSpeed(vecDistanceToPoint);

        float fThisSpeedDotProduct = DotProduct(&vecThisSpeed, &colPoint.m_vecNormal);
        float fEntitySpeedDotProduct = DotProduct(&vecEntitySpeed, &colPoint.m_vecNormal);
        if (0.2f * 0.707f > fabs(fThisSpeedDotProduct))
        {
            fFriction = 0.05f * fFriction;
        }

        CVector vecThisSpeedDifference = vecThisSpeed - (fThisSpeedDotProduct * colPoint.m_vecNormal);
        CVector vecEntitySpeedDifference = vecEntitySpeed - (fEntitySpeedDotProduct * colPoint.m_vecNormal);

        float fThisSpeedMagnitude = vecThisSpeedDifference.Magnitude();
        float fEntitySpeedMagnitude = vecEntitySpeedDifference.Magnitude();

        CVector vecMoveDirection = vecThisSpeedDifference * (1.0f / fThisSpeedMagnitude);

        CVector vecThisCentreOfMassMultiplied = Multiply3x3(GetMatrix(), m_vecCentreOfMass);

        CVector vecThisDifference = vecDistanceToPointFromThis - vecThisCentreOfMassMultiplied;
        CVector vecThisSpeedCrossProduct = CrossProduct(vecThisDifference, vecMoveDirection);
        float squaredMagnitude = vecThisSpeedCrossProduct.SquaredMagnitude();
        float fThisCollisionMass = 1.0f / (squaredMagnitude / m_fTurnMass + 1.0f / m_fMass);

        CVector vecEntityCentreOfMassMultiplied = Multiply3x3(entity->GetMatrix(), entity->m_vecCentreOfMass);

        CVector vecEntityDifference = vecDistanceToPoint - vecEntityCentreOfMassMultiplied;
        CVector vecEntitySpeedCrossProduct = CrossProduct(vecEntityDifference, vecMoveDirection);
        squaredMagnitude = vecEntitySpeedCrossProduct.SquaredMagnitude();
        float fEntityCollisionMass = 1.0f / (squaredMagnitude / entity->m_fTurnMass + 1.0f / entity->m_fMass);
        float fSpeed = (fEntitySpeedMagnitude * fEntityCollisionMass + fThisCollisionMass * fThisSpeedMagnitude) / (fEntityCollisionMass + fThisCollisionMass);
        if (fThisSpeedMagnitude > fSpeed) {
            float fThisSpeed = fThisCollisionMass * (fSpeed - fThisSpeedMagnitude);
            float fEntitySpeed = fEntityCollisionMass * (fSpeed - fEntitySpeedMagnitude);
            float fNegativeFriction = -fFriction;
            if (fThisSpeed < fNegativeFriction) {
                fThisSpeed = fNegativeFriction;
            }

            if (fEntitySpeed > fFriction) {
                fEntitySpeed = fFriction;
            }

            if (!physicalFlags.bDisableCollisionForce) {
                ApplyFrictionForce(vecMoveDirection * fThisSpeed, vecDistanceToPointFromThis);
            }

            if (!entity->physicalFlags.bDisableCollisionForce) {
                entity->ApplyFrictionForce(vecMoveDirection * fEntitySpeed, vecDistanceToPoint);
            }
            return true;
        }
        return false;
    }

    if (IsVehicle())
    {
        return false;
    }

    CVector vecDistanceToPointFromThis = colPoint.m_vecPoint - GetPosition();
    CVector vecThisSpeed = GetSpeed(vecDistanceToPointFromThis);

    float fThisSpeedDotProduct = DotProduct(&vecThisSpeed, &colPoint.m_vecNormal);
    float fEntitySpeedDotProduct = DotProduct(&entity->m_vecMoveSpeed, &colPoint.m_vecNormal);

    CVector vecThisSpeedDifference = vecThisSpeed - (fThisSpeedDotProduct * colPoint.m_vecNormal);
    CVector vecEntitySpeedDifference = entity->m_vecMoveSpeed - (fEntitySpeedDotProduct * colPoint.m_vecNormal);

    float fThisSpeedMagnitude = vecThisSpeedDifference.Magnitude();
    float fEntitySpeedMagnitude = vecEntitySpeedDifference.Magnitude();

    CVector vecMoveDirection = vecThisSpeedDifference * (1.0f / fThisSpeedMagnitude);

    CVector vecThisCentreOfMassMultiplied = Multiply3x3(GetMatrix(), m_vecCentreOfMass);

    CVector vecThisDifference = vecDistanceToPointFromThis - vecThisCentreOfMassMultiplied;
    CVector vecThisSpeedCrossProduct = CrossProduct(vecThisDifference, vecMoveDirection);
    float squaredMagnitude = vecThisSpeedCrossProduct.SquaredMagnitude();
    float fEntityMass = entity->m_fMass;
    float fThisCollisionMass = 1.0f / (squaredMagnitude / m_fTurnMass + 1.0f / m_fMass);
    float fSpeed = (fEntityMass * fEntitySpeedMagnitude + fThisCollisionMass * fThisSpeedMagnitude) / (fEntityMass + fThisCollisionMass);
    if (fThisSpeedMagnitude > fSpeed) {
        float fThisSpeed = (fSpeed - fThisSpeedMagnitude) * fThisCollisionMass;
        float fEntitySpeed = (fSpeed - fEntitySpeedMagnitude) * fEntityMass;
        float fFrictionTimeStep = CTimer::GetTimeStep() * fFriction;
        float fNegativeFrictionTimeStep = -fFrictionTimeStep;
        if (fThisSpeed < fNegativeFrictionTimeStep) {
            fThisSpeed = fNegativeFrictionTimeStep;
        }

        if (fEntitySpeed > fFrictionTimeStep) {
            fEntitySpeed = fFrictionTimeStep;
        }

        if (!physicalFlags.bDisableCollisionForce) {
            ApplyFrictionForce(vecMoveDirection * fThisSpeed, vecDistanceToPointFromThis);
        }

        entity->ApplyFrictionMoveForce(vecMoveDirection * fEntitySpeed);
        return true;
    }
    return false;
}

// --------------------- hooks

bool ApplyFriction4__hook(CPhysical* thiz, CPhysical* entity, float fFriction, CColPoint& colPoint) {
    return thiz->ApplyFriction(entity, fFriction, colPoint);
}

void ApplyFrictionForce_hook(CPhysical* thiz, CVector vecMoveForce, CVector point) {
    thiz->ApplyFrictionForce(vecMoveForce, point);
}

void ApplySpringCollisionAlt_hook(CPhysical* thiz, float fSuspensionForceLevel, CVector& direction, CVector& collisionPoint, float fSpringLength, float fSuspensionBias, CVector& normal, float& fSpringForceDampingLimit) {
    thiz->ApplySpringCollisionAlt(fSuspensionForceLevel, direction, collisionPoint, fSpringLength, fSuspensionBias, normal, fSpringForceDampingLimit);
}
void ApplyTurnForce_hook(CPhysical* thiz, CVector force, CVector point) {
    thiz->ApplyTurnForce(force, point);
}
void ApplyForce_hook(CPhysical* thiz, CVector vecForce, CVector point, bool bUpdateTurnSpeed) {
    thiz->ApplyForce(vecForce, point, bUpdateTurnSpeed);
}

void ApplySpringDampening_hook(CPhysical* thiz, float fDampingForce, float fSpringForceDampingLimit, CVector& direction, CVector& collisionPoint, CVector& collisionPos) {
    thiz->ApplySpringDampening(fDampingForce, fSpringForceDampingLimit, direction, collisionPoint, collisionPos);
}
void CPhysical::InjectHooks() {

}

CPhysical::CPhysical() : CEntity()
{
    m_pCollisionList.m_pNode = nullptr;

    CPlaceable::AllocateStaticMatrix();
    m_matrix->SetUnity();

    ResetMoveSpeed();
    ResetTurnSpeed();
    m_vecFrictionMoveSpeed.Set(0.0f, 0.0f, 0.0f);
    m_vecFrictionTurnSpeed.Set(0.0f, 0.0f, 0.0f);
    m_vecForce.Set(0.0f, 0.0f, 0.0f);
    m_vecTorque.Set(0.0f, 0.0f, 0.0f);

    m_fMass = 1.0f;
    m_fTurnMass = 1.0f;
    m_fVelocityFrequency = 1.0f;
    m_fAirResistance = 0.1f;
    m_pMovingList = nullptr;
    m_nFakePhysics = 0;
    m_nNumEntitiesCollided = 0;
    std::fill(std::begin(m_apCollidedEntities), std::end(m_apCollidedEntities), nullptr);

    m_nPieceType = 0;

    m_fDamageIntensity = 0.0f;
    m_pDamageEntity = nullptr;

    m_vecLastCollisionImpactVelocity.Set(0.0f, 0.0f, 0.0f);
    m_vecLastCollisionPosn.Set(0.0f, 0.0f, 0.0f);

    m_bUsesCollision = true;

    m_vecCentreOfMass.Set(0.0f, 0.0f, 0.0f);

    m_fMovingSpeed = 0.0f;
    m_pAttachedTo = nullptr;
    m_pEntityIgnoredCollision = nullptr;

    m_qAttachedEntityRotation = CQuaternion(0.0f, 0.0f, 0.0f, 0.0f);

    m_fDynamicLighting = 0.0f;
    m_pShadowData = nullptr;
    m_fPrevDistFromCam = 100.0f;

    m_nPhysicalFlags = 0;
    physicalFlags.bApplyGravity = true;

    m_nContactSurface = SURFACE_DEFAULT; // NOTSA; If not initialize, it will go out of bounds in the SurfaceInfos_c
    m_fContactSurfaceBrightness = 1.0f;
}

CPhysical::~CPhysical() {
//    if (m_pShadowData)
//        g_realTimeShadowMan.ReturnRealTimeShadow(m_pShadowData);

    m_pCollisionList.Flush();
}

void CPhysical::Add() {
    CHook::CallFunction<void>(g_libGTASA + (VER_x32 ? 0x3FCE3C + 1 : 0x4E0608), this);
}

void CPhysical::Remove() {
    CHook::CallFunction<void>(g_libGTASA + (VER_x32 ? 0x3FD02C + 1 : 0x4E07EC), this);
}
