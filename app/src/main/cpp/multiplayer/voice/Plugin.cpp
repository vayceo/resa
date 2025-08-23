#include "../main.h"
#include "../gui/gui.h"

#include "Plugin.h"

#include "include/util/Timer.h"

#include "Record.h"
#include "Playback.h"
#include "Network.h"
//#include "BlackList.h"
#include "PluginConfig.h"
#include "SpeakerList.h"
//#include "PluginMenu.h"
#include "GlobalStream.h"
#include "StreamAtPoint.h"
#include "StreamAtVehicle.h"
#include "StreamAtPlayer.h"
#include "StreamAtObject.h"
#include "game/Widgets/TouchInterface.h"
#include "BlackList.h"

extern CGUI *pGUI;

namespace Voice {
bool CVoicePlugin::OnPluginLoad() noexcept
{
    if(!CVoiceRender::Init())
    {
       DLOG("[sv:err:plugin] : failed to init render module");
        return false;
    }

    CVoiceRender::AddDeviceInitCallback(CVoicePlugin::OnDeviceInit);
    CVoiceRender::AddRenderCallback(CVoicePlugin::OnRender);
    CVoiceRender::AddDeviceFreeCallback(CVoicePlugin::OnDeviceFree);

    return true;
}

bool CVoicePlugin::OnSampLoad() noexcept
{
    if(!Samp::Init())
    {
       DLOG("[sv:err:plugin] : failed to init samp");
        CVoiceRender::Free();
        return false;
    }

    Samp::AddLoadCallback(CVoicePlugin::OnInitGame);
    Samp::AddExitCallback(CVoicePlugin::OnExitGame);

    if(!Network::Init())
    {
       DLOG("[sv:err:plugin] : failed to init network");
        CVoiceRender::Free();
        Samp::Free();
        return false;
    }

    Network::AddConnectCallback(CVoicePlugin::ConnectHandler);
    Network::AddSvConnectCallback(CVoicePlugin::PluginConnectHandler);
    Network::AddSvInitCallback(CVoicePlugin::PluginInitHandler);
    Network::AddDisconnectCallback(CVoicePlugin::DisconnectHandler);

    if(!Playback::Init())
    {
       DLOG("[sv:err:plugin] : failed to init playback");
        CVoiceRender::Free();
        Samp::Free();
        Network::Free();
        return false;
    }

    return true;
}

void CVoicePlugin::OnInitGame() noexcept
{
    // ~ none
}

void CVoicePlugin::OnExitGame() noexcept
{
    Network::Free();

    CVoicePlugin::streamTable.clear();

    Record::Free();
    Playback::Free();
}

void CVoicePlugin::MainLoop()
{
    if(!Samp::IsLoaded()) return;

    while(auto controlPacket = Network::ReceiveControlPacket())
    {
        CVoicePlugin::ControlPacketHandler(controlPacket);
        delete controlPacket->data;
        delete controlPacket;
    }

    while(const auto voicePacket = Network::ReceiveVoicePacket())
    {
        const auto& voicePacketRef = *voicePacket;

        const auto iter = CVoicePlugin::streamTable.find(voicePacketRef->stream);
        if(iter == CVoicePlugin::streamTable.end()) continue;

        iter->second->Push(*&voicePacketRef);
    }

    for(const auto& stream : CVoicePlugin::streamTable)
        stream.second->Tick();

    //Playback::Tick();
    Record::Tick();

    int defaultKey = 0x42;

    if (CTouchInterface::m_pWidgets[WidgetIDs::WIDGET_PHONE]->IsTouched(nullptr)) {
        if (PluginConfig::GetMicroEnable()) {
            if (!CVoicePlugin::recordStatus) {
               DLOG("[sv:dbg:plugin] : activation key(%hhu) pressed", defaultKey);

                if (!CVoicePlugin::recordBusy) {
                    CVoicePlugin::recordStatus = true;
                    Record::StartRecording();
                }

                SV::PressKeyPacket pressKeyPacket{};

                pressKeyPacket.keyId = defaultKey;

                if (!Network::SendControlPacket(SV::ControlPacketType::pressKey, &pressKeyPacket, sizeof(pressKeyPacket)))
                   DLOG("[sv:err:main:HookWndProc] : failed to send PressKey packet");
            }
        }
    }
    else {
        if (CVoicePlugin::recordStatus) {
           DLOG("[sv:dbg:plugin] : activation key(%hhu) released", defaultKey);

            if (!CVoicePlugin::recordBusy) {
                CVoicePlugin::recordStatus = false;
            }

            SV::ReleaseKeyPacket releaseKeyPacket{};

            releaseKeyPacket.keyId = defaultKey;

            if (!Network::SendControlPacket(SV::ControlPacketType::releaseKey, &releaseKeyPacket, sizeof(releaseKeyPacket)))
               DLOG("[sv:err:main:HookWndProc] : failed to send ReleaseKey packet");
        }
    }


    uint8_t frameBuffer[Network::kMaxVoiceDataSize];
    if(const auto frameSize = Record::GetFrame(frameBuffer, sizeof(frameBuffer)))
    {
        if(!Network::SendVoicePacket(frameBuffer, frameSize))
           DLOG("[sv:err:plugin] : failed to send voice packet");

        if(!CVoicePlugin::recordStatus)
        {
            Record::StopRecording();
            Network::EndSequence();
        }
    }
}

void CVoicePlugin::ConnectHandler(const std::string& serverIp, const uint16_t serverPort)
{
    // ~ none
}

void CVoicePlugin::PluginConnectHandler(SV::ConnectPacket& connectStruct)
{
    connectStruct.signature = SV::kSignature;
    connectStruct.version = SV::kVersion;
    connectStruct.micro = Record::HasMicro();
}

bool CVoicePlugin::PluginInitHandler(const SV::PluginInitPacket& initPacket)
{
    CVoicePlugin::muteStatus = initPacket.mute;

    if(!Record::Init(initPacket.bitrate))
    {
       DLOG("[sv:inf:plugin:packet:init] : failed init record");
    }

    return true;
}

void CVoicePlugin::ControlPacketHandler(ControlPacket* controlPacket)
{
   DLOG("ControlPacketHandler(ControlPacket* controlPacket)");

    switch(controlPacket->packet)
    {
        case SV::ControlPacketType::muteEnable:
        {
            if(controlPacket->length != 0) break;

           DLOG("[sv:dbg:plugin:muteenable]");

            CVoicePlugin::muteStatus = true;
            CVoicePlugin::recordStatus = false;
            CVoicePlugin::recordBusy = false;
        }
        break;
        case SV::ControlPacketType::muteDisable:
        {
            if(controlPacket->length != 0) break;

           DLOG("[sv:dbg:plugin:mutedisable]");

            CVoicePlugin::muteStatus = false;
        }
        break;
        case SV::ControlPacketType::startRecord:
        {
            if(controlPacket->length != 0) break;

           DLOG("[sv:dbg:plugin:startrecord]");

            if(CVoicePlugin::muteStatus) break;

            CVoicePlugin::recordBusy = true;
            CVoicePlugin::recordStatus = true;

            Record::StartRecording();
        }
        break;
        case SV::ControlPacketType::stopRecord:
        {
            if(controlPacket->length != 0) break;

           DLOG("[sv:dbg:plugin:stoprecord]");

            if(CVoicePlugin::muteStatus) break;

            CVoicePlugin::recordStatus = false;
            CVoicePlugin::recordBusy = false;
        }
        break;
        case SV::ControlPacketType::addKey:
        {
            const auto& stData = *reinterpret_cast<const SV::AddKeyPacket*>(controlPacket->data);
            if(controlPacket->length != sizeof(stData)) break;

           DLOG("[sv:dbg:plugin:addkey] : keyid(0x%hhx)", stData.keyId);
           DLOG("[dbg:keyfilter] : adding key (0x%hhx)", stData.keyId); // xd fake

            //KeyFilter::AddKey(stData.keyId);
        }
        break;
        case SV::ControlPacketType::removeKey:
        {
            const auto& stData = *reinterpret_cast<const SV::RemoveKeyPacket*>(controlPacket->data);
            if(controlPacket->length != sizeof(stData)) break;

           DLOG("[sv:dbg:plugin:removekey] : keyid(0x%hhx)", stData.keyId);
           DLOG("[dbg:keyfilter] : removing key (0x%hhx)", stData.keyId); // xd fake

            //KeyFilter::RemoveKey(stData.keyId);
        }
        break;
        case SV::ControlPacketType::removeAllKeys:
        {
            if(controlPacket->length) break;

           DLOG("[sv:dbg:plugin:removeallkeys]");
           DLOG("[dbg:keyfilter] : removing all keys"); // xd fake

            //KeyFilter::RemoveAllKeys();
        }
        break;
        case SV::ControlPacketType::createGStream:
        {
            const auto& stData = *reinterpret_cast<const SV::CreateGStreamPacket*>(controlPacket->data);
            if(controlPacket->length < sizeof(stData)) break;

            DLOG("[sv:dbg:plugin:creategstream] : stream(%p), color(0x%x), name(%s)",
                stData.stream, stData.color, stData.color ? stData.name : "");

            const auto& streamPtr = CVoicePlugin::streamTable[stData.stream] =
                MakeGlobalStream(stData.color, stData.name);

            streamPtr->AddPlayCallback(SpeakerList::OnSpeakerPlay);
            streamPtr->AddStopCallback(SpeakerList::OnSpeakerStop);
        }
        break;
        case SV::ControlPacketType::createLPStream:
        {
            const auto& stData = *reinterpret_cast<const SV::CreateLPStreamPacket*>(controlPacket->data);
            if(controlPacket->length < sizeof(stData)) break;

            DLOG("[sv:dbg:plugin:createlpstream] : "
                "stream(%p), dist(%.2f), pos(%.2f;%.2f;%.2f), color(0x%x), name(%s)",
                stData.stream, stData.distance, stData.position.x, stData.position.y, stData.position.z,
                stData.color, stData.color ? stData.name : "");

            const auto& streamPtr = CVoicePlugin::streamTable[stData.stream] =
                MakeStreamAtPoint(stData.color, stData.name, stData.distance, stData.position);

            streamPtr->AddPlayCallback(SpeakerList::OnSpeakerPlay);
            streamPtr->AddStopCallback(SpeakerList::OnSpeakerStop);
        }
        break;
        case SV::ControlPacketType::createLStreamAtVehicle:
        {
            const auto& stData = *reinterpret_cast<const SV::CreateLStreamAtPacket*>(controlPacket->data);
            if(controlPacket->length < sizeof(stData)) break;

            DLOG("[sv:dbg:plugin:createlstreamatvehicle] : "
                "stream(%p), dist(%.2f), vehicle(%hu), color(0x%x), name(%s)",
                stData.stream, stData.distance, stData.target,
                stData.color, stData.color ? stData.name : "");

            const auto& streamPtr = CVoicePlugin::streamTable[stData.stream] =
                MakeStreamAtVehicle(stData.color, stData.name, stData.distance, stData.target);

            streamPtr->AddPlayCallback(SpeakerList::OnSpeakerPlay);
            streamPtr->AddStopCallback(SpeakerList::OnSpeakerStop);
        }
        break;
        case SV::ControlPacketType::createLStreamAtPlayer:
        {
            const auto& stData = *reinterpret_cast<const SV::CreateLStreamAtPacket*>(controlPacket->data);
            if(controlPacket->length < sizeof(stData)) break;

            DLOG("[sv:dbg:plugin:createlstreamatplayer] : "
                "stream(%p), dist(%.2f), player(%hu), color(0x%x), name(%s)",
                stData.stream, stData.distance, stData.target,
                stData.color, stData.color ? stData.name : "");

            const auto& streamPtr = CVoicePlugin::streamTable[stData.stream] =
                MakeStreamAtPlayer(stData.color, stData.name, stData.distance, stData.target);

            streamPtr->AddPlayCallback(SpeakerList::OnSpeakerPlay);
            streamPtr->AddStopCallback(SpeakerList::OnSpeakerStop);
        }
        break;
        case SV::ControlPacketType::createLStreamAtObject:
        {
            const auto& stData = *reinterpret_cast<const SV::CreateLStreamAtPacket*>(controlPacket->data);
            if(controlPacket->length < sizeof(stData)) break;

            DLOG("[sv:dbg:plugin:createlstreamatobject] : "
                "stream(%p), dist(%.2f), object(%hu), color(0x%x), name(%s)",
                stData.stream, stData.distance, stData.target,
                stData.color, stData.color ? stData.name : "");

            const auto& streamPtr = CVoicePlugin::streamTable[stData.stream] =
                MakeStreamAtObject(stData.color, stData.name, stData.distance, stData.target);

            streamPtr->AddPlayCallback(SpeakerList::OnSpeakerPlay);
            streamPtr->AddStopCallback(SpeakerList::OnSpeakerStop);
        }
        break;
        case SV::ControlPacketType::updateLStreamDistance:
        {
            const auto& stData = *reinterpret_cast<const SV::UpdateLStreamDistancePacket*>(controlPacket->data);
            if(controlPacket->length != sizeof(stData)) break;

           DLOG("[sv:dbg:plugin:updatelpstreamdistance] : stream(%p), dist(%.2f)",
                stData.stream, stData.distance);

            const auto iter = CVoicePlugin::streamTable.find(stData.stream);
            if(iter == CVoicePlugin::streamTable.end()) break;

            static_cast<LocalStream*>(iter->second.get())->SetDistance(stData.distance);
        }
        break;
        case SV::ControlPacketType::updateLPStreamPosition:
        {
            const auto& stData = *reinterpret_cast<const SV::UpdateLPStreamPositionPacket*>(controlPacket->data);
            if(controlPacket->length != sizeof(stData)) break;

           DLOG("[sv:dbg:plugin:updatelpstreamcoords] : stream(%p), pos(%.2f;%.2f;%.2f)",
                stData.stream, stData.position.x, stData.position.y, stData.position.z);

            const auto iter = CVoicePlugin::streamTable.find(stData.stream);
            if(iter == CVoicePlugin::streamTable.end()) break;

            static_cast<StreamAtPoint*>(iter->second.get())->SetPosition(stData.position);
        }
        break;
        case SV::ControlPacketType::deleteStream:
        {
            const auto& stData = *reinterpret_cast<const SV::DeleteStreamPacket*>(controlPacket->data);
            if (controlPacket->length != sizeof(stData)) break;

            DLOG("[sv:dbg:plugin:deletestream] : stream(%p)", stData.stream);

            CVoicePlugin::streamTable.erase(stData.stream);
        }
        break;
        case SV::ControlPacketType::setStreamParameter:
        {
            const auto& stData = *reinterpret_cast<const SV::SetStreamParameterPacket*>(controlPacket->data);
            if(controlPacket->length != sizeof(stData)) break;

           DLOG("[sv:dbg:plugin:streamsetparameter] : stream(%p), parameter(%hhu), value(%.2f)",
                stData.stream, stData.parameter, stData.value);

            const auto iter = CVoicePlugin::streamTable.find(stData.stream);
            if(iter == CVoicePlugin::streamTable.end()) break;

            iter->second->SetParameter(stData.parameter, stData.value);
        }
        break;
        case SV::ControlPacketType::slideStreamParameter:
        {
            const auto& stData = *reinterpret_cast<const SV::SlideStreamParameterPacket*>(controlPacket->data);
            if(controlPacket->length != sizeof(stData)) break;

           DLOG("[sv:dbg:plugin:streamslideparameter] : "
                "stream(%p), parameter(%hhu), startvalue(%.2f), endvalue(%.2f), time(%u)",
                stData.stream, stData.parameter, stData.startvalue, stData.endvalue, stData.time);

            const auto iter = CVoicePlugin::streamTable.find(stData.stream);
            if(iter == CVoicePlugin::streamTable.end()) break;

            iter->second->SlideParameter(stData.parameter, stData.startvalue, stData.endvalue, stData.time);
        }
        break;
        case SV::ControlPacketType::createEffect:
        {
            const auto& stData = *reinterpret_cast<const SV::CreateEffectPacket*>(controlPacket->data);
            if(controlPacket->length < sizeof(stData)) break;

           DLOG("[sv:dbg:plugin:effectcreate] : "
                "stream(%p), effect(%p), number(%hhu), priority(%d)",
                stData.stream, stData.effect, stData.number, stData.priority);

            const auto iter = CVoicePlugin::streamTable.find(stData.stream);
            if(iter == CVoicePlugin::streamTable.end()) break;

            iter->second->EffectCreate(stData.effect, stData.number, stData.priority,
                stData.params, controlPacket->length - sizeof(stData));
        }
        break;
        case SV::ControlPacketType::deleteEffect:
        {
            const auto& stData = *reinterpret_cast<const SV::DeleteEffectPacket*>(controlPacket->data);
            if(controlPacket->length != sizeof(stData)) break;

           DLOG("[sv:dbg:plugin:effectdelete] : stream(%p), effect(%p)",
                stData.stream, stData.effect);

            const auto iter = CVoicePlugin::streamTable.find(stData.stream);
            if(iter == CVoicePlugin::streamTable.end()) break;

            iter->second->EffectDelete(stData.effect);
        }
        break;
    }
}

void CVoicePlugin::DisconnectHandler()
{
    CVoicePlugin::streamTable.clear();

    CVoicePlugin::muteStatus = false;
    CVoicePlugin::recordStatus = false;
    CVoicePlugin::recordBusy = false;

    Record::Free();
}

void CVoicePlugin::OnDeviceInit()
{
    SpeakerList::Init();
}

void CVoicePlugin::OnRender()
{
    Timer::Tick();
    CVoicePlugin::MainLoop();
    SpeakerList::Render();
}

void CVoicePlugin::OnDeviceFree()
{
    SpeakerList::Free();
}

bool CVoicePlugin::muteStatus {false };
bool CVoicePlugin::recordStatus {false };
bool CVoicePlugin::recordBusy {false };

std::map<uint32_t, StreamPtr> CVoicePlugin::streamTable;
}