//
// Created on 23.07.2023.
//

#include "DamageManager.h"
#include "Entity/Vehicle/Automobile.h"

CDamageManager::CDamageManager(float wheelDamageEffect) :
        m_fWheelDamageEffect{ wheelDamageEffect }
{
    ResetDamageStatus();
}

void CDamageManager::Init() {
    m_fWheelDamageEffect = 0.0f;
    ResetDamageStatus();
}

void CDamageManager::ResetDamageStatusAndWheelDamage() {
    ResetDamageStatus();
    m_fWheelDamageEffect = 0.5f;
}

void CDamageManager::FuckCarCompletely(bool bDontDetachWheel) {
    if (!bDontDetachWheel)
        SetWheelStatus((eCarWheel)GetRandomNumberInRange((size_t)eCarWheel::MAX_CARWHEELS), eCarWheelStatus::WHEEL_STATUS_MISSING);

    for (size_t i = 0; i < eDoors::MAX_DOORS; i++)
        SetDoorStatus((eDoors)i, eDoorStatus::DAMSTATE_NOTPRESENT);

    for (size_t i = 0; i < 3; i++) {
        ProgressPanelDamage(ePanels::FRONT_BUMPER);
        ProgressPanelDamage(ePanels::REAR_BUMPER);
    }

    for (size_t i = 0; i < eLights::MAX_LIGHTS; i++)
        SetLightStatus((eLights)i, eLightsDamageState::VEHICLE_LIGHT_OK);

    for (size_t i = 0; i < ePanels::MAX_PANELS; i++)
        SetPanelStatus((ePanels)i, ePanelDamageState::DAMSTATE_OK);

    SetEngineStatus(250u);
}

bool CDamageManager::ApplyDamage(CAutomobile* vehicle, tComponent compId, float fIntensity, float fColDmgMult) {
    if (!vehicle->autoFlags.bCanBeVisiblyDamaged)
        return false;

    tComponentGroup group{};
    uint8 relCompIdx{};
    GetComponentGroup(compId, group, relCompIdx);

    static constexpr float afPanelDamageMultByGroup[] = { 2.5f, 1.25f, 3.2f, 1.4f, 2.5f, 2.8f, 0.5f, 1.2f, 0.87f, 0.2f };
    fIntensity *= afPanelDamageMultByGroup[(size_t)group];
    if (compId == tComponent::COMPONENT_WINDSCREEN)
        fIntensity *= 0.6f;

    if (fIntensity <= 150.0f)
        return false;

    switch (group) {
        case tComponentGroup::COMPGROUP_PANEL: {
            //if (ProgressPanelDamage((ePanels)relCompIdx))
            //    vehicle->SetBumperDamage((ePanels)relCompIdx, false);
            return true;
        }
        case tComponentGroup::COMPGROUP_WHEEL: {
            ProgressWheelDamage((eCarWheel)relCompIdx);
            return true;
        }
        case tComponentGroup::COMPGROUP_DOOR:
        case tComponentGroup::COMPGROUP_BONNET:
        case tComponentGroup::COMPGROUP_BOOT: {
            //if (ProgressDoorDamage((eDoors)relCompIdx, vehicle))
            //    vehicle->SetDoorDamage((eDoors)relCompIdx, false);
            return true;
        }
        case tComponentGroup::COMPGROUP_LIGHT: {
            SetLightStatus((eLights)relCompIdx, eLightsDamageState::VEHICLE_LIGHT_SMASHED);
            //if (ProgressPanelDamage((ePanels)relCompIdx))
            //    vehicle->SetPanelDamage((ePanels)relCompIdx, false);
            return true;
        }
        default:
            return true;
    }
}

bool CDamageManager::ProgressAeroplaneDamage(uint8 nFrameId) {
    const auto current = (ePanelDamageState)GetAeroplaneCompStatus(nFrameId);
    if (current == ePanelDamageState::DAMSTATE_DAMAGED)
        return false;

    SetAeroplaneCompStatus(nFrameId, (ePanelDamageState)((unsigned)current + 1));
    return true;
}

