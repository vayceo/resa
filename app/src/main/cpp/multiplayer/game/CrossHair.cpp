//
// Created on 10.11.2023.
//

#include "CrossHair.h"
#include "common.h"
#include "../util/util.h"
#include "../main.h"
#include "../util/patch.h"
#include "../net/netgame.h"
#include "game.h"
#include "Entity/Ped/PlayerPed.h"

void CCrossHair::Init()
{
    Log("CCrossHair::Init");
    pCircleTex = new CSprite2d();
    pCircleTex->m_pTexture = CUtil::LoadTextureFromDB("txd", "siteM16");
    pSniperTex = new CSprite2d();
    pSniperTex->m_pTexture = CUtil::LoadTextureFromDB("txd", "sniperCross");
}

bool CCrossHair::IsCircleCrosshairMode(eCamMode mode)
{
    return mode == MODE_AIMWEAPON ||
           mode == MODE_DW_CAM_MAN ||
           (mode & 0xFFFD) == MODE_AIMWEAPON_ATTACHED;
}

bool CCrossHair::IsSniperCrosshairMode(eCamMode mode)
{
    return mode == MODE_SNIPER || mode == MODE_ROCKETLAUNCHER_HS;
}

void CCrossHair::Render()
{
    const auto pPed = GamePool_FindPlayerPed();
    if (!pPed)
        return;

    const auto camMode = static_cast<eCamMode>(GameGetLocalPlayerCameraMode());
    if (IsCircleCrosshairMode(camMode))
    {
        static float fCHairScreenMultX = (RsGlobal->maximumWidth - (RsGlobal->maximumHeight / 9 * 16)) / 2 + ((RsGlobal->maximumHeight / 9 * 16) * 0.524);
        static float fFixedOffset = RsGlobal->maximumWidth / (RsGlobal->maximumWidth - (RsGlobal->maximumHeight / 9 * 16)) * 2.0;
        const auto gunRadius = reinterpret_cast<CPlayerPed*>(pPed)->GetWeaponRadiusOnScreen();
        static float fCHairScreenMultY = (RsGlobal->maximumHeight / 9 * 16) / 10 * 6 * 0.4 + fFixedOffset;

        RwRenderStateSet(rwRENDERSTATETEXTUREFILTER, RWRSTATE(rwFILTERLINEAR));
        RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, RWRSTATE(FALSE));

        float f1 = ((RsGlobal->maximumHeight / 448.0) * 64.0) * gunRadius;

        float fPosX1 = ((f1 * 0.5) + fCHairScreenMultX) - f1;
        float fPosY1 = ((f1 * 0.5) + fCHairScreenMultY) - f1;

        float fPosX2 = (f1 * 0.5) + fPosX1;
        float fPosY2 = (f1 * 0.5) + fPosY1;

        pCircleTex->Draw(CRect{fPosX1, fPosY1, fPosX2, fPosY2}, CRGBA(255, 255, 255, 255));
        pCircleTex->Draw(CRect{fPosX1 + f1, fPosY1, fPosX2, fPosY2}, CRGBA(255, 255, 255, 255));
        pCircleTex->Draw(CRect{fPosX1, fPosY1 + f1, fPosX2, fPosY2}, CRGBA(255, 255, 255, 255));
        pCircleTex->Draw(CRect{fPosX1 + f1, fPosY1 + f1, fPosX2, fPosY2}, CRGBA(255, 255, 255, 255));
        m_UsedCrossHair = false;
        return;
    }
    else if (IsSniperCrosshairMode(camMode))
    {
        CRGBA white {255, 255, 255, 255};
        pSniperTex->Draw(0.0f, 0.0f, RsGlobal->maximumWidth, RsGlobal->maximumHeight, &white);
        m_UsedCrossHair = true;
        return;
    }
    m_UsedCrossHair = false;
}