//
// Created on 14.04.2023.
//

#include "cHandlingDataMgr.h"
#include "CFileMgr.h"
#include "main.h"
#include "VehicleNames.h"
#include "FileLoader.h"
#include "util/patch.h"
#include <vector>
#include <fstream>

//std::vector<tHandlingData> cHandlingDataMgr::m_aHandlingData;
//std::vector<tBikeHandlingData> cHandlingDataMgr::m_aBikeHandlingData;
//std::vector<tFlyingHandlingData> cHandlingDataMgr::m_aFlyingHandlingData;
//std::vector<tBoatHandlingData> cHandlingDataMgr::m_aBoatHandlingData;

void cHandlingDataMgr::LoadHandlingData()
{
    const auto pFile = CFileMgr::OpenFile("SAMP/handling.cfg", "rb");

   // char line[500];
    while (CFileLoader::LoadLine(pFile))
    {
        if (strlen(CFileLoader::ms_line) == 0 || CFileLoader::ms_line[0] == ';' || CFileLoader::ms_line[0] == '\r') {
            // Пропустить комментарии и пустые строки
            continue;
        }
        char name[32]{};
        if (sscanf(CFileLoader::ms_line, "%31s", name, std::size(name)) != 1) { // FIX_BUGS: Sized string read
            return;
        }
        const auto id = FindExactWord(name, &VehicleNames[0][0], std::size(VehicleNames[0]), std::size(VehicleNames));
        if (id == -1) {
            return; // Issue logged by `FindExactWord`, so no need to care about it here
        }
        switch (CFileLoader::ms_line[0]) {
            case ';': {
                break; // Comment
            }
            case '!': {
                // bike
                tBikeHandlingData d{};
                d.InitFromData(id, CFileLoader::ms_line);

                m_aBikeHandling[id] = d;
                break;
            }
            case '$': {
                tFlyingHandlingData d{};
                d.InitFromData(id, CFileLoader::ms_line);

                m_aFlyingHandling[id] = d;
                // flying
                break;
            }
            case '%': {
                // boat
                tBoatHandlingData d{};
                d.InitFromData(id, CFileLoader::ms_line);

                m_aBoatHandling[id] = d;
                break;
            }
            default: {
                tHandlingData d{};
                d.InitFromData(id, CFileLoader::ms_line);

                m_aVehicleHandling[id] = d;
                break;
            }
        }
    }
    CFileMgr::CloseFile(pFile);
}

int32 cHandlingDataMgr::FindExactWord(const char* name, const char* nameTable, uint32 entrySize, uint32 entryCount) {
    for (auto i = 0u; i < entryCount; i++) {
        const auto entry = &nameTable[entrySize * i];
        if (!strncmp(name, entry, strlen(entry))) {
            return i;
        }
    }
    Log("Vehicle name not found in table: %s", name);
    return -1;
}

int32 tHandlingData::InitFromData(int32 id, const char* line) {
    m_nVehicleId = id;
    Log("InitFromData veh %d, line = %s", id, line);
    const auto n = sscanf(
            line,
            "%*s %f %f %f%f\t%f\t%f\t%hhu\t%f\t%f\t%f\t%hhu\t%f\t%f\t%f\t%c\t%c\t%f\t%f\t%hhu\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%d\t%x\t%x\t%hhu\t%hhu\t%hhu",
            &m_fMass, // 1
            &m_fTurnMass,
            &m_fDragMult,

            &m_vecCentreOfMass.x, // 4
            &m_vecCentreOfMass.y,
            &m_vecCentreOfMass.z,

            &m_nPercentSubmerged, // 7

            &m_fTractionMultiplier, // 8
            &m_fTractionLoss,
            &m_fTractionBias,

            &m_transmissionData.m_nNumberOfGears, // 11
            &m_transmissionData.m_fMaxGearVelocity,
            &m_transmissionData.m_fEngineAcceleration,
            &m_transmissionData.m_fEngineInertia,
            &m_transmissionData.m_nDriveType,
            &m_transmissionData.m_nEngineType,

            &m_fBrakeDeceleration, // 18
            &m_fBrakeBias,
            &m_bABS,
            &m_fSteeringLock,
            &m_fSuspensionForceLevel,

            &m_fSuspensionDampingLevel, // 22
            &m_fSuspensionHighSpdComDamp,
            &m_fSuspensionUpperLimit,
            &m_fSuspensionLowerLimit,
            &m_fSuspensionBiasBetweenFrontAndRear,
            &m_fSuspensionAntiDiveMultiplier,

            &m_fSeatOffsetDistance,
            &m_fCollisionDamageMultiplier,

            &m_nMonetaryValue, // 31
            &m_nModelFlags,
            &m_nHandlingFlags,
            &m_nFrontLights,
            &m_nRearLights,
            &m_nAnimGroup
    );
    m_transmissionData.m_handlingFlags = m_nHandlingFlags;
    m_transmissionData.m_fEngineAcceleration *= 0.4f;
    cHandlingDataMgr::ConvertDataToGameUnits(this);
    return n == 35 ? -1 : n;
}

