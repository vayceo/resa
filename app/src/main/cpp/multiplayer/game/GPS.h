/*
    Plugin-SDK (Grand Theft Auto) source file
    Authors: GTA Community. See more here
    https://github.com/DK22Pac/plugin-sdk
    Do not delete this comment block. Respect others' work!
*/
#include "RenderWare.h"
#include "common.h"
#include "RenderWare.h"
#include "Radar.h"
#include "font.h"
#include "World.h"
#include "NodeAddress.h"
#include <cmath>

#define MAX_NODE_POINTS 2000
#define GPS_LINE_WIDTH  4.0f
#define GPS_LINE_R  180
#define GPS_LINE_G  24
#define GPS_LINE_B  24
#define GPS_LINE_A  255
#define MAX_TARGET_DISTANCE 20.0f

class GPS {
public:
    static inline bool gpsShown;
    static inline float gpsDistance;
    static inline CNodeAddress resultNodes[MAX_NODE_POINTS];
    static inline CVector2D nodePoints[MAX_NODE_POINTS];
    static inline RwIm2DVertex lineVerts[MAX_NODE_POINTS * 4];

//        Events::gameProcessEvent += []() {
//            if (FrontEndMenuManager.m_nTargetBlipIndex
//                && CRadar::ms_RadarTrace[LOWORD(FrontEndMenuManager.m_nTargetBlipIndex)].m_nCounter == HIWORD(FrontEndMenuManager.m_nTargetBlipIndex)
//                && CRadar::ms_RadarTrace[LOWORD(FrontEndMenuManager.m_nTargetBlipIndex)].m_nBlipDisplay
//                && FindPlayerPed()
//                && DistanceBetweenPoints(CVector2D(FindPlayerCoors(0)),
//                                         CVector2D(CRadar::ms_RadarTrace[LOWORD(FrontEndMenuManager.m_nTargetBlipIndex)].m_vecPos)) < MAX_TARGET_DISTANCE)
//            {
//                CRadar::ClearBlip(FrontEndMenuManager.m_nTargetBlipIndex);
//                FrontEndMenuManager.m_nTargetBlipIndex = 0;
//            }
//        };

//        Events::drawRadarOverlayEvent += []() {
//
//        };

//        Events::drawHudEvent += [] {
//            if (gpsShown) {
//                CFont::SetOrientation(ALIGN_CENTER);
//                CFont::SetColor(CRGBA(200, 200, 200, 255));
//                CFont::SetBackground(false, false);
//                CFont::SetWrapx(500.0f);
//                CFont::SetScale(0.4f * static_cast<float>(RsGlobal.maximumWidth) / 640.0f,
//                                0.8f * static_cast<float>(RsGlobal.maximumHeight) / 448.0f);
//                CFont::SetFontStyle(FONT_SUBTITLES);
//                CFont::SetProportional(true);
//                CFont::SetDropShadowPosition(1);
//                CFont::SetDropColor(CRGBA(0, 0, 0, 255));
//                CVector2D radarBottom;
//                CRadar::TransformRadarPointToScreenSpace(radarBottom, CVector2D(0.0f, -1.0f));
//                char text[16];
//                if (gpsDistance > 1000.0f)
//                    sprintf(text, "%.2fkm", gpsDistance / 1000.0f);
//                else
//                    sprintf(text, "%dm", static_cast<int>(gpsDistance));
//                CFont::PrintString(radarBottom.x, radarBottom.y + 8.0f * static_cast<float>(RsGlobal.maximumHeight) / 448.0f, text);
//            }
//        };
    static void Draw();

    static void Setup2dVertex(RwIm2DVertex &vertex, float x, float y);
};

//    static void Setup2dVertex(RwIm2DVertex &vertex, float x, float y) {
//        vertex.x = x;
//        vertex.y = y;
//        vertex.u = vertex.v = 0.0f;
//        vertex.z = CSprite2d::NearScreenZ + 0.0001f;
//        vertex.rhw = CSprite2d::RecipNearClip;
//        vertex.emissiveColor = RWRGBALONG(GPS_LINE_R, GPS_LINE_G, GPS_LINE_B, GPS_LINE_A);
//    }