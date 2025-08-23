#include "CDebugInfo.h"
#include "game/Entity/Ped/Ped.h"
#include "game/Tasks/TaskManager.h"
#include "main.h"
#include "gui/gui.h"
#include "net/netgame.h"
#include "game/RW/RenderWare.h"
#include "chatwindow.h"
#include "playertags.h"
#include "keyboard.h"
#include "CSettings.h"
#include "util/patch.h"
#include "game/Timer.h"
#include "game/Pools.h"

extern CGUI* pGUI;

uint32_t CDebugInfo::m_uiDrawDebug = 0;
uint32_t CDebugInfo::m_uiDrawFPS = 0;

void CDebugInfo::ToggleDebugDraw()
{
	m_uiDrawDebug = !m_uiDrawDebug;
}

void CDebugInfo::SetDrawFPS(uint32_t bDraw)
{
	m_uiDrawFPS = bDraw;
}

extern char streamimgState[123];

void CDebugInfo::Draw()
{
	if (!CGame::IsToggledHUDElement(HUD_ELEMENT_FPS)) return;

	char szStr[30];
	char szStrMem[64];
	char szStrPos[64];

	ImVec2 pos = ImVec2(pGUI->ScaleX(40.0f), pGUI->ScaleY(1080.0f - pGUI->GetFontSize() * 10));

	if (m_uiDrawDebug || m_uiDrawFPS)
	{
        static float fps = 60.f;
        static auto lastTick = CTimer::m_snTimeInMillisecondsNonClipped;
        if(CTimer::m_snTimeInMillisecondsNonClipped - lastTick > 500) {
            lastTick = CTimer::m_snTimeInMillisecondsNonClipped;
            fps = std::clamp(CTimer::game_FPS, 10.f, (float) CSettings::maxFps);
        }
		snprintf(&szStr[0], sizeof(szStr), "FPS: %.0f", fps);

		pGUI->RenderText(pos, (ImU32)0xFFFFFFFF, true, &szStr[0]);

		auto &msUsed = CStreaming::ms_memoryUsed;
		auto &msAvailable = CStreaming::ms_memoryAvailable;

		struct mallinfo memInfo = mallinfo();
		int totalAllocatedMB  = memInfo.uordblks / (1024 * 1024);

		snprintf(&szStrMem[0], sizeof(szStrMem), "MEM: %d mb (stream %d/%d) (Tex %d MB)",
				 totalAllocatedMB,
				 msUsed / (1024 * 1024),
				 msAvailable / (1024 * 1024),
				 TextureDatabaseRuntime::storedTexels / (1024 * 1024)
				 );

        strcpy(streamimgState, szStrMem);

		pos = ImVec2(pGUI->ScaleX(40.0f), pGUI->ScaleY(1080.0f - pGUI->GetFontSize() * 9));

		pGUI->RenderText(pos, (ImU32)0xFFFFFFFF, true, &szStrMem[0]);

		if (CGame::FindPlayerPed()->m_pPed)
		{
			snprintf(&szStrPos[0], sizeof(szStrPos), "POS: %.2f, %.2f, %.2f", CGame::FindPlayerPed()->m_pPed->m_matrix->m_pos.x, CGame::FindPlayerPed()->m_pPed->m_matrix->m_pos.y, CGame::FindPlayerPed()->m_pPed->m_matrix->m_pos.z);
			pos = ImVec2(pGUI->ScaleX(40.0f), pGUI->ScaleY(1080.0f - pGUI->GetFontSize() * 8));
			pGUI->RenderText(pos, (ImU32)0xFFFFFFFF, true, &szStrPos[0]);
		}
		//Log("pools = %d mem = %d", GetPedPoolGta()->GetNoOfUsedSpaces(), totalAllocatedMB);
//		char debugPools[250];
//		snprintf(&debugPools[0], sizeof(debugPools), "NSingle: %d; NDouble: %d; Peds: %d; Veh's: %d; Obj: %d; EntryInf: %d; Dummies: %d, Buildings: %d",
//				 GetPtrNodeSingleLinkPool()->GetNoOfUsedSpaces(),
//				 GetPtrNodeDoubleLinkPool()->GetNoOfUsedSpaces(),
//				 GetPedPoolGta()->GetNoOfUsedSpaces(),
//				 GetVehiclePoolGta()->GetNoOfUsedSpaces(),
//				 GetObjectPoolGta()->GetNoOfUsedSpaces(),
//				 GetEntryInfoNodePool()->GetNoOfUsedSpaces(),
//				 GetDummyPool()->GetNoOfUsedSpaces(),
//				 GetBuildingPool()->GetNoOfUsedSpaces()
//				 );
//
//		pos = ImVec2(pGUI->ScaleX(40.0f), pGUI->ScaleY(1080.0f - pGUI->GetFontSize() * 1));
//		pGUI->RenderText(pos, (ImU32)0xFFFFFFFF, true, &debugPools[0]);
	}
}

void CDebugInfo::ApplyDebugPatches()
{

}