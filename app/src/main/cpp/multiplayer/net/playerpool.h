#pragma once

#include <unordered_map>
#include "main.h"
#include "NetPool.h"

class CPlayerPool : public CNetPool<CRemotePlayer*>
{
public:
	static void Free();

	static bool Process();

	static void UpdateScore(PLAYERID playerId, int iScore);
	static void UpdatePing(PLAYERID playerId, uint32_t dwPing);

	static int GetLocalPlayerScore() { return m_iLocalPlayerScore; }
	static uint32_t GetLocalPlayerPing() { return m_dwLocalPlayerPing; }

	static int GetRemotePlayerScore(PLAYERID playerId)
	{
		if (playerId > MAX_PLAYERS) return 0;
		return m_iPlayerScores[playerId];
	}

	static uint32_t GetRemotePlayerPing(PLAYERID playerId)
	{
		if (playerId > MAX_PLAYERS) return 0;
		return m_dwPlayerPings[playerId];
	}

	static void SetLocalPlayerName(const char* szName) { strcpy(m_szLocalPlayerName, szName); }
	static char* GetLocalPlayerName() { return m_szLocalPlayerName; }
	static void SetLocalPlayerID(PLAYERID MyPlayerID)
	{
		strcpy(m_szPlayerNames[MyPlayerID], m_szLocalPlayerName);
		m_LocalPlayerID = MyPlayerID;
	}
	static PLAYERID GetLocalPlayerID() { return m_LocalPlayerID; }
	// remote
	static bool New(PLAYERID playerId, char* szPlayerName, bool bIsNPC);
	static bool Delete(PLAYERID playerId, uint8_t byteReason);

	static void SetPlayerName(PLAYERID playerId, char* szName) { strcpy(m_szPlayerNames[playerId], szName); }
	static char* GetPlayerName(PLAYERID playerId) {
		if(playerId == GetLocalPlayerID()) {
			return GetLocalPlayerName();
		}
		if(GetAt(playerId)) {
			return m_szPlayerNames[playerId];
		}
		return " ";
	}

	static PLAYERID FindRemotePlayerIDFromGtaPtr(CEntity * pActor);

	static void ResetCollisionChecking();
	static void ApplyCollisionChecking();

	static CRemotePlayer* GetSpawnedPlayer(PLAYERID playerid)
	{
		auto it = spawnedPlayers.find(playerid);

		return it != spawnedPlayers.end() ? it->second : nullptr;
	}

	static inline std::unordered_map<PLAYERID, CRemotePlayer*> spawnedPlayers {};
private:
	// LOCAL
	static inline PLAYERID		m_LocalPlayerID;
	static inline char			m_szLocalPlayerName[MAX_PLAYER_NAME+1];
	static inline int			m_iLocalPlayerScore;
	static inline uint32_t		m_dwLocalPlayerPing;

	//static inline bool			m_bPlayerSlotState[MAX_PLAYERS];
	static inline char			m_szPlayerNames[MAX_PLAYERS][MAX_PLAYER_NAME+1];
	static inline int			m_iPlayerScores[MAX_PLAYERS];
	static inline uint32_t		m_dwPlayerPings[MAX_PLAYERS];
	static inline bool 			m_bCollisionChecking[MAX_PLAYERS];

};