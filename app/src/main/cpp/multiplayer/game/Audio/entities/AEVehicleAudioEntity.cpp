//
// Created by Traw-GG on 13.07.2025.
//
#include "AEVehicleAudioEntity.h"
#include "util/patch.h"
#include "AEAudioEntity.h"
#include "game/Models/ModelInfo.h"

tVehicleAudioSettings VehicleAudioProperties[20000];
void readVehiclesAudioSettings()
{
    char vehicleModel[50];

    FILE* pFile;

    char line[300];

    // Zero VehicleAudioProperties
    memset(VehicleAudioProperties, 0x00, sizeof(VehicleAudioProperties));

    tVehicleAudioSettings CurrentVehicleAudioProperties{};

    memset(&CurrentVehicleAudioProperties, 0x0, sizeof(tVehicleAudioSettings));

    char buffer[0xFF];
    sprintf(buffer, "%s/AudioConfig/AudioConfig.cfg", g_pszRootStorage);
    pFile = fopen(buffer, "r");
    if (!pFile)
    {
        Log("Cannot read AudioConfig.cfg");
        return;
    }

    // File exists
    while (fgets(line, sizeof(line), pFile))
    {
        if (strncmp(line, ";the end", 8) == 0)
            break;

        if (line[0] == ';')
            continue;

        sscanf(line, "%s %d %d %d %d %f %f %d %f %d %d %d %d %d %f",
               vehicleModel,
               &CurrentVehicleAudioProperties.m_nVehicleSoundType,
               &CurrentVehicleAudioProperties.m_nEngineOnSoundBankId,
               &CurrentVehicleAudioProperties.m_nEngineOffSoundBankId,
               &CurrentVehicleAudioProperties.m_nBassSetting,
               &CurrentVehicleAudioProperties.m_fBassEq,
               &CurrentVehicleAudioProperties.field_C,
               &CurrentVehicleAudioProperties.m_nHornToneSoundInBank,
               &CurrentVehicleAudioProperties.m_fHornHigh,
               &CurrentVehicleAudioProperties.m_nDoorSound,
               &CurrentVehicleAudioProperties.m_EngineUpgrade,
               &CurrentVehicleAudioProperties.m_nRadioID,
               &CurrentVehicleAudioProperties.m_nRadioType,
               &CurrentVehicleAudioProperties.m_nVehTypeForAudio,
               &CurrentVehicleAudioProperties.m_fHornVolumeDelta);

        int32 result = CModelInfo::GetModelInfoIndex(vehicleModel);
        memcpy(&VehicleAudioProperties[result-400], &CurrentVehicleAudioProperties, sizeof(tVehicleAudioSettings));
    }

    fclose(pFile);
}

tVehicleAudioSettings CAEVehicleAudioEntity::GetVehicleAudioSettings(int16 vehId) {
    return VehicleAudioProperties[vehId - 400];
}

