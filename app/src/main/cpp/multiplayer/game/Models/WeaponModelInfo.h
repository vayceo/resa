//
// Created by traw-GG on 12.07.2025.
//
#pragma once

#include "ClumpModelInfo.h"
#include "Enums/eWeaponType.h"

struct CWeaponModelInfo : public CClumpModelInfo {
public:
    eWeaponType m_weaponInfo;

public:
    ModelInfoType GetModelType() override;
};

VALIDATE_SIZE(CWeaponModelInfo, (VER_x32 ? 0x40 : 0x58));