int32 tBoatHandlingData::InitFromData(int32 id, const char* line) {
    m_nVehicleId = id;

    const auto n = sscanf(
            line,
            "%*s\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f",
            &m_fThrustY,
            &m_fThrustZ,
            &m_fThrustAppZ,
            &m_fAqPlaneForce,
            &m_fAqPlaneLimit,
            &m_fAqPlaneOffset,
            &m_fWaveAudioMult,

            &m_vecMoveRes.x,
            &m_vecMoveRes.y,
            &m_vecMoveRes.z,

            &m_vecTurnRes.x,
            &m_vecTurnRes.y,
            &m_vecTurnRes.z,

            &m_fLookLRBehindCamHeight
    );
    return n == 14 ? -1 : n;
}

int32 tFlyingHandlingData::InitFromData(int32 id, const char* line) {
    m_nVehicleId = id;

    const auto n = sscanf(
            line,
            "%*s\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f%*c\t%f\t%f\t%f\t%f\t%f\t%f\t%f",
            &m_fThrust, // 1                                            ^^^ there's an extra `s` for the `RCRAIDER` that we have to ignore
            &m_fThrustFallOff,
            &m_fYaw,
            &m_fYawStab,
            &m_fSideSlip, // 5
            &m_fRoll,
            &m_fRollStab,
            &m_fPitch,
            &m_fPitchStab,
            &m_fFormLift, // 10
            &m_fAttackLift,
            &m_fGearUpR,
            &m_fGearDownL,
            &m_fWindMult,
            &m_fMoveRes, // 15

            &m_vecTurnRes.x, // 16
            &m_vecTurnRes.y,
            &m_vecTurnRes.z,

            &m_vecSpeedRes.x, // 19
            &m_vecSpeedRes.y,
            &m_vecSpeedRes.z
    );
    return n == 21 ? -1 : n;
}

int32 tBikeHandlingData::InitFromData(int32 id, const char* line) {
    m_nVehicleId = id;

    const auto n = sscanf(
            line,
            "%*s\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f",
            &m_fLeanFwdCOM,
            &m_fLeanFwdForce,
            &m_fLeanBakCOM,
            &m_fLeanBakForce,
            &m_fMaxLean,
            &m_fFullAnimLean,
            &m_fDesLean,
            &m_fSpeedSteer,
            &m_fSlipSteer,
            &m_fNoPlayerCOMz,
            &m_fWheelieAng,
            &m_fStoppieAng,
            &m_fWheelieSteer,
            &m_fWheelieStabMult,
            &m_fStoppieStabMult
    );
   // gHandlingDataMgr.ConvertBikeDataToGameUnits(this);
    return n == 15 ? -1 : n;
}

