#pragma once

#include "include/util/Render.h"
#include "include/util/Samp.h"

#include "ControlPacket.h"
#include "Stream.h"
#include "Header.h"

namespace Voice {
class CVoicePlugin {
    CVoicePlugin() = delete;
    ~CVoicePlugin() = delete;
    CVoicePlugin(const CVoicePlugin&) = delete;
    CVoicePlugin(CVoicePlugin&&) = delete;
    CVoicePlugin& operator=(const CVoicePlugin&) = delete;
    CVoicePlugin& operator=(CVoicePlugin&&) = delete;

public:
    static bool OnPluginLoad() noexcept;
    static bool OnSampLoad() noexcept;

private:
    static void OnInitGame() noexcept;
    static void OnExitGame() noexcept;

    static void MainLoop();

    static void ConnectHandler(const std::string& serverIp, uint16_t serverPort);
    static void PluginConnectHandler(SV::ConnectPacket& connectStruct);
    static bool PluginInitHandler(const SV::PluginInitPacket& initPacket);
    static void ControlPacketHandler(ControlPacket* controlPacket);
    static void DisconnectHandler();

    static void OnDeviceInit();
    static void OnRender();
    static void OnDeviceFree();

private:
    static bool muteStatus;
    static bool recordStatus;
    static bool recordBusy;

    static std::map<uint32_t, StreamPtr> streamTable;
};
}