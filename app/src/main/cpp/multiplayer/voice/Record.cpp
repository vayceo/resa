#include "../main.h"

#include "Record.h"

#include "PluginConfig.h"

#include "../main.h"
#include "Record.h"

#include "PluginConfig.h"

namespace Voice{

    bool Record::Init(const uint32_t bitrate) noexcept
    {
        if(Record::initStatus)
            return false;

        if(BASS_IsStarted() == 0)
            return false;

        DLOG("[sv:dbg:record:init] : module initializing...");

        Record::deviceNamesList.clear();
        Record::deviceNumbersList.clear();

        {
            BASS_DEVICEINFO devInfo {};

            for(int devNumber { 0 }; BASS_RecordGetDeviceInfo(devNumber, &devInfo); ++devNumber)
            {
                const bool deviceEnabled = devInfo.flags & BASS_DEVICE_ENABLED;
                const bool deviceLoopback = devInfo.flags & BASS_DEVICE_LOOPBACK;
                const uint32_t deviceType = devInfo.flags & BASS_DEVICE_TYPE_MASK;

                DLOG("[sv:dbg:record:init] : device detect "
                         "[ id(%d) enabled(%hhu) loopback(%hhu) name(%s) type(0x%x) ]",
                         devNumber, deviceEnabled, deviceLoopback, devInfo.name != nullptr
                                                                   ? devInfo.name : "none", deviceType);

                if(deviceEnabled && !deviceLoopback && devInfo.name != nullptr)
                {
                    try
                    {
                        Record::deviceNumbersList.emplace_back(devNumber);
                        Record::deviceNamesList.emplace_back(devInfo.name);
                    }
                    catch(const std::exception& exception)
                    {
                        Log("[sv:err:record:init] : failed to add device");
                        return false;
                    }
                }
            }
        }

        Memory::ScopeExit deviceListsResetScope { [] { Record::usedDeviceIndex = -1;
            Record::deviceNamesList.clear();
            Record::deviceNumbersList.clear(); } };

        if(Record::deviceNamesList.empty() || Record::deviceNumbersList.empty())
        {
            Log("[sv:inf:record:init] : failed to find microphone");
            return false;
        }

        {
            int opusErrorCode { -1 };

            Record::encoder = opus_encoder_create(SV::kFrequency, 1,
                                                  OPUS_APPLICATION_VOIP, &opusErrorCode);

            if(Record::encoder == nullptr || opusErrorCode < 0)
            {
                Log("[sv:err:record:init] : failed to "
                         "create encoder (code:%d)", opusErrorCode);
                return false;
            }
        }

        Memory::ScopeExit encoderResetScope { [] { opus_encoder_destroy(Record::encoder); } };

        if(const auto error = opus_encoder_ctl(Record::encoder,
                                               OPUS_SET_BITRATE(bitrate)); error < 0)
        {
            Log("[sv:err:record:init] : failed to "
                     "set bitrate for encoder (code:%d)", error);
            return false;
        }

        if(const auto error = opus_encoder_ctl(Record::encoder,
                                               OPUS_SET_SIGNAL(OPUS_SIGNAL_VOICE)); error < 0)
        {
            Log("[sv:err:record:init] : failed to "
                     "set audiosignal type for encoder (code:%d)", error);
            return false;
        }

        if(const auto error = opus_encoder_ctl(Record::encoder,
                                               OPUS_SET_COMPLEXITY(10)); error < 0)
        {
            Log("[sv:err:record:init] : failed to "
                     "set complexity for encoder (code:%d)", error);
            return false;
        }

        if(const auto error = opus_encoder_ctl(Record::encoder,
                                               OPUS_SET_PREDICTION_DISABLED(0)); error < 0)
        {
            Log("[sv:err:record:init] : failed to "
                     "enable prediction for encoder (code:%d)", error);
            return false;
        }

        if(const auto error = opus_encoder_ctl(Record::encoder,
                                               OPUS_SET_LSB_DEPTH(8)); error < 0)
        {
            Log("[sv:err:record:init] : failed to "
                     "set lsb depth for encoder (code:%d)", error);
            return false;
        }

        if(const auto error = opus_encoder_ctl(Record::encoder,
                                               OPUS_SET_FORCE_CHANNELS(1)); error < 0)
        {
            Log("[sv:err:record:init] : failed to "
                     "set count channels for encoder (code:%d)", error);
            return false;
        }

        if(const auto error = opus_encoder_ctl(Record::encoder,
                                               OPUS_SET_DTX(0)); error < 0)
        {
            Log("[sv:err:record:init] : failed to "
                     "set dtx for encoder (code:%d)", error);
            return false;
        }

        if(const auto error = opus_encoder_ctl(Record::encoder,
                                               OPUS_SET_INBAND_FEC(1)); error < 0)
        {
            Log("[sv:err:record:init] : failed to "
                     "set inband fec for encoder (code:%d)", error);
            return false;
        }

        if(const auto error = opus_encoder_ctl(Record::encoder,
                                               OPUS_SET_PACKET_LOSS_PERC(10)); error < 0)
        {
            Log("[sv:err:record:init] : failed to set "
                     "packet loss percent for encoder (code:%d)", error);
            return false;
        }

        Record::usedDeviceIndex = -1;

        if(PluginConfig::IsRecordLoaded() && !PluginConfig::GetDeviceName().empty())
        {
            for(std::size_t i { 0 }; i < Record::deviceNamesList.size(); ++i)
            {
                if(Record::deviceNamesList[i] == PluginConfig::GetDeviceName())
                {
                    Record::usedDeviceIndex = i;
                    break;
                }
            }
        }

        bool initRecordStatus = BASS_RecordInit(Record::usedDeviceIndex != -1 ?
                                                Record::deviceNumbersList[Record::usedDeviceIndex] : -1);

        if(!initRecordStatus && Record::usedDeviceIndex != -1)
        {
            initRecordStatus = BASS_RecordInit(Record::usedDeviceIndex = -1);
        }

        if(initRecordStatus && Record::usedDeviceIndex == -1)
        {
            for(std::size_t i { 0 }; i < Record::deviceNumbersList.size(); ++i)
            {
                if(Record::deviceNumbersList[i] == BASS_RecordGetDevice())
                {
                    Record::usedDeviceIndex = i;
                    break;
                }
            }
        }

        Memory::ScopeExit recordResetScope { [] { BASS_RecordFree(); } };

        if(!initRecordStatus || Record::usedDeviceIndex == -1)
        {
            Log("[sv:err:record:init] : failed to "
                     "init device (code:%d)", BASS_ErrorGetCode());
            return false;
        }

        if(!PluginConfig::IsRecordLoaded())
        {
            PluginConfig::SetDeviceName(Record::deviceNamesList[Record::usedDeviceIndex]);
        }

        Record::checkChannel = BASS_StreamCreate(SV::kFrequency2, 1,
                                                 NULL, STREAMPROC_PUSH, nullptr);

        if(Record::checkChannel == NULL)
        {
            Log("[sv:err:record:init] : failed to create "
                     "check stream (code:%d)", BASS_ErrorGetCode());
            return false;
        }

        BASS_ChannelSetAttribute(Record::checkChannel, BASS_ATTRIB_VOL, 1.f);

        if(!PluginConfig::IsRecordLoaded())
        {
            PluginConfig::SetRecordLoaded(true);
            Record::ResetConfigs();
        }

        deviceListsResetScope.Release();
        encoderResetScope.Release();
        recordResetScope.Release();
        //channelResetScope.Release();

        DLOG("[sv:dbg:record:init] : module initialized");

        Record::initStatus = true;
        Record::SyncConfigs();

        return true;
    }

