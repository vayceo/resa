#pragma once

#include <stdint.h>
#include "game/common.h"

#define DEBUG_INFO_ENABLED

class CDebugInfo
{
	static uint32_t m_uiDrawDebug;
	static uint32_t m_uiDrawFPS;

public:
	static void ToggleDebugDraw();

	static void SetDrawFPS(uint32_t bDraw);

	static void Draw();
	static void ApplyDebugPatches();
};

