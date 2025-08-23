#pragma once

struct CText3DLabel
{
	char        text[2048+1];
	char        textWithoutColors[2048+1];
	uint32_t    color;
	CVector     pos;
	float       drawDistance;
	bool        useLineOfSight;
	PLAYERID    attachedToPlayerID;
	VEHICLEID   attachedToVehicleID;
	CVector     offsetCoords;
	float       m_fTrueX;
};

class CText3DLabelsPool : public CNetPool<CText3DLabel*>
{
public:
	static void DrawAttachedToPlayer(CText3DLabel* pLabel);
	static void DrawAttachedToVehicle(CText3DLabel* pLabel);

	static void DrawTextLabel(CText3DLabel* pLabel, const CVector* pPos);

public:
	static void Free();

	static void CreateTextLabel(int labelID, char* text, uint32_t color, CVector pos, float drawDistance, bool useLOS, PLAYERID attachedToPlayerID, VEHICLEID attachedToVehicleID);
	static void Delete(int labelID);
	static void Update3DLabel(int labelID, uint32_t color, char* text);
	static void Draw();

	static void DrawVehiclesInfo();
};