    bool Record::IsInited() noexcept
    {
        return Record::initStatus;
    }

    void Record::Free() noexcept
    {
        if(!Record::initStatus)
            return;

        Log("[sv:dbg:record:free] : module releasing...");

        Record::StopRecording();
        BASS_ChannelStop(Record::recordChannel);
        BASS_RecordFree();

        Record::StopChecking();
        BASS_StreamFree(Record::checkChannel);

        opus_encoder_destroy(Record::encoder);

        Record::usedDeviceIndex = -1;
        Record::deviceNumbersList.clear();
        Record::deviceNamesList.clear();

        Log("[sv:dbg:record:free] : module released");

        Record::initStatus = false;
    }

    float clampEx(float source, float min, float max)
    {
        if(source < min) return min;
        if(source > max) return max;
        return source;
    }

    void Record::Tick() noexcept
    {
        if (Record::initStatus && Record::checkStatus)
        {
            if (const auto bufferSize = BASS_ChannelGetData(Record::recordChannel,
                                                            nullptr, BASS_DATA_AVAILABLE); bufferSize != -1 && bufferSize != 0)
            {
                if (const auto readDataSize = BASS_ChannelGetData(Record::recordChannel, Record::encBuffer.data(),
                                                                  clampEx(bufferSize, 0ul, SV::kFrameSizeInBytes)); readDataSize != -1 && readDataSize != 0)
                {
                    BASS_StreamPutData(Record::checkChannel, Record::encBuffer.data(), readDataSize);
                }
            }
        }
    }