bool CDamageManager::ProgressWheelDamage(eCarWheel wheel) {
    const eCarWheelStatus current = GetWheelStatus(wheel);
    if (current == eCarWheelStatus::WHEEL_STATUS_MISSING)
        return false;

    SetWheelStatus(wheel, (eCarWheelStatus)((unsigned)current + 1));
    return true;
}

bool CDamageManager::ProgressPanelDamage(ePanels panel) {
    switch (GetPanelStatus(panel)) {
        case ePanelDamageState::DAMSTATE_DAMAGED:
        {
            switch (panel) {
                case ePanels::WINDSCREEN_PANEL: {
                    if (CGeneral::GetRandomNumberInRange(0, 2))
                        return false;
                    break;
                }
                default: {
                    if (CGeneral::GetRandomNumberInRange(0, 8))
                        return false;
                    break;
                }
            }
            SetPanelStatus(panel, ePanelDamageState::DAMSTATE_OPENED_DAMAGED);
            return true;
        }
        case ePanelDamageState::DAMSTATE_OPENED_DAMAGED: {
            return false;
        }
    }
    SetPanelStatus(panel, (ePanelDamageState)((int)GetPanelStatus(panel) + 1));
    return true;
}

void CDamageManager::ProgressEngineDamage() {

}

bool CDamageManager::ProgressDoorDamage(eDoors door, CAutomobile* pAuto) {
    if ((uint32)door >= (uint32)eDoors::MAX_DOORS) {
        return false;
    }

    switch (GetDoorStatus(door)) {
        case eDoorStatus::DAMSTATE_OK:
        case eDoorStatus::DAMSTATE_OPENED: {
            //pAuto->GetDoor(door).Open(0.0f);
            SetDoorStatus(door, eDoorStatus::DAMSTATE_DAMAGED);
            break;
        }
        case eDoorStatus::DAMSTATE_DAMAGED: {
            SetDoorStatus(door, eDoorStatus::DAMSTATE_OPENED_DAMAGED);
            break;
        }
        case eDoorStatus::DAMSTATE_OPENED_DAMAGED: {
            if (CGeneral::GetRandomNumberInRange(0, 8))
                return false;
            SetDoorStatus(door, eDoorStatus::DAMSTATE_NOTPRESENT);
            break;
        }
        case eDoorStatus::DAMSTATE_NOTPRESENT:
            return false;
    }

    return true;
}

uint8 CDamageManager::GetAeroplaneCompStatus(uint8 frame) {
    return m_nPanelsStatus >> (2 * (m_nPanelsStatus - 12)) & 3;
}

void CDamageManager::SetAeroplaneCompStatus(uint8 frame, ePanelDamageState status) {
    const auto component = frame - 12;
    m_nPanelsStatus = (status << (2 * component)) | m_nPanelsStatus & ~(3 << (2 * component));
}

uint32 CDamageManager::GetEngineStatus() {
    return m_nEngineStatus;
}

void CDamageManager::SetEngineStatus(uint32 status) {
    m_nEngineStatus = std::min<uint8>(status, 250u);
}

eCarNodes CDamageManager::GetCarNodeIndexFromPanel(ePanels panel) {
    switch (panel) {
        case ePanels::FRONT_LEFT_PANEL:
            return eCarNodes::CAR_WING_LF;
        case ePanels::FRONT_RIGHT_PANEL:
            return eCarNodes::CAR_WING_RF;
        case ePanels::WINDSCREEN_PANEL:
            return eCarNodes::CAR_WINDSCREEN;
        case ePanels::FRONT_BUMPER:
            return eCarNodes::CAR_BUMP_FRONT;
        case ePanels::REAR_BUMPER:
            return eCarNodes::CAR_BUMP_REAR;
        default:
            return (eCarNodes)-1;
    }
}

eDoorStatus CDamageManager::GetDoorStatus_Component(tComponent doorComp) const {
    switch (doorComp) {
        case tComponent::COMPONENT_DOOR_RF:
            return GetDoorStatus(eDoors::DOOR_RIGHT_FRONT);
        case tComponent::COMPONENT_DOOR_LR:
            return GetDoorStatus(eDoors::DOOR_RIGHT_REAR);
        case tComponent::COMPONENT_DOOR_RR:
            return GetDoorStatus(eDoors::DOOR_LEFT_FRONT);
        case tComponent::COMPONENT_WING_LF:
            return GetDoorStatus(eDoors::DOOR_LEFT_REAR);
        default:
            return eDoorStatus::DAMSTATE_NOTPRESENT;
    }
}

