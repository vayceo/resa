//
// Created on 20.04.2023.
//

#include "RW/RenderWare.h"
#include "util/patch.h"

RwObject* GetFirstObjectCallback(RwObject* object, void* data) {
    *(RwObject**)(data) = object;
    return nullptr;
}

RwObject* GetFirstObject(RwFrame* frame) {
    RwObject* obj{};
    RwFrameForAllObjects(frame, GetFirstObjectCallback, &obj);
    return obj;
}

RpAtomic* GetFirstAtomic(RpClump* clump) {
    RpAtomic* atomic{};
    RpClumpForAllAtomics(clump, GetFirstAtomicCallback, &atomic);
    return atomic;
}

RpAtomic* GetFirstAtomicCallback(RpAtomic* atomic, void* data) {
    *(RpAtomic**)(data) = atomic;
    return nullptr;
}

RpHAnimHierarchy* GetAnimHierarchyFromSkinClump(RpClump* clump) {
    RpHAnimHierarchy* anim{};
    RpClumpForAllAtomics(clump, SkinAtomicGetHAnimHierarchCB, &anim);
    return anim;
}

// name not from Android
RpAtomic* SkinAtomicGetHAnimHierarchCB(RpAtomic* atomic, void* data) {
    *(RpHAnimHierarchy**)(data) = RpSkinAtomicGetHAnimHierarchy(atomic);
    return nullptr;
}

RpAtomic* AtomicRemoveAnimFromSkinCB(RpAtomic* atomic, void* data) {
    if (RpSkinGeometryGetSkin(RpAtomicGetGeometry(atomic))) {
        if (RpHAnimHierarchy* hier = RpSkinAtomicGetHAnimHierarchy(atomic)) {
            RtAnimAnimation*& currAnim = hier->currentAnim->pCurrentAnim;
            if (currAnim) {
                RtAnimAnimationDestroy(currAnim);
            }
            currAnim = nullptr;
        }
    }
    return atomic;
}

RpClump* RpClumpGetBoundingSphere(RpClump* clump, RwSphere* sphere, bool bUseLTM) {
    return CHook::CallFunction<RpClump*>(g_libGTASA + (VER_x32 ? 0x5D0E3C + 1 : 0x6F51F8), clump, sphere, bUseLTM);
}

RpHAnimHierarchy* GetAnimHierarchyFromFrame(RwFrame* frame) {
    return CHook::CallFunction<RpHAnimHierarchy*>("_Z25GetAnimHierarchyFromFrameP7RwFrame", frame);
}

RpHAnimHierarchy* GetAnimHierarchyFromClump(RpClump* clump) {
    return GetAnimHierarchyFromFrame(RpClumpGetFrame(clump));
}