    uint32_t Record::GetFrame(BYTE* const bufferPtr, const DWORD bufferSize) noexcept
    {
        if (!Record::initStatus || !Record::recordChannel || Record::checkStatus)
            return NULL;

        const auto cBufferSize = BASS_ChannelGetData(Record::recordChannel, nullptr, BASS_DATA_AVAILABLE);
        if (cBufferSize == -1 || cBufferSize < SV::kFrameSizeInBytes) return NULL;

        if (BASS_ChannelGetData(Record::recordChannel, Record::encBuffer.data(),
                                SV::kFrameSizeInBytes) != SV::kFrameSizeInBytes) return NULL;

        const auto encDataLength = opus_encode(Record::encoder, Record::encBuffer.data(),
                                               SV::kFrameSizeInSamples, bufferPtr, bufferSize);

        return encDataLength > 0 ? static_cast<DWORD>(encDataLength) : NULL;
    }


    bool Record::HasMicro() noexcept
    {
        BASS_DEVICEINFO devInfo {};

        for(uint32_t devNumber { 0 }; BASS_RecordGetDeviceInfo(devNumber, &devInfo); ++devNumber)
        {
            const bool deviceEnabled = devInfo.flags & BASS_DEVICE_ENABLED;
            const bool deviceLoopback = devInfo.flags & BASS_DEVICE_LOOPBACK;

            if(deviceEnabled && !deviceLoopback && devInfo.name != nullptr)
                return true;
        }

        return false;
    }

    bool Record::StartRecording() noexcept
    {
        if(!Record::initStatus || Record::recordChannel || Record::checkStatus)
            return false;

        if(!PluginConfig::GetMicroEnable())
            return false;

        Record::recordChannel = BASS_RecordStart(SV::kFrequency2, 1, /*BASS_RECORD_PAUSE | */BASS_RECORD_ECHOCANCEL | BASS_RECORD_AGC, nullptr, nullptr);

        if(Record::recordChannel == NULL)
        {
            Log("[sv:err:record:init] : failed to create record "
                "stream (code:%d)", BASS_ErrorGetCode());
            return false;
        }

        Log("[sv:dbg:record:startrecording] : channel recording starting...");

      //  BASS_ChannelPlay(Record::recordChannel, 0);

        return true;
    }

    bool Record::IsRecording() noexcept
    {
        return Record::recordChannel != 0;
    }

    void Record::StopRecording() noexcept
    {
        if(!Record::initStatus)
            return;

        if(Record::checkStatus)
            return;

       // BASS_ChannelPause(Record::recordChannel);
        opus_encoder_ctl(Record::encoder, OPUS_RESET_STATE);

        Log("[sv:dbg:record:stoprecording] : channel recording stoped");

        const auto bufferSize = BASS_ChannelGetData(Record::recordChannel, nullptr, BASS_DATA_AVAILABLE);
        if(bufferSize == -1 && bufferSize == 0) return;

        BASS_ChannelGetData(Record::recordChannel, nullptr, bufferSize);

        BASS_ChannelStop(Record::recordChannel);
        Record::recordChannel = 0;
    }