void CDamageManager::SetDoorStatus_Component(tComponent doorComp, eDoorStatus status) {
    switch (doorComp) {
        case tComponent::COMPONENT_DOOR_RF:
            SetDoorStatus(eDoors::DOOR_RIGHT_FRONT, status);
            break;
        case tComponent::COMPONENT_DOOR_LR:
            SetDoorStatus(eDoors::DOOR_RIGHT_REAR, status);
            break;
        case tComponent::COMPONENT_DOOR_RR:
            SetDoorStatus(eDoors::DOOR_LEFT_FRONT, status);
            break;
        case tComponent::COMPONENT_WING_LF:
            SetDoorStatus(eDoors::DOOR_LEFT_REAR, status);
            break;
    }
}

void CDamageManager::SetDoorStatus(eDoors door, eDoorStatus status) {
    switch (door) {
        case eDoors::DOOR_BONNET:
        case eDoors::DOOR_BOOT:
        case eDoors::DOOR_LEFT_FRONT:
        case eDoors::DOOR_RIGHT_FRONT:
        case eDoors::DOOR_LEFT_REAR:
        case eDoors::DOOR_RIGHT_REAR:
            m_aDoorsStatus[(unsigned)door] = status;
            break;
    }
}

eCarWheelStatus CDamageManager::GetWheelStatus(eCarWheel wheel) const {
    return m_anWheelsStatus[(unsigned)wheel];
}

void CDamageManager::SetWheelStatus(eCarWheel wheel, eCarWheelStatus status) {
    m_anWheelsStatus[(unsigned)wheel] = status;
}

ePanelDamageState CDamageManager::GetPanelStatus(ePanels panel) const {
    return (ePanelDamageState)((m_nPanelsStatus >> (4 * (unsigned)panel)) & 0xF);
}

void CDamageManager::SetPanelStatus(ePanels panel, ePanelDamageState status) {
    m_nPanelsStatus = (status << (4 * (unsigned)panel)) | m_nPanelsStatus & ~(15 << (4 * (unsigned)panel));
}

eLightsDamageState CDamageManager::GetLightStatus(eLights light) const {
    return (eLightsDamageState)((m_nLightsStatus >> (2 * (unsigned)light)) & 3);
}

void CDamageManager::SetLightStatus(eLights light, eLightsDamageState status) {
    m_nLightsStatus = ((unsigned)status << (2 * (unsigned)light)) | m_nLightsStatus & ~(3 << (2 * (unsigned)light));
}

void CDamageManager::ResetDamageStatus() {
    SetEngineStatus(0);

    for (size_t i = 0; i < eCarWheel::MAX_CARWHEELS; i++)
        SetWheelStatus((eCarWheel)i, eCarWheelStatus::WHEEL_STATUS_OK);

    for (size_t i = 0; i < eDoors::MAX_DOORS; i++)
        SetDoorStatus((eDoors)i, eDoorStatus::DAMSTATE_OK);

    for (size_t i = 0; i < eLights::MAX_LIGHTS; i++)
        SetLightStatus((eLights)i, eLightsDamageState::VEHICLE_LIGHT_OK);

    for (size_t i = 0; i < ePanels::MAX_PANELS; i++)
        SetPanelStatus((ePanels)i, ePanelDamageState::DAMSTATE_OK);
}

