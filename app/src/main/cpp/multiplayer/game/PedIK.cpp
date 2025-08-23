/*
    Plugin-SDK file
    Authors: GTA Community. See more here
    https://github.com/DK22Pac/plugin-sdk
    Do not delete this comment block. Respect others' work!
*/

#include "PedIK.h"
#include "Animation/AnimBlendFrameData.h"
#include "Entity/Ped/Ped.h"
#include "game/Enums/ePedBones.h"
#include "RwHelper.h"
#include "game/Plugins/RpAnimBlendPlugin/RpAnimBlend.h"
#include "util/patch.h"

void CPedIK::InjectHooks() {

}

// 0x5FDDB0
void CPedIK::RotateTorso(AnimBlendFrameData* bone, LimbOrientation& orientation, bool changeRoll) {
    const auto quat = bone->m_pIFrame->orientation.AsRtQuat();
    RtQuatRotate(quat, &XaxisIK, RadiansToDegrees(orientation.m_fYaw), rwCOMBINEREPLACE);
    RtQuatRotate(quat, &ZaxisIK, RadiansToDegrees(orientation.m_fPitch), rwCOMBINEPRECONCAT);
    m_pPed->bDontAcceptIKLookAts = true;
}

// 0x5FDF90
void CPedIK::RotateTorsoForArm(const CVector& direction) {
    auto destRotation = CVector2D{direction - m_pPed->GetPosition()}.Heading();

    // this might be inlined
    if (m_pPed->m_fCurrentRotation + DegreesToRadians(180.0f) < destRotation) {
        destRotation -= DegreesToRadians(360.0f);
    } else if (m_pPed->m_fCurrentRotation - DegreesToRadians(180.0f) > destRotation) {
        destRotation += DegreesToRadians(360.0f);
    }

    const auto difAngle = destRotation - m_pPed->m_fCurrentRotation;
    auto resultAngle = DegreesToRadians(45.0f);

    if (difAngle > DegreesToRadians(45.0f) && difAngle <= DegreesToRadians(90.0f)) {
        resultAngle = difAngle - DegreesToRadians(45.0f);
    } else {
        if (difAngle >= DegreesToRadians(-60.0f))
            return;

        resultAngle = difAngle + DegreesToRadians(60.0f);

        if (difAngle < DegreesToRadians(-80.0f)) {
            resultAngle = DegreesToRadians(-20.0f);
        }
    }

//    if (resultAngle != DegreesToRadians(0.0f)) {
//        const auto degreesHalf = RadiansToDegrees(resultAngle / 2.0f);
//        if (bRotateWithNeck) { // android doesn't have this check
//            RtQuatRotate(m_pPed->m_apBones[PED_NODE_NECK]->m_pIFrame->orientation.AsRtQuat(), &XaxisIK, degreesHalf, rwCOMBINEPOSTCONCAT);
//        }
//        RtQuatRotate(m_pPed->m_apBones[PED_NODE_UPPER_TORSO]->m_pIFrame->orientation.AsRtQuat(), &XaxisIK, degreesHalf, rwCOMBINEPOSTCONCAT);
//    }
}

// 0x5FDC00
bool CPedIK::PointGunInDirection(float zAngle, float distance, bool flag, float normalize) {
    bGunReachedTarget = false;
    bTorsoUsed = true;

    const auto angle = CGeneral::LimitRadianAngle(zAngle - m_pPed->m_fCurrentRotation);
    const auto hier  = GetAnimHierarchyFromSkinClump(m_pPed->m_pRwClump);
    const auto index = RpHAnimIDGetIndex(hier, m_pPed->m_apBones[PED_NODE_RIGHT_CLAVICLE]->m_nNodeId);

    // unused code
    // auto* boneMatrix = RwMatrixCreate();
    // *boneMatrix = RpHAnimHierarchyGetMatrixArray(hier)[index];
    // RwMatrixDestroy(boneMatrix);

    const auto limbResult = [&]() -> MoveLimbResult {
        if (normalize < 0.0f) {
            return MoveLimb(m_TorsoOrient, angle, distance, ms_torsoInfo);
        } else {
            return MoveLimb(m_TorsoOrient, angle, distance, ms_torsoInfo, normalize);
        }
    }();

    bool canReach = true;
    switch (limbResult) {
        case CANT_REACH_TARGET:
            canReach = false;
            break;
        case REACHED_TARGET:
            bGunReachedTarget = true;
            break;
    }

    const auto head = &RpHAnimHierarchyGetMatrixArray(hier)[PED_NODE_HEAD];
    const auto headAngle = -CGeneral::LimitRadianAngle(atan2(-head->at.y, -head->at.x) - m_pPed->m_fCurrentRotation);

    const auto axis = CVector{
            0.0f,
            flag ? std::cos(headAngle) : -std::sin(headAngle),
            flag ? std::sin(headAngle) :  std::cos(headAngle)
    };

    auto Torsoframe = m_pPed->m_apBones[PED_NODE_UPPER_TORSO];
    RtQuatRotate(Torsoframe->GetFrameOrientation().AsRtQuat(), &axis, RadiansToDegrees(m_TorsoOrient.m_fPitch), rwCOMBINEPOSTCONCAT);
    RtQuatRotate(Torsoframe->GetFrameOrientation().AsRtQuat(), &XaxisIK, RadiansToDegrees(m_TorsoOrient.m_fYaw), rwCOMBINEPOSTCONCAT);
    m_pPed->bUpdateMatricesRequired = true;

    return canReach;
}

