//
// Created on 15.11.2023.
//

#include "IplStore.h"
#include "../util/patch.h"
#include "constants.h"
#include "World.h"

void CIplStore::LoadIpls(CVector posn, bool bAvoidLoadInPlayerVehicleMovingDirection) {
    CHook::CallFunction<void>(g_libGTASA + (VER_x32 ? 0x28195C + 1 : 0x33CA20), posn, bAvoidLoadInPlayerVehicleMovingDirection);
}

void CIplStore::EnsureIplsAreInMemory(const CVector *posn) {
    CHook::CallFunction<void>(g_libGTASA + (VER_x32 ? 0x281BB8 + 1 : 0x33CD08), posn);
}

void CIplStore::AddIplsNeededAtPosn(const CVector *posn) {
    CHook::CallFunction<void>(g_libGTASA + (VER_x32 ? 0x2811F4 + 1 : 0x33C140), posn);
}

void CIplStore::RemoveIpl(int32 iplSlotIndex) {
    CHook::CallFunction<void>(g_libGTASA + (VER_x32 ? 0x280758 + 1 : 0x33B320), iplSlotIndex);
}

/*!
 * @returns Slot index
 */
int32 CIplStore::AddIplSlot(const char* name) {
    const auto def = new (ms_pPool->New()) IplDef{ name }; // MOTE: Seemingly they didn't overload `operator new` for it..
    return ms_pPool->GetIndex(def);
}

/*!
 * @brief Allocate pool if it doesn't exist yet, allocate quad tree, etc..
 */
void CIplStore::Initialise() {
    bool& gbIplsNeededAtPosn = *(bool*)(g_libGTASA + (VER_x32 ? 0x6DFDA4 : 0x8BE3CC));
    int32& NumIplEntityIndexArrays = *(int32*)(g_libGTASA + (VER_x32 ? 0x6DFDA8 : 0x8BE3D0));
    CEntity**& ppCurrIplInstance = *(CEntity***)(g_libGTASA + (VER_x32 ? 0x6DFDAC : 0x8BE3D8));

    gbIplsNeededAtPosn = false;

    if (!ms_pPool) {
        ms_pPool = new CIplPool(TOTAL_IPL_MODEL_IDS, "IPL Files");
    }

    AddIplSlot("generic");

    NumIplEntityIndexArrays = 0;
    ppCurrIplInstance = nullptr;

    ms_pQuadTree = new CQuadTreeNode(WORLD_BOUNDS, 3);
    assert(ms_pQuadTree);
}

void CIplStore::InjectHooks() {
    CHook::Write(g_libGTASA + (VER_x32 ? 0x6770AC : 0x84C1A8), &ms_pPool);
    CHook::Write(g_libGTASA + (VER_x32 ? 0x678D0C : 0x84FA48), &ms_pQuadTree);

    CHook::Redirect("_ZN9CIplStore10InitialiseEv", &Initialise);
}