void cHandlingDataMgr::ConvertDataToGameUnits(tHandlingData* h) {
    const auto t = &h->GetTransmission();

    t->m_fEngineAcceleration *= ACCEL_CONST;
    t->m_fMaxGearVelocity *= VELOCITY_CONST;
    h->m_fBrakeDeceleration *= ACCEL_CONST;
    h->m_fMassRecpr = 1.f / h->m_fMass;
    h->m_fBuoyancyConstant = h->m_fMass * 0.8f / (float)h->m_nPercentSubmerged;
    h->m_fCollisionDamageMultiplier *= h->m_fMassRecpr * 2000.f;

    auto maxVelocity{ t->m_fMaxGearVelocity };
    while (maxVelocity > 0.f) {
        maxVelocity -= 0.01f;

        const auto maxVelocitySq = maxVelocity * maxVelocity;
        const auto v = h->m_fDragMult >= 0.01f
                       ? h->m_fDragMult / 1000.f * 0.5f * maxVelocitySq
                       : -((1.f / (maxVelocitySq * h->m_fDragMult + 1.f) - 1.f) * maxVelocity);
        if (t->m_fEngineAcceleration / 6.f < v) {
            break;
        }
    }

    std::tie(t->m_fMaxVelocity, t->m_maxReverseGearVelocity) = [&]() -> std::tuple<float, float> {
        if (h->m_nVehicleId == 38) { // RC Bandit
            return { t->m_fMaxGearVelocity, -t->m_fMaxGearVelocity };
        }

        if (h->m_bUseMaxspLimit) {
            const auto v = t->m_fMaxGearVelocity / 1.2f;
            return { v, std::min(-0.2f, v * -0.25f) };
        }

        t->m_fMaxGearVelocity = maxVelocity * 1.2f;

        if (h->m_nVehicleId >= 162 && h->m_nVehicleId <= 174) {
            return { maxVelocity, -0.05f };
        } else {
            return { maxVelocity, std::min(-0.2f, maxVelocity * -0.3f) };
        }
    }();

    t->m_fEngineAcceleration /= (t->m_nDriveType == '4') ? 4.f : 2.f;

    t->InitGearRatios();
}

// get handling id by name
int32 cHandlingDataMgr::GetHandlingId(const char* nameToFind) {

    for (int i = 0; i < std::size(VehicleNames); i++) {
        if (strcmp(VehicleNames[i], nameToFind) == 0) {
            return i;
        }
    }
    DLOG("Can't find handling %s", nameToFind);
    return 0;
}

tFlyingHandlingData* cHandlingDataMgr::GetFlyingPointer(uint8 handlingId) {
    auto it = m_aFlyingHandling.find(handlingId);
    return it != m_aFlyingHandling.end() ? &it->second : nullptr;
}

// 0x6F5300
tBoatHandlingData* cHandlingDataMgr::GetBoatPointer(uint8 handlingId) {
    auto it = m_aBoatHandling.find(handlingId);
    return it != m_aBoatHandling.end() ? &it->second : nullptr;
}

tHandlingData* cHandlingDataMgr::GetVehiclePointer(uint32 handlingId) {
    auto it = m_aVehicleHandling.find(handlingId);
    return it != m_aVehicleHandling.end() ? &it->second : nullptr;
}

tBikeHandlingData* cHandlingDataMgr::GetBikeHandlingPointer(uint32 handlingId) {
    auto it = m_aBikeHandling.find(handlingId);
    return it != m_aBikeHandling.end() ? &it->second : nullptr;
}

int32 GetHandlingId_hooked(uintptr* thiz, const char* nameToFind) {
    return cHandlingDataMgr::GetHandlingId(nameToFind);
}

void cHandlingDataMgr::InjectHooks() {
    CHook::Redirect("_ZN16cHandlingDataMgr13GetHandlingIdEPKc", &GetHandlingId_hooked);
//    CHook::Redirect("_ZN16cHandlingDataMgr16LoadHandlingDataEv", &LoadHandlingData);
//    CHook::Redirect("_ZN16cHandlingDataMgr14GetBoatPointerEh", &GetBoatPointer);
//    CHook::Redirect("_ZN16cHandlingDataMgr16GetFlyingPointerEh", &GetFlyingPointer);
}
