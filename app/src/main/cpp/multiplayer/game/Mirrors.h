//
// Created by traw-GG on 14.07.2025.
//
#pragma once

#include "game.h"
#include "Matrix.h"
#include "Vector.h"

enum eMirrorType : uint8 {
    MIRROR_TYPE_NONE = 0,
    MIRROR_TYPE_WALL = 1,
    MIRROR_TYPE_FLOOR = 2,
    MIRROR_TYPE_SPHERE_MAP = 3
};

class CMirrors {
public:
    static inline RwRaster*     reflBuffer[2];
    static inline RwRaster*     pBuffer;
    static inline RwRaster*     pZBuffer;
    static inline eMirrorType   TypeOfMirror;
    static inline bool          bRenderingReflection;

public:
    static void InjectHooks();

    static void CreateBuffer();
    static void RenderReflections();
    static void BeforeMainRender();
};
