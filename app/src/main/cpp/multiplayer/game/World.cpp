//
// Created on 13.05.2023.
//

#include "World.h"
#include "../util/patch.h"
#include "net/netgame.h"

//CRepeatSector CWorld::ms_aRepeatSectors[MAX_REPEAT_SECTORS_Y][MAX_REPEAT_SECTORS_X];
//CPtrListDoubleLink CWorld::ms_listMovingEntityPtrs;

CSector* GetSector(int32 x, int32 y) {
    static CSector(&ms_aSectors)[MAX_SECTORS_Y][MAX_SECTORS_X] = *(CSector(*)[MAX_SECTORS_Y][MAX_SECTORS_X])(g_libGTASA + (VER_x32 ? 0x96B9F4 : 0xBDCB20));

    const auto x1 = std::clamp<int32>(x, 0, MAX_SECTORS_X - 1);
    const auto y1 = std::clamp<int32>(y, 0, MAX_SECTORS_Y - 1);

    return &ms_aSectors[y1][x1];
}

CRepeatSector* GetRepeatSector(int32 x, int32 y) {
    static CRepeatSector(&ms_aRepeatSectors)[MAX_REPEAT_SECTORS_Y][MAX_REPEAT_SECTORS_X] = *(CRepeatSector(*)[MAX_REPEAT_SECTORS_Y][MAX_REPEAT_SECTORS_X])(g_libGTASA + (VER_x32 ? 0x987BF4 : 0xC14F20));

    return &ms_aRepeatSectors[y % MAX_REPEAT_SECTORS_Y][x % MAX_REPEAT_SECTORS_X];
}

void CWorld::InjectHooks() {

    CHook::Write(g_libGTASA + (VER_x32 ? 0x006761FC : 0x84A478), &bIncludeBikers);
    CHook::Write(g_libGTASA + (VER_x32 ? 0x006765D4 : 0x84AC18), &bIncludeCarTyres);
    CHook::Write(g_libGTASA + (VER_x32 ? 0x00675F9C : 0x849FC0), &bIncludeDeadPeds);
    CHook::Write(g_libGTASA + (VER_x32 ? 0x00676698 : 0x84ADA0), &ms_nCurrentScanCode);

    CHook::InstallPLT(g_libGTASA + (VER_x32 ? 0x675C58 : 0x849A20), &ProcessPedsAfterPreRender);
}

bool CWorld::ProcessLineOfSight(const CVector* origin, const CVector* target, CColPoint* outColPoint, CEntity** outEntity, bool buildings, bool vehicles, bool peds, bool objects, bool dummies, bool doSeeThroughCheck, bool doCameraIgnoreCheck, bool doShootThroughCheck) {
   // assert(!origin.HasNanOrInf() && !target.HasNanOrInf()); // We're getting random nan/inf's from somewhere, so let's try to root cause it...
    return CHook::CallFunction<bool>(g_libGTASA + (VER_x32 ? 0x424B94 + 1 : 0x508C7C), origin, target, outColPoint, outEntity, buildings, vehicles, peds, objects, dummies, doSeeThroughCheck, doCameraIgnoreCheck, doShootThroughCheck);
}
float CWorld::FindGroundZForCoord(float x, float y) {
    return CHook::CallFunction<float>(g_libGTASA + (VER_x32 ? 0x0042A7C4 + 1 : 0x50F3A0), x, y);
}

bool CWorld::GetIsLineOfSightClear(const CVector& origin, const CVector& target, bool buildings, bool vehicles, bool peds, bool objects, bool dummies, bool doSeeThroughCheck, bool doCameraIgnoreCheck) {
    return CHook::CallFunction<bool>(g_libGTASA + (VER_x32 ? 0x00423468 + 1 : 0x5075A4), &origin, &target, buildings, vehicles, peds, objects, dummies, doSeeThroughCheck, doCameraIgnoreCheck);
}

void CWorld::Add(CEntity *entity) {
    ((void(*)(CEntity*))(g_libGTASA + (VER_x32 ? 0x00423418 + 1 : 0x507518)))(entity);
}

void CWorld::Remove(CEntity *entity) {
    ((void (*)(CEntity*))(g_libGTASA + (VER_x32 ? 0x0042330C + 1 : 0x5073A0))) (entity);
}

void CWorld::ProcessPedsAfterPreRender() {
    if (CTimer::bSkipProcessThisFrame)
        return;

    for(auto &pair : CPlayerPool::spawnedPlayers) {
        auto pPed = pair.second->GetPlayerPed();

        if (!pPed->m_pPed->m_bRemoveFromWorld && !pPed->m_pPed->m_bOffscreen) {
            pPed->ProcessAttach();
            pPed->m_pPed->GetIntelligence()->ProcessAfterPreRender();
        }
    }

    if(auto pLocalPlayer = CLocalPlayer::GetPlayerPed()) {
        pLocalPlayer->m_pPed->GetIntelligence()->ProcessAfterPreRender();
        pLocalPlayer->ProcessAttach();
    }

    for(auto &pair : CActorPool::list) {
        auto pPed = pair.second->m_pPed;
        if (!pPed->m_bRemoveFromWorld) {
            pPed->GetIntelligence()->ProcessAfterPreRender();
        }
    }
}

void CWorld::ClearScanCodes() {
    const auto ProcessList = [](const CPtrList& list) {
        for (CPtrNode* node = list.GetNode(); node; node = node->GetNext()) {
            static_cast<CEntity*>(node->m_item)->m_nScanCode = 0;
        }
    };

    for (auto y = 0; y < MAX_SECTORS_Y; y++) {
        for (auto x = 0; x < MAX_SECTORS_X; x++) {
            const auto& sector = *GetSector(x, y);
            ProcessList(sector.m_buildings);
            ProcessList(sector.m_dummies);
        }
    }

    for (auto y = 0; y < MAX_REPEAT_SECTORS_Y; y++) {
        for (auto x = 0; x < MAX_REPEAT_SECTORS_X; x++) {
            for (const auto& list : GetRepeatSector(x, y)->m_lists) {
                ProcessList(list);
            }
        }
    }
}

void CWorld::IncrementCurrentScanCode() {
    if (ms_nCurrentScanCode >= 65535u) {
        ClearScanCodes();
        ms_nCurrentScanCode = 1;
    } else {
        ms_nCurrentScanCode++;
    }
}
