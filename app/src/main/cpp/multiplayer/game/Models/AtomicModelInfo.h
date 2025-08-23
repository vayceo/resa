//
// Created on 07.03.2023.
//

#pragma once

#include "BaseModelInfo.h"


struct CAtomicModelInfo : public CBaseModelInfo {
public:
    CAtomicModelInfo* AsAtomicModelInfoPtr() override;
    ModelInfoType GetModelType() override;
};


void SetAtomicModelInfoFlags(CAtomicModelInfo* modelInfo, uint32 dwFlags);

VALIDATE_SIZE(CAtomicModelInfo, (VER_x32 ? 0x38 : 0x48));