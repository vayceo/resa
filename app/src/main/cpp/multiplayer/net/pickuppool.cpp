#include "../main.h"
#include "../game/game.h"
#include "../net/netgame.h"
#include "../CSettings.h"

void CPickupPool::Free()
{
	auto ids = GetAllIds();
	for (auto& id : ids) {
		Destroy(id);
	}
}

void CPickupPool::New(int pickupId, NEW_PICKUP_DATA *pPickup)
{
	if(GetAt(pickupId))
        Destroy(pickupId);

    auto newPickup = new PICKUP();
	newPickup->iModel = pPickup->iModel;
	newPickup->iType = pPickup->iType;
    newPickup->pos = pPickup->pos;

	newPickup->m_droppedWeapon.bDroppedWeapon = false;

	newPickup->dwGtaId = CGame::CreatePickup(pPickup->iModel, pPickup->iType, &newPickup->pos, &newPickup->m_iPickupID);

	list[pickupId] = newPickup;
}

void CPickupPool::Destroy(int pickupId)
{
	auto pPickup = GetAt(pickupId);
	if(pPickup)
	{
		auto modelId = pPickup->iModel;

		ScriptCommand(&destroy_pickup, pPickup->dwGtaId);

        delete pPickup;
		list.erase(pickupId);

		CStreaming::RemoveModelIfNoRefs(modelId);
	}
}

int CPickupPool::GetNumberFromID(int iPickup)
{
	for(auto &pair : list) {
		auto pPickup = pair.second;
		if (pPickup->m_iPickupID == iPickup)
			return pair.first;
	}

	return -1;
}

void CPickupPool::PickedUp(int iPickup)
{
	int index = GetNumberFromID(iPickup);

	if(index == -1)
		return;

    RakNet::BitStream bsPickup;
    bsPickup.Write(index);

    pNetGame->GetRakClient()->RPC(&RPC_PickedUpPickup, &bsPickup, HIGH_PRIORITY, RELIABLE_SEQUENCED, 0, false, UNASSIGNED_NETWORK_ID, 0);
}