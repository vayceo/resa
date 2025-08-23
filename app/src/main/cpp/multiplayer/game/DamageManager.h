//
// Created on 23.07.2023.
//

#pragma once

#include <cmath>
#include "common.h"
#include "Enums/eCarWheel.h"
#include "Enums/eCarNodes.h"

class CAutomobile;

enum eCarWheelStatus : uint8_t {
    WHEEL_STATUS_OK = 0,
    WHEEL_STATUS_BURST,
    WHEEL_STATUS_MISSING,
    WHEEL_STATUS_3,
    WHEEL_STATUS_4
};

enum ePanelDamageState : uint8_t {
    DAMSTATE_OK             = 0, // Closed
    DAMSTATE_OPENED         = 1, // Open
    DAMSTATE_DAMAGED        = 2, // Closed
    DAMSTATE_OPENED_DAMAGED = 3, // Open
    DAMSTATE_NOTPRESENT     = 4  // Missing
};

enum eLightsDamageState : uint8_t {
    VEHICLE_LIGHT_OK      = 0x0,
    VEHICLE_LIGHT_SMASHED = 0x1
};

enum ePanels : uint8 {
    FRONT_LEFT_PANEL = 0,
    FRONT_RIGHT_PANEL,
    REAR_LEFT_PANEL,
    REAR_RIGHT_PANEL,
    WINDSCREEN_PANEL,
    FRONT_BUMPER,
    REAR_BUMPER,

    MAX_PANELS
};

enum tComponent : uint8_t {
    COMPONENT_NA         = 0,
    COMPONENT_WHEEL_LF   = 1,
    COMPONENT_WHEEL_RF   = 2,
    COMPONENT_WHEEL_LR   = 3,
    COMPONENT_WHEEL_RR   = 4,
    COMPONENT_BONNET     = 5,
    COMPONENT_BOOT       = 6,
    COMPONENT_DOOR_LF    = 7,
    COMPONENT_DOOR_RF    = 8,
    COMPONENT_DOOR_LR    = 9,
    COMPONENT_DOOR_RR    = 10,
    COMPONENT_WING_LF    = 11,
    COMPONENT_WING_RF    = 12,
    COMPONENT_WING_LR    = 13,
    COMPONENT_WING_RR    = 14,
    COMPONENT_WINDSCREEN = 15,
    COMPONENT_BUMP_FRONT = 16,
    COMPONENT_BUMP_REAR  = 17,

    MAX_COMPONENTS = COMPONENT_BUMP_REAR /* cause it starts at 1, not 0 */
};

enum eDoors : uint8  {
    DOOR_BONNET = 0,
    DOOR_BOOT,
    DOOR_LEFT_FRONT,
    DOOR_RIGHT_FRONT,
    DOOR_LEFT_REAR,
    DOOR_RIGHT_REAR,

    MAX_DOORS
};

enum tComponentGroup : uint8 {
    COMPGROUP_PANEL  = 0,
    COMPGROUP_WHEEL  = 1,
    COMPGROUP_DOOR   = 2,
    COMPGROUP_BONNET = 3,
    COMPGROUP_BOOT   = 4,
    COMPGROUP_LIGHT  = 5,
    COMPGROUP_NA     = 6,

    MAX_COMGROUPS
};
using eDoorStatus = ePanelDamageState;

class CDamageManager {
public:
    float           m_fWheelDamageEffect;
    uint8_t         m_nEngineStatus; // 0 - 250
    eCarWheelStatus m_anWheelsStatus[eCarWheel::MAX_CARWHEELS];
    eDoorStatus     m_aDoorsStatus[eDoors::MAX_DOORS];
    union {
        /* Great job R*, really could've just used an array here... Ends up to be the same size... */

        uint32_t m_nLightsStatus;
        struct {
            uint32_t leftFront : 2;
            uint32_t rightFront : 2;
            uint32_t rightRear : 2;
            uint32_t leftRear : 2;
        } m_lightStates;
    };
    union {
        uint32_t m_nPanelsStatus; // Index with ePanels
        struct {
            uint32_t frontLeft : 4;
            uint32_t frontRight : 4;
            uint32_t rearLeft : 4;
            uint32_t rearRight : 4;
            uint32_t windscreen : 4;
            uint32_t frontBumper : 4;
            uint32_t rearBumper : 4;
        } m_panelStates;
        struct {
            /* unknown, bitfields of 2 */
        } m_aeroplanePanelStates;
    };

public:
    static void InjectHooks();

