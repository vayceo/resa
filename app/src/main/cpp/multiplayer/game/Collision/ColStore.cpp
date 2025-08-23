//
// Created on 01.07.2023.
//

#include "ColStore.h"
#include "util/patch.h"
#include "game/constants.h"

void CColStore::RequestCollision(const CVector *pos, int32 areaCode) {

}

void CColStore::RemoveCol(int32 colSlot)
{
   CHook::CallFunction<void>(g_libGTASA + (VER_x32 ? 0x002E25A0 + 1 : 0x3A5688), colSlot);
}

void CColStore::AddCollisionNeededAtPosn(const CVector *pos) {
   CHook::CallFunction<void>(g_libGTASA + (VER_x32 ? 0x2E2750 + 1 : 0x3A5904), pos);
}

void CColStore::LoadCollision(CVector pos, bool bIgnorePlayerVeh)
{
   CHook::CallFunction<void>(g_libGTASA + (VER_x32 ? 0x2E291C + 1 : 0x3A5AFC), pos, bIgnorePlayerVeh);

//    if (CStreaming::ms_disableStreaming)
//        return;
//
//    auto* player = FindPlayerPed();
//    if (player && !bIgnorePlayerVeh)
//    {
//        if (auto* vehicle = FindPlayerVehicle()) {
//            pos += vehicle->m_vecMoveSpeed * CVector(20.0F, 20.0F, 0.0F);
//        }
//    }
//
//    SetCollisionRequired(pos, -1);
//    if (ms_bCollisionNeeded)
//        SetCollisionRequired(ms_vecCollisionNeeded, CGame::currArea);
//
////    for (auto& obj : CTheScripts::MissionCleanUp.m_Objects)
////    {
////        CEntity* entity = nullptr;
////        if (obj.type == MissionCleanUpEntityType::MISSION_CLEANUP_ENTITY_TYPE_VEHICLE)
////        {
////            entity = GetVehiclePool()->GetAtRef(obj.handle);
////            if (!entity || entity->m_nStatus == eEntityStatus::STATUS_WRECKED)
////                continue;
////        }
////        else if (obj.type == MissionCleanUpEntityType::MISSION_CLEANUP_ENTITY_TYPE_PED)
////        {
////            entity = GetPedPool()->GetAtRef(obj.handle);
////            if (!entity || entity->AsPed()->m_nPedState == PEDSTATE_DIE || entity->AsPed()->m_nPedState == PEDSTATE_DEAD)
////                continue;
////        }
////
////        if (!entity || entity->AsPhysical()->physicalFlags.b15 || entity->AsPhysical()->physicalFlags.bDontApplySpeed)
////            continue;
////
////        ms_nRequiredCollisionArea = entity->m_nAreaCode;
////        ms_pQuadTree->ForAllMatching(entity->GetPosition(), SetIfCollisionIsRequiredReducedBB);
////    }
//
//    for (auto i = 1; i < ms_pColPool->GetSize(); i++) {
//        auto* def = ms_pColPool->GetAt(i);
//        if (!def)
//            continue;
//
//        if (def->m_bCollisionIsRequired || def->m_bProcedural || def->m_nRefCount > 0)
//        {
//            if (!def->m_bActive)
//                CStreaming::RequestModel(COLToModelId(i), STREAMING_PRIORITY_REQUEST | STREAMING_KEEP_IN_MEMORY);
//
//            def->m_bCollisionIsRequired = false;
//        }
//        else if (def->m_bActive)
//            CStreaming::RemoveModel(COLToModelId(i));
//    }
//
//    if (!bIgnorePlayerVeh)
//        ms_bCollisionNeeded = false;
}

void CColStore::EnsureCollisionIsInMemory(const CVector* pos)
{
   CHook::CallFunction<void>(g_libGTASA + (VER_x32 ? 0x2E2C50 + 1 : 0x3A5EA4), pos);
}

int32 CColStore::AddColSlot(const char* name)
{
    return CHook::CallFunction<int32>("_ZN9CColStore10AddColSlotEPKc", name);
}

void CColStore::Initialise()
{
    //CHook::CallFunction<void>(g_libGTASA + (VER_x32 ? 0x2E20B0 + 1 : 0x3A50C8));

    bool& ms_bCollisionNeeded = *(bool*)(g_libGTASA + (VER_x32 ? 0x796124 : 0x9760B8));

    const auto rect = CRect(-3000.0F, -3000.0F, 3000.0F, 3000.0F);
    ms_bCollisionNeeded = false;
    if (!ms_pColPool)
        ms_pColPool = new CColPool(TOTAL_COL_MODEL_IDS, "CollisionFiles");

    AddColSlot("generic");
    ms_pQuadTree = new CQuadTreeNode(rect, 3);
}

void CColStore::InjectHooks() {
    CHook::Write(g_libGTASA + (VER_x32 ? 0x679658 : 0x850CD0), &ms_pColPool);
    CHook::Write(g_libGTASA + (VER_x32 ? 0x679F48 : 0x851EA0), &ms_pQuadTree);
}
