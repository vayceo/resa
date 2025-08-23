#include "../main.h"
#include "../game/game.h"
#include "netgame.h"
#include "../gui/gui.h"
#include "textdrawpool.h"
#include "game/textdraw.h"

extern CGUI* pGUI;

void CTextDrawPool::Init()
{

}

void CTextDrawPool::Free()
{
	int x = 0;
	while (x != MAX_TEXT_DRAWS) {
		m_pTextDraw[x] = NULL;
		m_bSlotState[x] = FALSE;
		DestroyTextDrawTexture(x);
		x++;
	}
	m_TextDraws.clear();
	m_bSelectState = false;
}

CTextDraw* CTextDrawPool::New(uint16_t wText, TEXT_DRAW_TRANSMIT* TextDrawTransmit, char* szText)
{
	if (wText > MAX_TEXT_DRAWS || wText < 0) return nullptr;
	if (m_pTextDraw[wText]) Delete(wText);

	auto* pTextDraw = new CTextDraw(TextDrawTransmit, szText);

	if (pTextDraw)
	{
		m_pTextDraw[wText] = pTextDraw;
		m_bSlotState[wText] = TRUE;
		m_TextDraws.push_back(wText);

		return pTextDraw;
	}
	return NULL;
}

void CTextDrawPool::Delete(uint16_t wText)
{
	if (m_pTextDraw[wText])
	{
		delete m_pTextDraw[wText];
		m_pTextDraw[wText] = NULL;
		m_bSlotState[wText] = FALSE;
		m_TextDraws.remove(wText);
	}
}

void CTextDrawPool::Draw()
{
	for (uint16_t x: m_TextDraws)
	{
		if (m_bSlotState[x])
		{
			m_pTextDraw[x]->Draw();
		}
	}
}

bool CTextDrawPool::OnTouchEvent(int type, int num, int x, int y)
{
    if (!m_bSelectState) return true;
    static bool bWannaClick = false;

    for (uint16_t id: m_TextDraws)
    {
        if (m_bSlotState[id] && m_pTextDraw[id])
        {
            if (!m_pTextDraw[id]->m_TextDrawData.byteSelectable)
            {
                id++;
                continue;
            }

            switch (type)
            {
                case TOUCH_PUSH:
                    if (IsPointInRect(x, y, &(m_pTextDraw[id]->m_rectArea)))
                    {
                        bWannaClick = true;
                        return false;
                    }
                    break;

                case TOUCH_POP:
                    if (IsPointInRect(x, y, &(m_pTextDraw[id]->m_rectArea)))
                    {
                        CGUI::PushToBufferedQueueTextDrawPressed((uint16_t)id);
                        return false;
                    }
                    bWannaClick = false;
                    break;

                case TOUCH_MOVE:
                    break;
            }
        }
        id++;
    }

    return true;
}

bool CTextDrawPool::IsPointInRect(float x, float y, CRect* rect)
{
    if (x >= rect->left && x <= rect->right &&
        y >= rect->bottom && y <= rect->top)
        return true;

    return false;
}

void CTextDrawPool::SetSelectState(bool bState, uint32_t dwColor)
{
	if (bState)
	{
		m_bSelectState = true;
		//pGame->DisplayHUD(false);
		CGame::FindPlayerPed()->TogglePlayerControllable(false);
	}
	else {
		m_bSelectState = false;
		//pGame->DisplayHUD(true);
		CGame::FindPlayerPed()->TogglePlayerControllable(true);
	}
}

void CTextDrawPool::SnapshotProcess()
{
	for (uint16_t i: m_TextDraws)
	{
		if (m_bSlotState[i] && m_pTextDraw[i]) {
			m_pTextDraw[i]->SnapshotProcess();
		}
	}
}