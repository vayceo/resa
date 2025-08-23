//
// Created on 23.11.2023.
//

#include "Snow.h"
#include "../main.h"
#include "game.h"
#include "../net/netgame.h"


static const char* g_iSnows[] = {"", "SNOWFXII", "SNOWFXIII", "SNOWFXIV"};

void CSnow::Initialise()
{
    m_dwParticeHandle1 = 0;
    m_dwParticeHandle2 = 0;
    m_dwLastTickCreated = GetTickCount();
}

#include "scripting.h"

void CSnow::Process()
{
    if (!m_iCurrentSnow)
        return;

    auto pPed = CLocalPlayer::GetPlayerPed();

    if (CGame::currArea && m_dwParticeHandle1 && m_dwParticeHandle2)
    {
        ScriptCommand(&destroy_particle, m_dwParticeHandle1);
        ScriptCommand(&destroy_particle, m_dwParticeHandle2);

        m_dwParticeHandle1 = 0;
        m_dwParticeHandle2 = 0;
        return;
    }

    if (!m_dwParticeHandle1 && !m_dwParticeHandle2)
    {
        if (m_iCurrentSnow < 0 || m_iCurrentSnow >= 4)
            m_iCurrentSnow = 1;

        uint32 dwActorhandle = pPed->m_dwGTAId;

        ScriptCommand(&attach_particle_to_actor, g_iSnows[m_iCurrentSnow], dwActorhandle, 0.0f, 10.0f, 2.0f, 1, &m_dwParticeHandle1);
        ScriptCommand(&make_particle_visible, m_dwParticeHandle1);

        ScriptCommand(&attach_particle_to_actor, g_iSnows[m_iCurrentSnow], dwActorhandle, 0.0f, 1.0f, 2.0f, 1, &m_dwParticeHandle2);
        ScriptCommand(&make_particle_visible, m_dwParticeHandle2);

        m_dwLastTickCreated = GetTickCount();
    }
    if (GetTickCount() - m_dwLastTickCreated >= 2000)
    {
        ScriptCommand(&destroy_particle, m_dwParticeHandle1);
        ScriptCommand(&destroy_particle, m_dwParticeHandle2);

        m_dwParticeHandle1 = 0;
        m_dwParticeHandle2 = 0;
    }
}

void CSnow::SetCurrentSnow(int iSnow)
{
    m_iCurrentSnow = iSnow;
}

int CSnow::GetCurrentSnow()
{
    return m_iCurrentSnow;
}