bool CDamageManager::GetComponentGroup(tComponent nComp, tComponentGroup& outCompGroup, uint8& outComponentRelativeIdx) {
    outComponentRelativeIdx = (uint8)-2;
    switch (nComp) {
        case tComponent::COMPONENT_WHEEL_LF:
        case tComponent::COMPONENT_WHEEL_RF:
        case tComponent::COMPONENT_WHEEL_LR:
        case tComponent::COMPONENT_WHEEL_RR: {
            outCompGroup = tComponentGroup::COMPGROUP_WHEEL;
            outComponentRelativeIdx = (uint8)nComp - 1;
            return true;
        }
        case tComponent::COMPONENT_BONNET: {
            outCompGroup = tComponentGroup::COMPGROUP_BONNET;
            outComponentRelativeIdx = (uint8)eDoors::DOOR_BONNET;
            return true;
        }
        case tComponent::COMPONENT_BOOT: {
            outCompGroup = tComponentGroup::COMPGROUP_BOOT;
            outComponentRelativeIdx = (uint8)eDoors::DOOR_BOOT;
            return true;
        }
        case tComponent::COMPONENT_DOOR_LF:
        case tComponent::COMPONENT_DOOR_RF:
        case tComponent::COMPONENT_DOOR_LR:
        case tComponent::COMPONENT_DOOR_RR: {
            outCompGroup = tComponentGroup::COMPGROUP_DOOR;
            outComponentRelativeIdx = (uint8)nComp - (uint8)tComponent::COMPONENT_BONNET;
            return true;
        }
        case tComponent::COMPONENT_WING_LF:
        case tComponent::COMPONENT_WING_RF:
        case tComponent::COMPONENT_WING_LR:
        case tComponent::COMPONENT_WING_RR:
        case tComponent::COMPONENT_WINDSCREEN: {
            outCompGroup = tComponentGroup::COMPGROUP_LIGHT;
            outComponentRelativeIdx = (uint8)nComp - (uint8)tComponent::COMPONENT_WING_LF;
            return true;
        }
        case tComponent::COMPONENT_BUMP_FRONT:
        case tComponent::COMPONENT_BUMP_REAR: {
            outCompGroup = tComponentGroup::COMPGROUP_PANEL;
            outComponentRelativeIdx = (uint8)nComp - (uint8)tComponent::COMPONENT_WING_LF;
            return true;
        }
        case tComponent::COMPONENT_NA: {
            outCompGroup = tComponentGroup::COMPGROUP_NA;
            outComponentRelativeIdx = 0;
            return true;
        }
    }
    return false;
}

void CDamageManager::SetAllWheelsState(eCarWheelStatus state) {
    constexpr eCarWheel wheels[]{CAR_WHEEL_FRONT_LEFT, CAR_WHEEL_REAR_LEFT, CAR_WHEEL_FRONT_RIGHT, CAR_WHEEL_REAR_RIGHT};
    for (auto&& wheel : wheels) {
        SetWheelStatus(wheel, state);
    }
}

void CDamageManager::SetDoorStatus(std::initializer_list<eDoors> doors, eDoorStatus status) {
    for (auto&& door : doors) {
        SetDoorStatus(door, status);
    }
}

auto CDamageManager::GetAllLightsState() const->std::array<eLightsDamageState, 4> {
    return {
            GetLightStatus(eLights::LEFT_HEADLIGHT),
            GetLightStatus(eLights::RIGHT_HEADLIGHT),
            GetLightStatus(eLights::LEFT_TAIL_LIGHT),
            GetLightStatus(eLights::RIGHT_TAIL_LIGHT)
    };
}

auto CDamageManager::GetAllDoorsStatus() const -> std::array<eDoorStatus, MAX_DOORS> {
    return {
            GetDoorStatus(eDoors::DOOR_BONNET),
            GetDoorStatus(eDoors::DOOR_BOOT),
            GetDoorStatus(eDoors::DOOR_LEFT_FRONT),
            GetDoorStatus(eDoors::DOOR_RIGHT_FRONT),
            GetDoorStatus(eDoors::DOOR_LEFT_REAR),
            GetDoorStatus(eDoors::DOOR_RIGHT_REAR)
    };
}

bool CDamageManager::IsDoorOpen(eDoors door) const {
    switch (GetDoorStatus(door)) {
        case eDoorStatus::DAMSTATE_OPENED:
        case eDoorStatus::DAMSTATE_OPENED_DAMAGED:
            return true;
        case eDoorStatus::DAMSTATE_NOTPRESENT:
            assert(0 && "Door not present @ IsDoorOpen"); // Otherwise `!IsDoorOpen() == IsDoorClosed()` may not always be true which may cause bugs :D
            return false;
    }
    return false;
}

