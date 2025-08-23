//
// Created on 15.10.2023.
//

#include "GPS.h"
#include "net/netgame.h"
#include "net/playerpool.h"
#include "Entity/Ped/Ped.h"
#include "game.h"
#include "PathFind.h"
#include "java_systems/HUD.h"

void GPS::Draw() {
//    float gpsDistance = 0;
//    auto playa = CLocalPlayer::m_pPlayerPed;
//    if(!playa)return;
//    auto pPed = playa->m_pPed;
//    if(!pPed)return;
//    if (true) {
//        bool isGamePaused = CTimer::GetIsPaused(), bScissors = (!isGamePaused);
//        auto drawRadarOrMap = CTimer::GetIsPaused() || CHUD::bIsShow;
////        auto FrontEndMenuManager = (uintptr_t *) (g_libGTASA + 0x008ED7C0);
////        auto FrontEndMenuManager = (uintptr_t *) (g_libGTASA + 0x008ED7C0);
//
//        CPathFind &ThePaths = *(CPathFind *) (g_libGTASA + 0x00709894);
//
//        CVector destPosn = CVector(1868.0, -2034.0, 30.08);
//        destPosn.z = CWorld::FindGroundZForCoord(destPosn.x, destPosn.y);
//
//        short nodesCount = 0;
//
//        ThePaths.DoPathSearch(
//                PATH_TYPE_VEH,
//                pPed->GetPosition(),
//                CNodeAddress(),
//                destPosn,
//                resultNodes,
//                &nodesCount,
//                MAX_NODE_POINTS,
//                &gpsDistance,
//                999999.0f,
//                nullptr,
//                999999.0f,
//                false,
//                CNodeAddress(),
//                false,
//                false
//                //playa->pVehicle->m_nVehicleSubType == VEHICLE_SUBTYPE_BOAT
//        );
//
//        Log("nodesCount = %d = %d", nodesCount, gpsDistance);
//        if(nodesCount > 0) {
////            if (isTargetBlip && bScissors &&
////                gpsDistance < pCfgClosestMaxGPSDistance->GetFloat()) {
////                ClearRadarBlip(TargetBlip.m_nHandleIndex);
////                gMobileMenu->m_TargetBlipHandle.m_nHandleIndex = 0;
////                TargetBlip.m_nHandleIndex = 0;
////                return;
////            }
//
//            CPathNode *node;
//            CVector2D nodePos;
//            if (isGamePaused) {
//                for (int i = 0; i < nodesCount; ++i) {
//                    node = &ThePaths->pNodes[resultNodes[i].m_wAreaId][resultNodes[i].m_wNodeId];
//                    nodePos = node->GetPosition2D();
//                    TransformRealWorldPointToRadarSpace(nodePos, nodePos);
//                    LimitRadarPoint(nodePos);
//                    TransformRadarPointToScreenSpace(nodePoints[i], nodePos);
//                    nodePoints[i] *= flMenuMapScaling;
//                }
//            } else {
//                for (int i = 0; i < nodesCount; ++i) {
//                    node = &ThePaths->pNodes[resultNodes[i].m_nAreaId][resultNodes[i].m_nNodeId];
//                    nodePos = node->GetPosition2D();
//                    TransformRealWorldPointToRadarSpace(nodePos, nodePos);
//                    TransformRadarPointToScreenSpace(nodePoints[i], nodePos);
//                }
//            }
//
//            if (IsRadarVisible() || isGamePaused) {
//                if (bScissors) SetScissorRect(radarRect); // Scissor
//                RwRenderStateSet(rwRENDERSTATETEXTURERASTER, NULL);
//
//                unsigned int vertIndex = 0;
//                --nodesCount;
//
//                CVector2D point[4], shift[2], dir;
//                float angle;
//                if (isGamePaused) {
//                    float mp = gMobileMenu->m_fMapZoom - 140.0f;
//                    if (mp < 140.0f) mp = 140.0f;
//                    else if (mp > 960.0f) mp = 960.0f;
//                    mp = mp / 960.0f + 0.4f;
//                    mp *= lineWidth;
//
//                    for (int i = 0; i < nodesCount; i++) {
//                        dir = CVector2D::Diff(nodePoints[i + 1], nodePoints[i]);
//                        angle = atan2(dir.y, dir.x);
//
//                        sincosf(angle - 1.5707963f, &shift[0].y, &shift[0].x);
//                        shift[0] *= mp;
//                        sincosf(angle + 1.5707963f, &shift[1].y, &shift[1].x);
//                        shift[1] *= mp;
//
//                        Setup2DVertex(lineVerts[vertIndex], nodePoints[i].x + shift[0].x, nodePoints[i].y + shift[0].y, color);
//                        Setup2DVertex(lineVerts[++vertIndex], nodePoints[i + 1].x + shift[0].x, nodePoints[i + 1].y + shift[0].y, color);
//                        Setup2DVertex(lineVerts[++vertIndex], nodePoints[i].x + shift[1].x, nodePoints[i].y + shift[1].y, color);
//                        Setup2DVertex(lineVerts[++vertIndex], nodePoints[i + 1].x + shift[1].x, nodePoints[i + 1].y + shift[1].y, color);
//                        ++vertIndex;
//                    }
//                } else {
//                    for (int i = 0; i < nodesCount; i++) {
//                        dir = CVector2D::Diff(nodePoints[i + 1], nodePoints[i]);
//                        angle = atan2(dir.y, dir.x);
//
//                        sincosf(angle - 1.5707963f, &shift[0].y, &shift[0].x);
//                        shift[0] *= lineWidth;
//                        sincosf(angle + 1.5707963f, &shift[1].y, &shift[1].x);
//                        shift[1] *= lineWidth;
//
//                        Setup2DVertex(lineVerts[vertIndex], nodePoints[i].x + shift[0].x, nodePoints[i].y + shift[0].y, color);
//                        Setup2DVertex(lineVerts[++vertIndex], nodePoints[i + 1].x + shift[0].x, nodePoints[i + 1].y + shift[0].y, color);
//                        Setup2DVertex(lineVerts[++vertIndex], nodePoints[i].x + shift[1].x, nodePoints[i].y + shift[1].y, color);
//                        Setup2DVertex(lineVerts[++vertIndex], nodePoints[i + 1].x + shift[1].x, nodePoints[i + 1].y + shift[1].y, color);
//                        ++vertIndex;
//                    }
//                }
//                RwIm2DRenderPrimitive(rwPRIMTYPETRISTRIP, lineVerts, 4 * nodesCount);
//                //if (bScissors) SetScissorRect(emptyRect); // Scissor
//            }
//        }
//    }
}

void GPS::Setup2dVertex(RwIm2DVertex &vertex, float x, float y) {
    vertex.x = x;
    vertex.y = y;
    vertex.u = vertex.v = 0.0f;
    vertex.z = CSprite2d::NearScreenZ + 0.0001f;
    vertex.rhw = CSprite2d::RecipNearClip;
    vertex.emissiveColor = RWRGBALONG(GPS_LINE_R, GPS_LINE_G, GPS_LINE_B, GPS_LINE_A);
}