#pragma once

#pragma pack(push, 1)
struct NEW_PICKUP_DATA
{
	int iModel;
	int iType;
	CVector pos;
};
#pragma pack(pop)

struct DROPPED_WEAPON
{
	bool bDroppedWeapon;
	PLAYERID fromPlayer;
};

struct PICKUP
{
    int iModel;
    int iType;
    CVector pos;

    uintptr dwGtaId;
	uint32 m_iPickupID{static_cast<uint32>(-1)};
	DROPPED_WEAPON m_droppedWeapon{};
};

class CPickupPool : CNetPool<PICKUP*>
{
public:
	static void Free();

	static void New(int pickupId, NEW_PICKUP_DATA *pPickup);
	static void Destroy(int iPickup);
	static void PickedUp(int iPickup);
	static void Process();

	static int GetNumberFromID(int id);
};