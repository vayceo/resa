#pragma once

#include "game/Entity/Entity.h"

CPed* GamePool_FindPlayerPed();
CPed* GamePool_Ped_GetAt(int iID);
int GamePool_Ped_GetIndex(CPed *pActor);

CVehicle *GamePool_Vehicle_GetAt(int iID);
int GamePool_Vehicle_GetIndex(CVehicle *pVehicle);

CPhysical *GamePool_Object_GetAt(int iID);

int LineOfSight(CVector* start, CVector* end, void* colpoint, uintptr_t ent,
	char buildings, char vehicles, char peds, char objects, char dummies, bool seeThrough, bool camera, bool unk);

bool IsPedModel(unsigned int uiModel);
bool IsValidModel(unsigned int uiModelID);

void InitPlayerPedPtrRecords();
void SetPlayerPedPtrRecord(uint8_t bytePlayer, uintptr_t dwPedPtr);
uint8_t FindPlayerNumFromPedPtr(uintptr_t dwPedPtr);

void SetScissorRect(void* pRect);
float DegToRad(float fDegrees);
// 0.3.7
float FloatOffset(float f1, float f2);

const char* GetAnimByIdx(int idx);

void ProjectMatrix(CVector* vecOut, CMatrix* mat, CVector* vecPos);
void RwMatrixScale(RwMatrix* mat, CVector* vecScale);

int GetFreeTextDrawTextureSlot();
void DestroyTextDrawTexture(int index);

uintptr_t GetModelInfoByID(int iModelID);

void DestroyAtomicOrClump(uintptr_t rwObject);
float GetModelColSphereRadius(int iModel);
const char* GetAnimByIdx(int idx);
int GetAnimIdxByName(const char* szName);