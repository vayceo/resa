//
// Created on 20.09.2023.
//

#include "RenderBuffer.h"
#include "../util/patch.h"

int32 s_IndicesToBeStored;
int32 s_VerticesToBeStored;
const auto IdxBufferSize = (int32)std::size(aTempBufferIndices);
const auto VtxBufferSize = (int32)std::size(TempBufferVertices.m_3d);


void RenderBuffer::RenderStuffInBuffer() {
    Render(rwPRIMTYPETRILIST, nullptr, rwIM3D_VERTEXUV);
}

void RenderBuffer::StartStoring(
        int32 nIndicesNeeded,
        int32 nVerticesNeeded,
        RwImVertexIndex*& outPtrFirstIndex,
        RwIm3DVertex*& outPtrFirstVertex
) {
    assert(nIndicesNeeded > 0 && nIndicesNeeded <= IdxBufferSize);
    assert(nVerticesNeeded > 0 && nVerticesNeeded <= VtxBufferSize);

    if (uiTempBufferIndicesStored + nIndicesNeeded >= IdxBufferSize) {
        RenderBuffer::RenderStuffInBuffer();
    }

    if (uiTempBufferVerticesStored + nVerticesNeeded >= VtxBufferSize) {
        RenderBuffer::RenderStuffInBuffer();
    }

    outPtrFirstIndex = &aTempBufferIndices[uiTempBufferIndicesStored];
    outPtrFirstVertex = &TempBufferVertices.m_3d[uiTempBufferVerticesStored];

    s_IndicesToBeStored = nIndicesNeeded;
    s_VerticesToBeStored = nVerticesNeeded;
}

void RenderBuffer::StopStoring() {
    const auto totalNumIdx = uiTempBufferIndicesStored + s_IndicesToBeStored;
    if (uiTempBufferIndicesStored >= totalNumIdx) { // Doesn't make a lot of sense, as it will only be true if `s_IndicesToBeStored` is negative, which isn't allowed.
        uiTempBufferVerticesStored += s_VerticesToBeStored;
        uiTempBufferIndicesStored  += s_IndicesToBeStored;
    } else {
        for (auto n = uiTempBufferIndicesStored; n <= totalNumIdx; n++) {
            aTempBufferIndices[n] += uiTempBufferVerticesStored;
        }
        uiTempBufferVerticesStored += s_VerticesToBeStored;
        uiTempBufferIndicesStored   = totalNumIdx;
    }
}

void RenderBuffer::ClearRenderBuffer() {
    uiTempBufferIndicesStored  = 0;
    uiTempBufferVerticesStored = 0;
}

// NOTSA
void RenderBuffer::Render(RwPrimitiveType primType, RwMatrix* ltm, RwUInt32 /*RwIm3DTransformFlags*/ flags, bool isIndexed) {
    if (uiTempBufferVerticesStored) {
        if (RwIm3DTransform(TempBufferVertices.m_3d, uiTempBufferVerticesStored, ltm, flags)) {
            if (isIndexed) {
                assert(aTempBufferIndices);
                RwIm3DRenderIndexedPrimitive(primType, aTempBufferIndices, uiTempBufferIndicesStored);
            } else {
                RwIm3DRenderPrimitive(primType);
            }
            RwIm3DEnd();
        }
    }
    ClearRenderBuffer();
}

bool RenderBuffer::CanFitVertices(int32 nVtxNeeded) {
    return uiTempBufferVerticesStored + nVtxNeeded <= VtxBufferSize;
}

RwIm3DVertex *RenderBuffer::PushVertex(CVector pos, CRGBA color) {
    const auto vtx = &TempBufferVertices.m_3d[uiTempBufferVerticesStored++];

    RwIm3DVertexSetPos(vtx, pos.x, pos.y, pos.z);
    RwIm3DVertexSetRGBA(vtx, color.r, color.g, color.b, color.a);

    return vtx;
}

// notsa
RwIm3DVertex* RenderBuffer::PushVertex(CVector pos, CVector2D uv, CRGBA color) {
    const auto vtx = PushVertex(pos, color);

            RwIm3DVertexSetU(vtx, uv.x);
            RwIm3DVertexSetV(vtx, uv.y);

    return vtx;
}

void RenderBuffer::InjectHooks() {
    CHook::Write(g_libGTASA + (VER_x32 ? 0x679AD0 : 0x8515C0), &TempBufferVertices);
    CHook::Write(g_libGTASA + (VER_x32 ? 0x675ED4 : 0x849E30), &aTempBufferIndices);

    CHook::Write(g_libGTASA + (VER_x32 ? 0x679D04 : 0x851A20), &uiTempBufferVerticesStored);
    CHook::Write(g_libGTASA + (VER_x32 ? 0x676504 : 0x84AA78), &uiTempBufferIndicesStored);
}

// notsa
void PushIndex(int32 idx, bool useCurrentVtxAsBase) {
    idx = useCurrentVtxAsBase
          ? uiTempBufferVerticesStored + idx
          : idx;
    assert(idx >= 0);
    aTempBufferIndices[uiTempBufferIndicesStored++] = idx;
}

void RenderBuffer::PushIndices(std::initializer_list<int32> idxs, bool useCurrentVtxAsBase) {
    for (auto idx : idxs) {
        PushIndex(idx, useCurrentVtxAsBase);
    }
}
