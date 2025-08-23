#include "common.h"
#include "CarEnterExit.h"
#include "game/Animation/AnimManager.h"
#include "game/Entity/Vehicle/Vehicle.h"
#include "game/Enums/eTargetDoor.h"
#include "util/patch.h"
#include "Entity/Ped/Ped.h"
#include "scripting.h"

/*
const float& CCarEnterExit::ms_fMaxSpeed_CanDragPedOut = *(float*)0x0;
const float& CCarEnterExit::ms_fMaxSpeed_PlayerCanDragPedOut = *(float*)0x0;
*/
int32 CCarEnterExit::ComputeTargetDoorToEnterAsPassenger(const CVehicle* vehicle, int32 psgrIdx) {
    if (vehicle->m_nVehicleFlags.bIsBus) {
        return 8;
    }

    switch (psgrIdx) {
        case 0:
            return (vehicle->IsBike() || vehicle->m_pHandlingData->m_bTandemSeats) ? 11 : 8; // Inverted condition
        case 1:
            return 11;
        case 2:
            return 9;
        default:
            return -1;
    }
}



bool CCarEnterExit::SetPedInCarDirect(CPed* ped, CVehicle* vehicle, int32 doorId, bool bAsDriver) {
    // ����


    return CHook::CallFunction<bool>(g_libGTASA + (VER_x32 ? 0x0050AAC8 + 1 : 0x61B044), ped, vehicle, doorId, bAsDriver);
//    if (bAsDriver) {
//        // Warp ped into vehicle
//        CTaskSimpleCarSetPedInAsDriver task{ vehicle };
//        task.m_bWarpingInToCar = true;
//        task.ProcessPed(ped);
//
////        // And make them drive
////        ped->GetTaskManager().SetTask(new CTaskComplexCarDriveWander{ vehicle, vehicle->m_autoPilot.m_nCarDrivingStyle, (float)vehicle->m_autoPilot.m_nCruiseSpeed }, TASK_PRIMARY_PRIMARY);
//
//        return true;
//    }
//    // Warp ped into vehicle
//    {
//        CTaskSimpleCarSetPedInAsPassenger task{ vehicle, (eTargetDoor)doorId };
//        task.m_bWarpingInToCar = true;
//        task.ProcessPed(ped);
//    }
////
////    if (vehicle->IsBike()) {
////      //  ped->GetTaskManager().SetTask(new CTaskComplexCarDrive{ vehicle, false }, TASK_PRIMARY_PRIMARY);
////    }
//
////    // Set mutal acquaintance respect between the ped and all other occupants up to the ped's seat
////    // I assume the function is only ever called with `bAsDriver` if there are no passengers
////    // So that's why this code-path is only reachable if `bAsDriver` is false
////
////    const auto SetMutalAcquaintanceWith = [ped](CPed* other) {
////        if (other) {
////            const auto SetWith = [](CPed* of, CPed* with) {
////                if (!of->IsCreatedByMission()) {
////                    of->GetAcquaintance().SetAsAcquaintance(ACQUAINTANCE_RESPECT, CPedType::GetPedFlag(with->m_nPedType));
////                }
////            };
////            SetWith(ped, other);
////            SetWith(other, ped);
////        }
////    };
//
////    SetMutalAcquaintanceWith(vehicle->m_pDriver);
////
////    const auto psgrIdx = ComputePassengerIndexFromCarDoor(vehicle, doorId);
////    assert(psgrIdx != -1); // I really doubt this can happen, if it does, an `if` has to be added
////    rng::for_each(vehicle->GetPassengers() | rng::views::take((size_t)psgrIdx), SetMutalAcquaintanceWith); // Set with all other passengers up to the ped's seat

    return true;
}

void CCarEnterExit::InjectHooks() {
   // CHook::Redirect(g_libGTASA + 0x00494FE4, &CCarEnterExit::SetPedInCarDirect);
}
