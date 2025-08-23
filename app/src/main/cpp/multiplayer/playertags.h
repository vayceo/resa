#pragma once
#include "gui/gui.h"

class CPlayerTags
{
public:
	static void Init();
	static void Shutdown();

	static void Render();
	static void AddChatBubble(PLAYERID playerId, char* szText, uint32_t dwColor, float fDistance, uint32_t dwTime);
	static void ResetChatBubble(PLAYERID playerId);
	static void DrawChatBubble(PLAYERID playerId, CVector* vec, float fDistance);
	static void Draw(CVector* vec, const char* szName, const float *fDist, CRemotePlayer* pPlayer);

private:
	static inline RwTexture* pMuteIconTex;
    static inline RwTexture* pSpeakIconTex;

	static inline ImVec2 HealthBarBDR1;
	static inline ImVec2 HealthBarBDR2;
	static inline ImVec2 HealthBarBG1;
	static inline ImVec2 HealthBarBG2;
	static inline ImVec2 HealthBar1;
	static inline ImVec2 HealthBar2;

	static inline ImColor HealthBarColor;
	static inline ImColor HealthBarBGColor;
	static inline ImColor HealthBarBDRColor;

	static inline RwTexture* m_pAfk_icon;
	static inline RwTexture* m_pKeyboardIconTex;

	static inline uint32_t m_dwColors[MAX_PLAYERS]{};
	static inline float m_fDistance[MAX_PLAYERS]{};
	static inline float m_fTrueX[MAX_PLAYERS]{};
	static inline uint32_t m_dwTime[MAX_PLAYERS]{};
	static inline uint32_t m_dwStartTime[MAX_PLAYERS]{};
	static inline char* m_pSzText[MAX_PLAYERS]{};
	static inline char* m_pSzTextWithoutColors[MAX_PLAYERS]{};
	static inline bool m_bChatBubbleStatus[MAX_PLAYERS]{};
	static inline int m_iOffset[MAX_PLAYERS]{};

    static void RenderActors();

	static void DrawHealthAndArmour(CVector *screenPos, float health, float armour);
};