    bool Record::StartChecking() noexcept
    {
        if(!Record::initStatus || Record::checkStatus)
            return false;

        if(!PluginConfig::GetMicroEnable())
            return false;

        Record::StopRecording();

        Log("[sv:dbg:record:startchecking] : checking device starting...");

        BASS_ChannelPlay(Record::checkChannel, 1);
        BASS_ChannelPlay(Record::recordChannel, 1);
        Record::checkStatus = true;

        return true;
    }

    bool Record::IsChecking() noexcept
    {
        return Record::checkStatus;
    }

    void Record::StopChecking() noexcept
    {
        if(Record::initStatus && Record::checkStatus)
        {
            Log("[sv:dbg:record:stopchecking] : checking device stoped");

            BASS_ChannelStop(Record::checkChannel);
            Record::checkStatus = false;
        }
    }

    bool Record::GetMicroEnable() noexcept
    {
        return PluginConfig::GetMicroEnable();
    }

    int Record::GetMicroVolume() noexcept
    {
        return PluginConfig::GetMicroVolume();
    }

    int Record::GetMicroDevice() noexcept
    {
        return Record::usedDeviceIndex;
    }

    void Record::SetMicroEnable(const bool microEnable) noexcept
    {
        if(!Record::initStatus)
            return;

        PluginConfig::SetMicroEnable(microEnable);

        if(!PluginConfig::GetMicroEnable())
        {
            Record::StopRecording();
            Record::StopChecking();
        }
    }

    void Record::SetMicroVolume(const int microVolume) noexcept
    {
        if(!Record::initStatus)
            return;

        PluginConfig::SetMicroVolume(clampEx(microVolume, 0, 100));

        BASS_RecordSetInput(-1, BASS_INPUT_ON, static_cast<float>
                                               (PluginConfig::GetMicroVolume()) / 100.f);
    }

    void Record::SetMicroDevice(const int deviceIndex) noexcept
    {
        if(!Record::initStatus)
            return;

        /*const auto devNamesCount = Record::deviceNamesList.size();
        const auto devNumbersCount = Record::deviceNumbersList.size();

        if(deviceIndex < 0 || deviceIndex >= fmin(devNamesCount, devNumbersCount))
            return;

        if(deviceIndex == Record::usedDeviceIndex)
            return;

        BASS_ChannelStop(Record::recordChannel);
        BASS_RecordFree();

        const auto oldDevIndex = Record::usedDeviceIndex;

        if(BASS_RecordInit(Record::deviceNumbersList[Record::usedDeviceIndex = deviceIndex]) == 0 &&
            BASS_RecordInit(Record::deviceNumbersList[Record::usedDeviceIndex = oldDevIndex]) == 0)
            return;

        Record::recordChannel = BASS_RecordStart(SV::kFrequency2, 1, !Record::recordStatus &&
            !Record::checkStatus ? BASS_RECORD_PAUSE : NULL, nullptr, nullptr);

        PluginConfig::SetDeviceName(Record::deviceNamesList[Record::usedDeviceIndex]);*/
    }

    void Record::SyncConfigs() noexcept
    {
        Record::SetMicroEnable(PluginConfig::GetMicroEnable());
        Record::SetMicroVolume(PluginConfig::GetMicroVolume());
    }

    void Record::ResetConfigs() noexcept
    {
        PluginConfig::SetMicroEnable(PluginConfig::kDefValMicroEnable);
        PluginConfig::SetMicroVolume(PluginConfig::kDefValMicroVolume);
    }

    const std::vector<std::string>& Record::GetDeviceNamesList() noexcept
    {
        return Record::deviceNamesList;
    }

    const std::vector<int>& Record::GetDeviceNumbersList() noexcept
    {
        return Record::deviceNumbersList;
    }

    bool Record::initStatus { false };

    bool Record::checkStatus { false };

    HRECORD Record::recordChannel { NULL };
    OpusEncoder* Record::encoder { nullptr };

    std::array<opus_int16, SV::kFrameSizeInSamples> Record::encBuffer {};

    HSTREAM Record::checkChannel { NULL };

    int Record::usedDeviceIndex { -1 };
    std::vector<std::string> Record::deviceNamesList;
    std::vector<int> Record::deviceNumbersList;

}