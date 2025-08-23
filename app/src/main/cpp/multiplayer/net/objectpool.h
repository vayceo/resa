#pragma once

#define INVALID_OBJECT_ID	0xFFF9

class CObjectPool : public CNetPool<CObjectSamp*>
{
public:
	static void Free();

	static bool New(uint16_t ObjectID, int iModel, CVector vecPos, CVector vecRot, float fDrawDistance = 0);
	static bool Delete(uint16_t ObjectID);

	static uint16_t FindIDFromGtaPtr(CEntity *pGtaObject);

	static CObjectSamp *GetObjectFromGtaPtr(CEntity *pGtaObject);

	static void Process();
};