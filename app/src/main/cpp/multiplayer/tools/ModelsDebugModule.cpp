//
// Created on 20.11.2023.
//

#include "ModelsDebugModule.h"
#include "../game/common.h"
#include "../gui/gui.h"
#include "game/Renderer.h"
#include "game/Render/Sprite.h"
#include "main.h"
#include "game/Streaming.h"
#include "game/Entity/Entity.h"

void CModelsDebugModule::RenderMenuEntry() {
    notsa::ui::DoNestedMenuIL({ "Visualization", "Models" }, [&] {
        ImGui::Checkbox("Enabled", &m_bEnable);
    });
}

void TextWithColors(ImVec2 pos, ImColor col, const char* szStr, const char* szStrWithoutColors);

size_t GetTexSize(RpGeometry* geom) {
    size_t totalSize{};

    int numMats = geom->matList.numMaterials;
    if (numMats > 16)
        numMats = 16;

    for (int i = 0; i < numMats; i++) {
        RpMaterial *pMaterial = geom->matList.materials[i];
        if(!pMaterial)
            continue;

        auto tex = pMaterial->texture;
        if(!tex)
            continue;

        //totalSize += CHook::CallFunction<RwUInt32>(g_libGTASA + 0x1D3814 + 1, tex->raster->width, tex->raster->height, 0x12);
    }
    return totalSize;
}

void CModelsDebugModule::DrawModelInfo(CEntity* entity) {
    const auto& mi = entity->GetModelInfo();

    CVector screenPos;

    if (!CSprite::CalcScreenCoors(entity->GetPosition(), &screenPos, nullptr, nullptr, false, true))
        return;

    char str[512]{};
    sprintf(str, "%s [id: %d] nSpecialType: %d\nDFF size: %d kb Tex size: %d kb",
            mi->m_modelName, entity->m_nModelIndex, mi->nSpecialType,
            (CStreaming::GetInfo(entity->m_nModelIndex).GetCdSize() * STREAMING_SECTOR_SIZE) / 1024,
            GetTexSize(entity->m_pRwAtomic->geometry)
    );

    TextWithColors(ImVec2(screenPos.x, screenPos.y), __builtin_bswap32(0x4e86edFF), str, str);
}

void CModelsDebugModule::RenderWindow() {
    if (!m_bEnable)
        return;

    for (auto& entity : std::span{ CRenderer::ms_aVisibleEntityPtrs, (size_t)CRenderer::ms_nNoOfVisibleEntities }) {
        if (!entity || !entity->m_matrix)
            continue;

        const auto& mi = entity->GetModelInfo();
        if (!mi)
            continue;

        DrawModelInfo(entity);
    }
}
