//
// Created on 02.05.2023.
//

#include "Sprite.h"
#include "../../util/patch.h"
#include "game/Camera.h"
#include "game/Draw.h"

bool
CSprite::CalcScreenCoors(const RwV3d &posn, RwV3d *out, float *w, float *h, bool checkMaxVisible,
                         bool checkMinVisible)
                        {
    return CHook::CallFunction<bool>(g_libGTASA + (VER_x32 ? 0x005C57E8 + 1 : 0x6E9DF8), &posn, out, w, h, checkMaxVisible, checkMinVisible);
}

void CSprite::RenderOneXLUSprite(CVector pos, float SizeX, float SizeY, uint8 r, uint8 g, uint8 b, int16 intensity, float rhw, uint8 a, uint8 udir, uint8 vdir) {
    CHook::CallFunction<void>(g_libGTASA + (VER_x32 ? 0x005C5FC0 + 1 : 0x6EA5AC), pos, SizeX, SizeY, r, g, b, intensity, rhw, a, udir, vdir, 0.0f, 0.0f);
}

void CSprite::InitSpriteBuffer() {
    CHook::CallFunction<void>(g_libGTASA + (VER_x32 ? 0x005C5A9C + 1 : 0x6EA094));
}

void CSprite::RenderBufferedOneXLUSprite_Rotate_Dimension(float x, float y, float z, float SizeX, float SizeY, uint8 r, uint8 g, uint8 b, int16 intensity, float rz, float rotation, uint8 a) {
    CHook::CallFunction<void>(g_libGTASA + (VER_x32 ? 0x005C6B6C + 1 : 0x6EB0BC), x, y, z, SizeX, SizeY, r, g, b, intensity, rz, rotation, a);
}

void CSprite::RenderBufferedOneXLUSprite(float ScreenX, float ScreenY, float ScreenZ, float SizeX, float SizeY, uint8 r, uint8 g, uint8 b, int16 intensity, float recipNearZ, uint8 a11) {
    CHook::CallFunction<void>(g_libGTASA + (VER_x32 ? 0x005C5B10 + 1 : 0x6EA118), ScreenX, ScreenY, ScreenZ, SizeX, SizeY, r, g, b, intensity, recipNearZ, a11);
}

void CSprite::FlushSpriteBuffer() {
    CHook::CallFunction<void>(g_libGTASA + (VER_x32 ? 0x005C59F4 + 1 : 0x6E9FE8));
}

void CSprite::RenderBufferedOneXLUSprite2D(float posX, float posY, float sizeX, float sizeY, const CRGBA *color, int16 intensity, uint8 alpha) {
    CHook::CallFunction<void>(g_libGTASA + (VER_x32 ? 0x005C7D08 + 1 : 0x6EC108), posX, posY, sizeX, sizeY, color, intensity, alpha);
}