void CPedIK::PointGunAtPosition(const CVector& posn, float normalize) {
    CHook::CallFunction<void>(g_libGTASA + (VER_x32 ? 0x004B90BC + 1 : 0x5B45A0), this, posn, normalize);
}

void CPedIK::PitchForSlope() {
    CHook::CallFunction<void>(g_libGTASA + (VER_x32 ? 0x004B9758 + 1 : 0x5B4C1C), this);

}

// unused
RwMatrixTag* CPedIK::GetWorldMatrix(RwFrame* frame, RwMatrixTag* transformMat) {
    *transformMat = *RwFrameGetMatrix(frame);
    for (frame = RwFrameGetParent(frame); frame; frame = RwFrameGetParent(frame)) {
        RwMatrixTransform(transformMat, RwFrameGetMatrix(frame), rwCOMBINEPOSTCONCAT);
    }
    return transformMat;
}

MoveLimbResult CPedIK::MoveLimb(LimbOrientation& limb, float targetYaw, float targetPitch, LimbMovementInfo& moveInfo) {
    auto result = HAVENT_REACHED_TARGET;

    // yaw
    if (std::abs(limb.m_fYaw - targetYaw) < moveInfo.yawD) {
        limb.m_fYaw = targetYaw;
        result = REACHED_TARGET;
    } else {
        if (limb.m_fYaw > targetYaw) {
            limb.m_fYaw -= moveInfo.yawD;
        } else if (limb.m_fYaw < targetYaw) {
            limb.m_fYaw += moveInfo.yawD;
        }
    }

    if (limb.m_fYaw > moveInfo.maxYaw || limb.m_fYaw < moveInfo.minYaw) {
        limb.m_fYaw = std::clamp(limb.m_fYaw, moveInfo.minYaw, moveInfo.maxYaw);
        result = CANT_REACH_TARGET;
    }

    // pitch
    if (std::abs(limb.m_fPitch - targetPitch) < moveInfo.pitchD) {
        limb.m_fPitch = targetPitch;
    } else {
        if (limb.m_fPitch > targetPitch) {
            limb.m_fPitch -= moveInfo.pitchD;
        } else if (limb.m_fPitch < targetPitch) {
            limb.m_fPitch += moveInfo.pitchD;
        }
        result = HAVENT_REACHED_TARGET;
    }

    if (limb.m_fPitch > moveInfo.maxPitch || limb.m_fPitch < moveInfo.minPitch) {
        limb.m_fPitch = std::clamp(limb.m_fPitch, moveInfo.minPitch, moveInfo.maxPitch);
        result = CANT_REACH_TARGET;
    }

    return result;
}

// 0x5FDB60
MoveLimbResult CPedIK::MoveLimb(LimbOrientation& limb, float targetYaw, float targetPitch, LimbMovementInfo& moveInfo, float normalize) {
    auto result = HAVENT_REACHED_TARGET;

    limb.m_fYaw = normalize * targetYaw;
    limb.m_fPitch = normalize * targetPitch;

    if (limb.m_fYaw > moveInfo.maxYaw || limb.m_fYaw < moveInfo.minYaw) {
        limb.m_fYaw = std::clamp(limb.m_fYaw, moveInfo.minYaw, moveInfo.maxYaw);
        result = CANT_REACH_TARGET;
    }

    if (limb.m_fPitch > moveInfo.maxPitch || limb.m_fPitch < moveInfo.minPitch) {
        limb.m_fPitch = std::clamp(limb.m_fPitch, moveInfo.minPitch, moveInfo.maxPitch);
        result = CANT_REACH_TARGET;
    } else if (normalize > 0.9f && result == HAVENT_REACHED_TARGET) {
        result = REACHED_TARGET;
    }

    return result;
}
