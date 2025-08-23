//
// Created on 16.09.2023.
//

#pragma once

#include "Object.h"

class CCutsceneObject : public CObject {
    union {
        RwFrame* m_pAttachToFrame;
        uint32   m_nAttachBone; // this one if m_pAttachmentObject != 0
    };
    CObject* m_pAttachmentObject;
    CVector     m_vWorldPosition;
    CVector     m_vForce;
};

VALIDATE_SIZE(CCutsceneObject, (VER_x32 ? 0x1A4 : 0x220));