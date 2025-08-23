//
// Created by traw-GG on 05.08.2025.
//

#pragma once

#include "../../common.h"
#include "../../RW/rwcore.h"
#include "../../Core/Vector2D.h"
#include "Radar.h"

enum OSPointerState : int32
{
    OSPS_ButtonReleased = 0x0,
    OSPS_ButtonUp = 0x1,
    OSPS_ButtonPressed = 0x2,
    OSPS_ButtonDown = 0x3,
    OSPS_ButtonInvalid = 0xFF
};

struct MenuScreen
{
    int (**_vptr$MenuScreen)(void);
    RwTexture *arrowTex;
    float opacity;
    bool hasBack;
};

template <typename T> struct OSArray
{
    unsigned int numAlloced;
    unsigned int numEntries;
    T *dataPtr;
};

struct MobileMenu
{
    CVector2D bgUVSize;
    CVector2D bgTargetCoords;
    CVector2D bgCurCoords;
    CVector2D bgStartCoords;
    OSArray<MenuScreen*> screenStack;
    MenuScreen* pendingScreen;
    RwTexture* bgTex;
    RwTexture* sliderEmpty;
    RwTexture* sliderFull;
    RwTexture* sliderNub;
    RwTexture* controlsBack;
    RwTexture* controlsBack2;
    tBlipHandle waypoint_blip;
    bool8 m_WantsToRestartGame;
    bool WantsToLoad;
    int SelectedSlot;
    bool CurrentGameNotResumable;
    bool InitializedForSignOut;
    float NEW_MAP_SCALE;
    float MAP_OFFSET_X;
    float MAP_OFFSET_Y;
    float MAP_AREA_X;
    float MAP_AREA_Y;
    bool DisplayingMap;
    bool isMapMode;
    bool pointerMode;
    bool isMouse;
    CVector2D pointerCoords[4];
    OSPointerState pointerState[4];
    unsigned int pointerPress[4];
};
VALIDATE_SIZE(MobileMenu, (VER_x32 ? 0xB0 : 0xD0));

class CMobileMenu {
public:
    static void InjectHooks();

    static MobileMenu* GetMobileMenu();
};

