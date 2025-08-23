//
// Created by Traw-GG on 09.08.2024.
//
#pragma once

#include "common.h"
#include "Rect.h"
#include "Vector.h"
#include "Enums/eAreaCodes.h"

class CObject;
class CPed;
class CPlayerPed;
class CEntryExit;

class CEntryExit {
public:
    enum eFlags {
        UNKNOWN_INTERIOR       = 1 << 0,
        UNKNOWN_PAIRING        = 1 << 1,
        CREATE_LINKED_PAIR     = 1 << 2,
        REWARD_INTERIOR        = 1 << 3,
        USED_REWARD_ENTRANCE   = 1 << 4,
        CARS_AND_AIRCRAFT      = 1 << 5,
        BIKES_AND_MOTORCYCLES  = 1 << 6,
        DISABLE_ON_FOOT        = 1 << 7,

        ACCEPT_NPC_GROUP       = 1 << 8,
        FOOD_DATE_FLAG         = 1 << 9,
        UNKNOWN_BURGLARY       = 1 << 10,
        DISABLE_EXIT           = 1 << 11,
        BURGLARY_ACCESS        = 1 << 12,
        ENTERED_WITHOUT_EXIT   = 1 << 13,
        ENABLE_ACCESS          = 1 << 14,
        DELETE_ENEX            = 1 << 15,
    };

    char    m_szName[8]{};
    CRect   m_recEntrance{};
    float   m_fEntranceZ{};
    float   m_fEntranceAngleRad;
    CVector m_vecExitPos{};
    float   m_fExitAngle{};
    union {
        struct {
            uint16 bUnknownInterior : 1;     // 0x1
            uint16 bUnknownPairing : 1;      // 0x2
            uint16 bCreateLinkedPair : 1;    // 0x4
            uint16 bRewardInterior : 1;      // 0x8
            uint16 bUsedRewardEntrance : 1;  // 0x10
            uint16 bCarsAndAircraft : 1;     // 0x20
            uint16 bBikesAndMotorcycles : 1; // 0x40
            uint16 bDisableOnFoot : 1;       // 0x80

            uint16 bAcceptNpcGroup : 1;      // 0x100
            uint16 bFoodDateFlag : 1;        // 0x200
            uint16 bUnknownBurglary : 1;     // 0x400
            uint16 bDisableExit : 1;         // 0x800
            uint16 bBurglaryAccess : 1;      // 0x1000
            uint16 bEnteredWithoutExit : 1;  // 0x2000
            uint16 bEnableAccess : 1;        // 0x4000
            uint16 bDeleteEnex : 1;          // 0x8000
        };
        uint16 m_nFlags{};
    };
    eAreaCodes  m_nArea{};
    uint8       m_nSkyColor{};
    uint8       m_nTimeOn{};
    uint8       m_nTimeOff{};
    uint8       m_nNumberOfPeds{};
    CEntryExit* m_pLink{};

    static bool& ms_bWarping;
    static CObject*& ms_pDoor;
    static CEntryExit*& ms_spawnPoint;

public:
    static void InjectHooks();

    void GetPositionRelativeToOutsideWorld(CVector& outPos);

    auto GetLinkedOrThis() { return m_pLink ? m_pLink : this; }
    auto GetLinkedOrThis() const { return m_pLink ? m_pLink : this; }

    [[nodiscard]] CVector GetPosition() const;
};
VALIDATE_SIZE(CEntryExit, (VER_x32 ? 0x3C : 0x40));
