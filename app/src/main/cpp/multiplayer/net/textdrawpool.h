#pragma once

/*

SA:MP Multiplayer Modification
Copyright 2004-2005 SA:MP Team

Version: $Id: textdrawpool.h,v 1.3 2008-02-15 07:20:36 kyecvs Exp $

*/

//----------------------------------------------------

#include "game/textdraw.h"

class CTextDrawPool
{
private:

    static inline std::list<uint16_t>   m_TextDraws;
	static inline CTextDraw*            m_pTextDraw[MAX_TEXT_DRAWS]{};
    static inline bool	                m_bSlotState[MAX_TEXT_DRAWS]{};

public:
    static inline bool			        m_bSelectState{};

    static void Init();
    static void Free();

    static void SetSelectState(bool bState, uint32_t dwColor);
    static CTextDraw* New(uint16_t wText, TEXT_DRAW_TRANSMIT* TextDrawTransmit, char* szText);
    static void Delete(uint16_t wText);
	static void Draw();
    static bool OnTouchEvent(int type, int num, int x, int y);
    static bool IsPointInRect(float x, float y, CRect* rect);

    static void SnapshotProcess();

    static CTextDraw* GetAt(uint16_t wText) {
		if (wText >= MAX_TEXT_DRAWS) return NULL;
		if (!m_bSlotState[wText]) return NULL;
		return m_pTextDraw[wText];
	};
};

//----------------------------------------------------
