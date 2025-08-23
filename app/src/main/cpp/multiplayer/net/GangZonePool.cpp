#include "../main.h"
#include "../game/game.h"
#include "netgame.h"

void CGangZonePool::Free()
{
    auto ids = GetAllIds();
    for (auto& id : ids) {
        Delete(id);
    }
}

void CGangZonePool::New(uint16_t zoneId, float fMinX, float fMinY, float fMaxX, float fMaxY, uint32_t dwColor)
{
	Delete(zoneId);

	auto pGangZone = new GANG_ZONE;

    pGangZone->rect = {fMinX, fMinY, fMaxX, fMaxY};
    pGangZone->dwColor = dwColor;
    pGangZone->dwAltColor = dwColor;

    list[zoneId] = pGangZone;
}

void CGangZonePool::Delete(uint16_t zoneId)
{
	if(GetAt(zoneId))
	{
        delete list[zoneId];
        list.erase(zoneId);
	}
}

void CGangZonePool::Draw()
{
	static uint32_t dwLastTick = 0;
	uint32_t dwTick = GetTickCount();
	static uint8_t alt = 0;

	if(dwTick - dwLastTick >= 500)
	{
		alt = ~alt;
		dwLastTick = dwTick;
	}

    for(auto& pair : list) {
        auto pZone = pair.second;

        CGame::DrawGangZone(&pZone->rect, alt ? pZone->dwAltColor : pZone->dwColor);
	}
}

void CGangZonePool::Flash(uint16_t zoneId, uint32_t dwColor)
{
    auto pZone = GetAt(zoneId);

	if(pZone)
        pZone->dwAltColor = dwColor;
}

void CGangZonePool::StopFlash(uint16_t zoneId)
{
    auto pZone = GetAt(zoneId);

	if(pZone)
        pZone->dwAltColor = pZone->dwColor;
}