void CAEVehicleAudioEntity::Initialise(CEntity* entity) {
    assert(entity && entity->IsVehicle());

    m_nTime144 = 0;
    m_pEntity = entity;
    m_bPlayerDriver = false;
    m_bPlayerPassenger = false;
    m_bVehicleRadioPaused = false;
    m_bSoundsStopped = false;
    m_nEngineState = 0;
    m_nGearRelatedStuff = 0;
    field_AC = 0;
    m_nEngineBankSlotId = -1;
    m_nRainDropCounter = 0;
    field_7C = 0;
    field_B4 = 0;
    field_B8 = 0;
    field_BC = false;
    m_nBoatHitWaveLastPlayedTime = 0;
    m_nTimeToInhibitAcc = 0;
    m_nTimeToInhibitCrz = 0;
    m_bNitroSoundPresent = false;
    m_bDisableHeliEngineSounds = false;
    m_nEngineSoundPlayPos = -1;
    m_nEngineSoundLastPlayedPos = -1;
    field_154 = 0;
    field_14E = 0;
    m_nAcclLoopCounter = 0;

    for (auto i = 0; auto& sound : m_aEngineSounds) {
        sound.Init(i++);
    }

    m_fHornVolume = -100.0f;
    m_fPlaneSoundVolume_Probably = -100.0f;
    m_nSkidSoundType = -1;
    m_nRoadNoiseSoundType = -1;
    m_nFlatTyreSoundType = -1;
    m_nReverseGearSoundType = -1;
    field_234 = -1.0f;
    m_fPlaneSoundSpeed = -1.0f;
    field_248 = -1.0f;

    m_SkidSoundMaybe   = nullptr;
    m_RoadNoiseSound   = nullptr;
    m_FlatTyreSound    = nullptr;
    m_ReverseGearSound = nullptr;
    m_HornTonSound     = nullptr;
    m_SirenSound       = nullptr;
    m_PoliceSirenSound = nullptr;

    field_238 = 0.0f;
    field_23C = 1.0f;
    field_240 = 0.0f;

    m_Settings = GetVehicleAudioSettings(entity->m_nModelIndex);
    m_bModelWithSiren = reinterpret_cast<CVehicle*>(entity)->UsesSiren();
    if (m_Settings.m_nRadioType == eRadioType::RADIO_UNKNOWN) {
        m_Settings.m_nRadioID = eRadioID::RADIO_OFF;
    }

    m_fGeneralVehicleSoundVolume = GetDefaultVolume(AE_GENERAL_VEHICLE_SOUND);

    switch (entity->m_nModelIndex) {
        case MODEL_PIZZABOY:
        case MODEL_CADDY:
        case MODEL_FAGGIO:
        case MODEL_BAGGAGE:
        case MODEL_FORKLIFT:
        case MODEL_VORTEX:
        case MODEL_KART:
        case MODEL_MOWER:
        case MODEL_SWEEPER:
        case MODEL_TUG:
            m_bInhibitAccForLowSpeed = true;
            break;
        default:
            m_bInhibitAccForLowSpeed = false;
            break;
    }

    switch (m_Settings.m_nVehicleSoundType) {
        case VEHICLE_SOUND_CAR:
            m_fGeneralVehicleSoundVolume -= 1.5F;
            m_nEngineAccelerateSoundBankId = m_Settings.m_nEngineOnSoundBankId;
            m_nEngineDecelerateSoundBankId = m_Settings.m_nEngineOffSoundBankId;
            if (m_bEnabled)
                return;

            if (m_Settings.m_nEngineOffSoundBankId != -1 && m_Settings.m_nEngineOffSoundBankId != 129) {
                m_nEngineBankSlotId = CHook::CallFunction<int16>("_ZN21CAEVehicleAudioEntity15RequestBankSlotEs", m_Settings.m_nEngineOffSoundBankId);
            }

            m_bEnabled = true;
            return;

        case VEHICLE_SOUND_MOTORCYCLE:
        case VEHICLE_SOUND_BICYCLE:
            m_nEngineAccelerateSoundBankId = m_Settings.m_nEngineOnSoundBankId;

            if (m_Settings.IsMotorcycle())
                m_fGeneralVehicleSoundVolume = m_fGeneralVehicleSoundVolume - 1.5F;

            if (m_bEnabled)
                return;

            m_nEngineDecelerateSoundBankId = m_Settings.m_nEngineOffSoundBankId;
            if (m_nEngineDecelerateSoundBankId != -1)
                m_nEngineBankSlotId = CHook::CallFunction<int16>("_ZN21CAEVehicleAudioEntity15RequestBankSlotEs", m_Settings.m_nEngineOffSoundBankId);

            m_bEnabled = true;
            return;

        case VEHICLE_SOUND_BOAT:
        case VEHICLE_SOUND_TRAIN:
            m_nEngineAccelerateSoundBankId = m_Settings.m_nEngineOnSoundBankId;
            m_nEngineDecelerateSoundBankId = m_Settings.m_nEngineOffSoundBankId;
            if (m_bEnabled)
                return;

            if (m_Settings.m_nEngineOffSoundBankId != -1 && m_Settings.m_nEngineOffSoundBankId != 129)
                m_nEngineBankSlotId = CHook::CallFunction<int16>("_ZN21CAEVehicleAudioEntity15RequestBankSlotEs", m_Settings.m_nEngineOffSoundBankId);

            m_bEnabled = true;
            return;

        case VEHICLE_SOUND_HELI:
        case VEHICLE_SOUND_NON_VEH:
            m_nEngineDecelerateSoundBankId = m_Settings.m_nEngineOffSoundBankId;
            m_nEngineAccelerateSoundBankId = m_Settings.m_nEngineOnSoundBankId;

            m_bEnabled = true;
            return;

        case VEHICLE_SOUND_PLANE:
            m_nEngineDecelerateSoundBankId = m_Settings.m_nEngineOffSoundBankId;
            m_nEngineAccelerateSoundBankId = m_Settings.m_nEngineOnSoundBankId;
            if (m_bEnabled)
                return;

            if (m_Settings.m_nEngineOffSoundBankId != -1)
                m_nEngineBankSlotId = CHook::CallFunction<int16>("_ZN21CAEVehicleAudioEntity15RequestBankSlotEs", m_Settings.m_nEngineOffSoundBankId);

            m_bEnabled = true;
            return;

        case VEHICLE_SOUND_TRAILER:
            m_nEngineAccelerateSoundBankId = m_Settings.m_nEngineOnSoundBankId;
            m_fGeneralVehicleSoundVolume = m_fGeneralVehicleSoundVolume - 1.5F;
            if (m_bEnabled)
                return;

            m_nEngineDecelerateSoundBankId = m_Settings.m_nEngineOffSoundBankId;
            if (m_nEngineDecelerateSoundBankId != -1)
                m_nEngineBankSlotId = CHook::CallFunction<int16>("_ZN21CAEVehicleAudioEntity15RequestBankSlotEs", m_Settings.m_nEngineOffSoundBankId);

            m_bEnabled = true;
            return;

        default:
            return;
    }
}

void CAEVehicleAudioEntity_Initialise_hooked(CAEVehicleAudioEntity* thiz, CVehicle* pVehicle) {
    thiz->Initialise(pVehicle);
}

void CAEVehicleAudioEntity::InjectHooks() {
    CHook::InstallPLT(g_libGTASA + (VER_x32 ? 0x0066F264 : 0x83EE28), &CAEVehicleAudioEntity_Initialise_hooked);
}