    CDamageManager(float wheelDamageEffect);

    void Init();
    void ResetDamageStatus();
    void ResetDamageStatusAndWheelDamage();
    void FuckCarCompletely(bool bDetachWheel);
    bool ApplyDamage(CAutomobile* vehicle, tComponent compId, float fIntensity, float fColDmgMult);

    // Set next level of damage to aero-plane component
    bool ProgressAeroplaneDamage(uint8 nFrameId);
    bool ProgressWheelDamage(eCarWheel wheel);
    bool ProgressPanelDamage(ePanels panel);
    void ProgressEngineDamage();
    bool ProgressDoorDamage(eDoors door, CAutomobile* pAuto);

    uint8 GetAeroplaneCompStatus(uint8 frame);
    void SetAeroplaneCompStatus(uint8 frame, ePanelDamageState status);

    uint32 GetEngineStatus();
    void SetEngineStatus(uint32 status);

    // There are 2 door functions, one takes `tComponent` the other `eDoors`.
    // To select the correct version to call look at the called function's address when you are REing code
    // And chose accordingly.
    [[nodiscard]] eDoorStatus GetDoorStatus_Component(tComponent nDoorIdx) const;
    void SetDoorStatus_Component(tComponent door, eDoorStatus status);

    [[nodiscard]] eDoorStatus GetDoorStatus(eDoors door) const;
    void SetDoorStatus(eDoors door, eDoorStatus status);

    [[nodiscard]] eCarWheelStatus GetWheelStatus(eCarWheel wheel) const;
    void SetWheelStatus(eCarWheel wheel, eCarWheelStatus status);

    [[nodiscard]] ePanelDamageState GetPanelStatus(ePanels panel) const;
    void SetPanelStatus(ePanels panel, ePanelDamageState status);

    [[nodiscard]] eLightsDamageState GetLightStatus(eLights light) const;
    void SetLightStatus(eLights light, eLightsDamageState status);

    // returns -1 if no node for this panel
    static eCarNodes GetCarNodeIndexFromPanel(ePanels panel);
    static eCarNodes GetCarNodeIndexFromDoor(eDoors door);
    static bool GetComponentGroup(tComponent nComp, tComponentGroup& outCompGroup, uint8& outComponentRelativeIdx);

    // NOTSA
    void SetAllWheelsState(eCarWheelStatus state);
    void SetDoorStatus(std::initializer_list<eDoors> doors, eDoorStatus status);
    void SetDoorOpen(eDoors door);
    void SetDoorOpen_Component(tComponent door);
    void SetDoorClosed(eDoors door);
    [[nodiscard]] auto GetAllLightsState() const -> std::array<eLightsDamageState, 4>;
    [[nodiscard]] auto GetAllDoorsStatus() const -> std::array<eDoorStatus, MAX_DOORS>;
    [[nodiscard]] bool IsDoorOpen(eDoors door) const;
    [[nodiscard]] bool IsDoorClosed(eDoors door) const;
    [[nodiscard]] bool IsDoorPresent(eDoors door) const;
    [[nodiscard]] bool IsDoorDamaged(eDoors door) const;

    template<typename T>
    requires std::is_integral_v<T> // Только для целых чисел
    inline T GetRandomNumberInRange(T min, T max) {
        static std::random_device rd;  // Источник энтропии
        static std::mt19937 gen(rd()); // Генератор (Mersenne Twister)
        std::uniform_int_distribution<T> dist(min, max - 1); // [min, max-1]
        return dist(gen);
    }

    template<typename T>
    requires (std::is_integral_v<T> && std::is_unsigned_v<T>) // Только для беззнаковых
    inline T GetRandomNumberInRange(T max) {
        return GetRandomNumberInRange<T>(0, max);
    }

private:
    // Wrapper functions

    // Return type has to be uint32, but we want to return `eCarWheelStatus` in our code.
    uint32 GetWheelStatus_Hooked(eCarWheel wheel) { return (uint32)GetWheelStatus(wheel); }
};

VALIDATE_SIZE(CDamageManager, 0x18);
