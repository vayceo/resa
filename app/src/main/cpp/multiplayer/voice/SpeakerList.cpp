#include "../main.h"
#include "../gui/gui.h"
#include "../game/common.h"
#include "../game/util.h"
#include "../game/game.h"
#include "../game/PedSamp.h"
#include "../net/netgame.h"
#include "../game/Entity/Ped/Ped.h"
#include "SpeakerList.h"

#include "PluginConfig.h"
#include "game/Render/Sprite.h"
#include "BlackList.h"

extern CGUI *pGUI;

namespace Voice {
bool SpeakerList::Init() noexcept
{
    if(SpeakerList::initStatus)
        return false;

    try
    {
        SpeakerList::tSpeakerIcon = CUtil::LoadTextureFromDB("gui", "speaker_icon");
    }
    catch (const std::exception& exception)
    {
        Log("[sv:err:speakerlist:init] : failed to create speaker icon");
        SpeakerList::tSpeakerIcon = nullptr;
        return false;
    }

    if(!PluginConfig::IsSpeakerLoaded())
    {
        PluginConfig::SetSpeakerLoaded(true);
    }

    SpeakerList::initStatus = true;

    return true;
}

void SpeakerList::Free() noexcept
{
    if(!SpeakerList::initStatus)
        return;

    SpeakerList::tSpeakerIcon = nullptr;

    SpeakerList::initStatus = false;
}

void SpeakerList::Show() noexcept
{
    SpeakerList::showStatus = true;
}

bool SpeakerList::IsShowed() noexcept
{
    return SpeakerList::showStatus;
}

void SpeakerList::Hide() noexcept
{
    SpeakerList::showStatus = false;
}

void SpeakerList::Render()
{
    if(!SpeakerList::initStatus || !SpeakerList::IsShowed())
        return;

    int curTextLine;
    char szText[64], szText2[64];
    ImVec2 textPos = ImVec2(pGUI->ScaleX(45), pGUI->ScaleY(480));

    for(auto &pair : CPlayerPool::spawnedPlayers) {
        auto pPlayer = pair.second;
        auto playerId = pair.first;

        if (const auto playerName = CPlayerPool::GetPlayerName(playerId); playerName) {

            if (!SpeakerList::playerStreams[playerId].empty()) {
                if (curTextLine < 4 && playerName && strlen(playerName)) {
                    ImVec2 curTextPos = textPos;

                    ImVec2 a = ImVec2(curTextPos.x, curTextPos.y);
                    ImVec2 b = ImVec2(curTextPos.x + PluginConfig::kDefValSpeakerIconSize, curTextPos.y + PluginConfig::kDefValSpeakerIconSize);
                    ImGui::GetBackgroundDrawList()->AddImage((ImTextureID) SpeakerList::tSpeakerIcon->raster, a, b);

                    curTextPos.x += PluginConfig::kDefValSpeakerIconSize + 5;
                    sprintf(szText, "%s (ID: %d) ", playerName, playerId);
                    pGUI->RenderText(curTextPos, 0xFFFFFFFF, true, szText);

                    for (const auto &streamInfo: SpeakerList::playerStreams[playerId]) {
                        if (streamInfo.second.GetColor() == NULL)
                            continue;

                        curTextPos.x += ImGui::CalcTextSize(szText).x;
                        sprintf(szText2, "[%s]", streamInfo.second.GetName().c_str());
                        pGUI->RenderText(curTextPos, streamInfo.second.GetColor(), true, szText2);
                        break;
                    }

                    textPos.y += PluginConfig::kDefValSpeakerIconSize;

                    curTextLine++;
                }
            }
        }
    }
}

void SpeakerList::Draw(CVector* vec, float fDist)
{
	CVector TagPos;

	TagPos = vec;

	TagPos.z += 0.25f + (fDist * 0.0475f);

	CVector Out;
    CSprite::CalcScreenCoors(TagPos, &Out, nullptr, nullptr, false, false);

	if(Out.z < 1.0f)
		return;

    ImVec2 pos = ImVec2(Out.x, Out.y);
	pos.x -= PluginConfig::kDefValSpeakerIconSize / 2;
    pos.y -= pGUI->GetFontSize();

    ImVec2 a = ImVec2(pos.x, pos.y);
    ImVec2 b = ImVec2(pos.x + PluginConfig::kDefValSpeakerIconSize, pos.y + PluginConfig::kDefValSpeakerIconSize);
    ImGui::GetBackgroundDrawList()->AddImage((ImTextureID)SpeakerList::tSpeakerIcon->raster, a, b);
}

void SpeakerList::OnSpeakerPlay(const Stream& stream, const uint16_t speaker) noexcept
{
    uint16_t wSpeaker = speaker;
    if(speaker < 0) wSpeaker = 0;
    else if(speaker > MAX_PLAYERS - 1) wSpeaker = MAX_PLAYERS - 1;
    if(speaker != wSpeaker) return;

    SpeakerList::playerStreams[speaker][(Stream*)(&stream)] = stream.GetInfo();
}

void SpeakerList::OnSpeakerStop(const Stream& stream, const uint16_t speaker) noexcept
{
    uint16_t wSpeaker = speaker;
    if(speaker < 0) wSpeaker = 0;
    else if(speaker > MAX_PLAYERS - 1) wSpeaker = MAX_PLAYERS - 1;
    if(speaker != wSpeaker) return;

    SpeakerList::playerStreams[speaker].erase((Stream*)(&stream));
}

std::array<std::unordered_map<Stream*, StreamInfo>, MAX_PLAYERS> SpeakerList::playerStreams;

bool SpeakerList::initStatus { false };
bool SpeakerList::showStatus { false };

RwTexture* SpeakerList::tSpeakerIcon { nullptr };
}