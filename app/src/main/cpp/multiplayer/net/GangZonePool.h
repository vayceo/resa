#pragma once

#include "NetPool.h"

struct GANG_ZONE
{
	CRect 		rect;
	uint32_t	dwColor;
	uint32_t 	dwAltColor;
};

class CGangZonePool : public CNetPool<GANG_ZONE*>
{
public:
	static void Free();

	static void New(uint16_t wZone, float fMinX, float fMinY, float fMaxX, float fMaxY, uint32_t dwColor);
	static void Delete(uint16_t wZone);

	static void Draw();

	static void Flash(uint16_t wZone, uint32_t dwColor);
	static void StopFlash(uint16_t wZone);
};