bool CDamageManager::IsDoorClosed(eDoors door) const {
    switch (GetDoorStatus(door)) {
        case eDoorStatus::DAMSTATE_OK:
        case eDoorStatus::DAMSTATE_DAMAGED:
            return true;
        case eDoorStatus::DAMSTATE_NOTPRESENT:
            assert(0 && "Door not present @ IsDoorClosed"); // Otherwise `!IsDoorOpen() == IsDoorClosed()` may not always be true which may cause bugs :D
            return false;
    }
    return false;
}

bool CDamageManager::IsDoorPresent(eDoors door) const {
    return GetDoorStatus(door) != DAMSTATE_NOTPRESENT;
}

bool CDamageManager::IsDoorDamaged(eDoors door) const {
    switch (GetDoorStatus(door)) {
        case eDoorStatus::DAMSTATE_OK:
        case eDoorStatus::DAMSTATE_OPENED:
            return false;
    }
    return true;
}

void CDamageManager::SetDoorOpen(eDoors door) {
    switch (GetDoorStatus(door)) {
        case eDoorStatus::DAMSTATE_OK:
            SetDoorStatus(door, eDoorStatus::DAMSTATE_OPENED);
            break;
        case eDoorStatus::DAMSTATE_DAMAGED:
            SetDoorStatus(door, eDoorStatus::DAMSTATE_OPENED_DAMAGED);
            break;
        case eDoorStatus::DAMSTATE_NOTPRESENT:
            //assert(0 && "Door should be present @ SetDoorOpen");
            break;
    }
}

void CDamageManager::SetDoorOpen_Component(tComponent door) {
    switch (GetDoorStatus_Component(door)) {
        case eDoorStatus::DAMSTATE_OK:
            SetDoorStatus_Component(door, eDoorStatus::DAMSTATE_OPENED);
            break;
        case eDoorStatus::DAMSTATE_DAMAGED:
            SetDoorStatus_Component(door, eDoorStatus::DAMSTATE_OPENED_DAMAGED);
            break;
        case eDoorStatus::DAMSTATE_NOTPRESENT:
            //assert(0 && "Door should be present @ SetDoorOpen_Component");
            break;
    }
}

void CDamageManager::SetDoorClosed(eDoors door) {
    switch (GetDoorStatus(door)) {
        case eDoorStatus::DAMSTATE_OPENED:
            SetDoorStatus(door, eDoorStatus::DAMSTATE_OK);
            break;
        case eDoorStatus::DAMSTATE_OPENED_DAMAGED:
            SetDoorStatus(door, eDoorStatus::DAMSTATE_DAMAGED);
            break;
        case eDoorStatus::DAMSTATE_NOTPRESENT:
            //assert(0 && "Door should be present @ SetDoorClosed");
            break;
    }
}

eDoorStatus CDamageManager::GetDoorStatus(eDoors nDoorIdx) const {
    switch (nDoorIdx) {
        case eDoors::DOOR_BONNET:
        case eDoors::DOOR_BOOT:
        case eDoors::DOOR_LEFT_FRONT:
        case eDoors::DOOR_RIGHT_FRONT:
        case eDoors::DOOR_LEFT_REAR:
        case eDoors::DOOR_RIGHT_REAR:
            return m_aDoorsStatus[(unsigned)nDoorIdx];
        default:
            return eDoorStatus::DAMSTATE_NOTPRESENT;
    }
}

eCarNodes CDamageManager::GetCarNodeIndexFromDoor(eDoors door) {
    switch (door) {
        case eDoors::DOOR_BONNET:
            return eCarNodes::CAR_BONNET;
        case eDoors::DOOR_BOOT:
            return eCarNodes::CAR_BOOT;
        case eDoors::DOOR_LEFT_FRONT:
            return eCarNodes::CAR_DOOR_LF;
        case eDoors::DOOR_RIGHT_FRONT:
            return eCarNodes::CAR_DOOR_RF;
        case eDoors::DOOR_LEFT_REAR:
            return eCarNodes::CAR_DOOR_LR;
        case eDoors::DOOR_RIGHT_REAR:
            return eCarNodes::CAR_DOOR_RR;
        default:
            return (eCarNodes)-1;
    }
}

void CDamageManager::InjectHooks() {

}