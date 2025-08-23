#include "../main.h"
#include "game.h"
#include "Camera.h"
#include "Draw.h"


CAMERA_AIM* GameGetInternalAim()
{
    CCamera& origCam = *reinterpret_cast<CCamera*>(g_libGTASA + (VER_x32 ? 0x00951FA8 : 0xBBA8D0));
    return reinterpret_cast<CAMERA_AIM *>(&origCam.m_aCams[0].Front);
}

CAMERA_AIM caLocalPlayerAim;
CAMERA_AIM caRemotePlayerAim[MAX_PLAYERS];
uint16_t * pbyteCameraMode = nullptr;
uint16_t* wCameraMode2 = nullptr;

CAMERA_AIM * pcaInternalAim = nullptr;
float * pfCameraExtZoom = nullptr;
float* pfAspectRatio = nullptr;

float fCameraExtZoom[MAX_PLAYERS];
float fLocalCameraExtZoom;

float fCameraAspectRatio[MAX_PLAYERS];
float fLocalAspectRatio;

uint16_t byteCameraMode[MAX_PLAYERS];

//----------------------------------------------------------

void GameStoreLocalPlayerCameraExtZoom()
{
	fLocalCameraExtZoom = *pfCameraExtZoom;
	fLocalAspectRatio = *pfAspectRatio;
}

//----------------------------------------------------------

void GameSetLocalPlayerCameraExtZoom()
{
	*pfCameraExtZoom = fLocalCameraExtZoom;
	*pfAspectRatio = fLocalAspectRatio;
}

//----------------------------------------------------------

void GameSetPlayerCameraExtZoom(int bytePlayerID, float fZoom)
{
	fCameraExtZoom[bytePlayerID] = fZoom;
}

void GameSetPlayerCameraExtZoom(int bytePlayerID, float fZoom, float fAspectRatio)
{
	fCameraExtZoom[bytePlayerID] = fZoom;
	fCameraAspectRatio[bytePlayerID] = fAspectRatio;
}

//----------------------------------------------------------

float GameGetPlayerCameraExtZoom(int bytePlayerID)
{
	return fCameraExtZoom[bytePlayerID];
}

//----------------------------------------------------------

float GameGetLocalPlayerCameraExtZoom()
{
	float value = ((*pfCameraExtZoom) - 35.0f) / 35.0f;	// normalize for 35.0 to 70.0
	return value;
}

//----------------------------------------------------------

void GameSetRemotePlayerCameraExtZoom(int bytePlayerID)
{
	float fZoom = fCameraExtZoom[bytePlayerID];
	float fValue = fZoom * 35.0f + 35.0f; // unnormalize for 35.0 to 70.0
	*pfCameraExtZoom = fValue;
}

//----------------------------------------------------------

void GameSetPlayerCameraMode(uint16_t byteMode, int bytePlayerID)
{
	byteCameraMode[bytePlayerID] = byteMode;
}

//----------------------------------------------------------

uint16_t GameGetPlayerCameraMode(int bytePlayerID)
{
	return byteCameraMode[bytePlayerID];
}

//----------------------------------------------------------

uint16_t GameGetLocalPlayerCameraMode()
{
	if (!pbyteCameraMode) return 0;
	return *pbyteCameraMode;
}

void GameSetLocalPlayerCameraMode(uint16_t mode)
{
	if (!pbyteCameraMode) return;
	*pbyteCameraMode = mode;
}

//----------------------------------------------------------

void GameAimSyncInit()
{
    DLOG("GameAimSyncInit");

    CCamera& TheCamera = *reinterpret_cast<CCamera*>(g_libGTASA + (VER_x32 ? 0x00951FA8 : 0xBBA8D0));
    
	pbyteCameraMode = (uint16_t *) &TheCamera.m_aCams[0].m_nMode;
	pcaInternalAim = GameGetInternalAim();
	pfCameraExtZoom = &TheCamera.m_aCams[0].FOV;
	pfAspectRatio = (float*)(g_libGTASA + (VER_x32 ? 0x00A26A90 : 0xCC7F00));
	wCameraMode2 = &TheCamera.PlayerWeaponMode.m_nMode;

	memset(&caLocalPlayerAim, 0, sizeof(CAMERA_AIM));
	memset(caRemotePlayerAim, 0, sizeof(CAMERA_AIM) * MAX_PLAYERS);
	memset(byteCameraMode, 4, MAX_PLAYERS * sizeof(uint16_t));

	for (int i = 0; i < MAX_PLAYERS; i++)
		fCameraExtZoom[i] = 1.0f;
}

//----------------------------------------------------------

void GameStoreLocalPlayerAim()
{
	if (!pcaInternalAim) return;
	memcpy(&caLocalPlayerAim, pcaInternalAim, sizeof(CAMERA_AIM));
}

//----------------------------------------------------------

void GameSetLocalPlayerAim()
{
	//if (!pcaInternalAim) return;
	memcpy(pcaInternalAim, &caLocalPlayerAim, sizeof(CAMERA_AIM));
	//memcpy(pInternalCamera,&SavedCam,sizeof(RwMatrix));
}

//----------------------------------------------------------

//----------------------------------------------------------

void GameStoreRemotePlayerAim(int iPlayer, CAMERA_AIM * caAim)
{
	memcpy(&caRemotePlayerAim[iPlayer], caAim, sizeof(CAMERA_AIM));
}

//----------------------------------------------------------

void GameSetRemotePlayerAim(int iPlayer)
{
	if (!pcaInternalAim) return;
	memcpy(pcaInternalAim, &caRemotePlayerAim[iPlayer], sizeof(CAMERA_AIM));
}

//----------------------------------------------------------

CAMERA_AIM * GameGetRemotePlayerAim(int iPlayer)
{
	return &caRemotePlayerAim[iPlayer];
}

float GameGetAspectRatio()
{
	return *pfAspectRatio;
}