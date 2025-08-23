//
// Created by Traw-GG on 09.08.2024.
//
#include "EntryExitManager.h"
#include "util/patch.h"
#include "World.h"

void CEntryExitManager::Init() {
    mp_QuadTree = new CQuadTreeNode(WORLD_BOUNDS, 4);

    ms_exitEnterState = 0;
    ms_bDisabled = false;
    ms_entryExitStackPosn = 0;
    ms_bBurglaryHousesEnabled = false;

    mp_poolEntryExits = new CEntryExitsPool(400, "Entry exits");
}

void CEntryExitManager::InjectHooks() {
    CHook::Write(g_libGTASA + (VER_x32 ? 0x677FE4 : 0x84DFF8), &mp_poolEntryExits);
    CHook::Write(g_libGTASA + (VER_x32 ? 0x677808 : 0x84D048), &mp_QuadTree);

    CHook::Write(g_libGTASA + (VER_x32 ? 0x679334 : 0x850690), &ms_bBurglaryHousesEnabled);
    CHook::Write(g_libGTASA + (VER_x32 ? 0x6794DC : 0x8509E0), &ms_entryExitStackPosn);
    CHook::Write(g_libGTASA + (VER_x32 ? 0x676D0C : 0x84BA78), &ms_bDisabled);
    CHook::Write(g_libGTASA + (VER_x32 ? 0x678614 : 0x84EC50), &ms_exitEnterState);

    CHook::Redirect("_ZN17CEntryExitManager4InitEv", &Init);
}