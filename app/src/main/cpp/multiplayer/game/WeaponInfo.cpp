//
// Created on 24.04.2024.
//

#include "WeaponInfo.h"
#include "util/patch.h"
#include "Models/ModelInfo.h"
#include "game/Entity/Ped/Ped.h"
#include "CFileMgr.h"
#include "FileLoader.h"
#include "Animation/AnimManager.h"
#include "Enums/eMeleeCombo.h"

eWeaponFire CWeaponInfo::FindWeaponFireType(const char* name) {
    static constexpr struct { std::string_view name; eWeaponFire wf; } mapping[] {
            { "MELEE",       WEAPON_FIRE_MELEE       },
            { "INSTANT_HIT", WEAPON_FIRE_INSTANT_HIT },
            { "PROJECTILE",  WEAPON_FIRE_PROJECTILE  },
            { "AREA_EFFECT", WEAPON_FIRE_AREA_EFFECT },
            { "CAMERA",      WEAPON_FIRE_CAMERA      },
            { "USE",         WEAPON_FIRE_USE         },
    };
    const auto it = std::ranges::find(mapping, name, [](const auto& e) { return e.name; });
    if (it != std::end(mapping)) {
        return it->wf;
    }
    return WEAPON_FIRE_INSTANT_HIT;
}

auto GetBaseComboByName(const char* name) {
    static constexpr std::pair<std::string_view, eMeleeCombo> mapping[] {
            { "UNARMED",     MELEE_COMBO_UNARMED_1 },
            { "BBALLBAT",    MELEE_COMBO_BBALLBAT  },
            { "KNIFE",       MELEE_COMBO_KNIFE     },
            { "GOLFCLUB",    MELEE_COMBO_GOLFCLUB  },
            { "SWORD",       MELEE_COMBO_SWORD     },
            { "CHAINSAW",    MELEE_COMBO_CHAINSAW  },
            { "DILDO",       MELEE_COMBO_DILDO     },
            { "FLOWERS",     MELEE_COMBO_FLOWERS   },
    };
    const auto it = std::ranges::find(mapping, name, [](const auto& e) { return e.first; });
    if (it != std::end(mapping))
        return it->second;

    return eMeleeCombo::MELEE_COMBO_UNARMED_1;
}

void CWeaponInfo::LoadWeaponData() {
    Log("Loading weapon.dat..");

    auto f = CFileMgr::OpenFile("DATA/WEAPON.DAT", "rb");
    for (auto line = CFileLoader::LoadLine(f); line; line = CFileLoader::LoadLine(f)) {
        std::string_view sv(line);
        if (sv.find("ENDWEAPONDATA") != std::string_view::npos) {
            break;
        }

        switch ((uint8)line[0]) { // Gotta cast it because of `case 163`
            case '$': { // Gun data
                char weaponName[32]{};
                char fireTypeName[32]{};
                float targetRange{}, weaponRange{};
                int32 modelId1{}, modelId2{};
                int32 slot{};
                char animGrpName[32]{};
                uint32 ammo{};
                uint32 dmg{};
                CVector offset{};
                uint32 skill{};
                uint32 reqStatLevelForSkill{};
                float accuracy{};
                float moveSpeed{};

                struct {
                    int32 end{}, start{}, fire{};
                } animLoopInfo[2]{};

                int32 breakoutTime{};
                uint32 flags{};
                float speed{}, radius{};
                float lifespan{}, spread{};

                std::istringstream iss(line);
                std::string tmp;

                // Пропускаем первый символ
                iss >> tmp;

                iss >> weaponName >> fireTypeName
                    >> targetRange >> weaponRange
                    >> modelId1 >> modelId2 >> slot
                    >> animGrpName
                    >> ammo >> dmg
                    >> offset.x >> offset.y >> offset.z
                    >> skill >> reqStatLevelForSkill
                    >> accuracy >> moveSpeed
                    >> animLoopInfo[0].start >> animLoopInfo[0].end >> animLoopInfo[0].fire
                    >> animLoopInfo[1].start >> animLoopInfo[1].end >> animLoopInfo[1].fire
                    >> breakoutTime
                    >> std::hex >> flags >> std::dec
                    >> speed
                    >> radius
                    >> lifespan
                    >> spread;

                const auto weaponType = FindWeaponType(weaponName);
                const auto skillLevel = TypeHasSkillStats(weaponType)
                                        ? (eWeaponSkill)skill
                                        : eWeaponSkill::STD;
                auto& wi = *GetWeaponInfo(weaponType, skillLevel);
                wi.m_nWeaponFire = FindWeaponFireType(fireTypeName);
                wi.m_fTargetRange = targetRange;
                wi.m_fWeaponRange = weaponRange;
                wi.m_nModelId1 = modelId1;
                wi.m_nModelId2 = modelId2;
                wi.m_nSlot = slot;
                wi.m_nAmmoClip = ammo;
                wi.m_nDamage = dmg;
                wi.m_vecFireOffset = offset;
                wi.m_nSkillLevel = (uint32)skillLevel;
                wi.m_nReqStatLevel = reqStatLevelForSkill;
                wi.m_fAccuracy = accuracy;
                wi.m_fMoveSpeed = moveSpeed;
                wi.m_fBreakoutTime = (float)breakoutTime / 30.f;
                wi.m_nFlags = flags;
                wi.m_fSpeed = speed;
                wi.m_fLifespan = lifespan;
                wi.m_fSpread = spread;
                wi.m_fRadius = radius;

                const auto SetAnimLoopInfos = [&](auto& start, auto& end, auto& fire, auto idx) {
                    assert(start <= end);

                    const auto info = animLoopInfo[idx];
                    start = (float)info.start / 30.f;
                    end = (float)info.end / 30.f;
                    fire = (float)info.fire / 30.f;

                    end = start + std::floor(0.1f + (end - start) * 50.f) / 50.f - 0.006f;
                };
                SetAnimLoopInfos(wi.m_fAnimLoopStart, wi.m_fAnimLoopEnd, wi.m_fAnimLoopFire, 0);
                SetAnimLoopInfos(wi.m_fAnimLoop2Start, wi.m_fAnimLoop2End, wi.m_fAnimLoop2Fire, 1);


                if (!std::string_view{ animGrpName }.starts_with("null")) {
                    wi.m_eAnimGroup = CAnimManager::GetAnimationGroupIdByName(animGrpName);
                }

                if (wi.m_eAnimGroup >= ANIM_GROUP_PYTHON && wi.m_eAnimGroup <= ANIM_GROUP_SPRAYCAN) {
                    wi.m_nAimOffsetIndex = wi.m_eAnimGroup - ANIM_GROUP_PYTHON;
                }

                if (skillLevel == eWeaponSkill::STD && weaponType != eWeaponType::WEAPON_DETONATOR) {
                    if (modelId1 > 0) {
                        CModelInfo::GetModelInfo(modelId1)->AsWeaponModelInfoPtr()->m_weaponInfo = weaponType;
                    }
                }
                break;
            }
            case '%': { // Gun aiming offsets
                char stealthAnimGrp[32]{};
                float aimX{}, aimZ{};
                float duckX{}, duckZ{};
                uint32 RLoadA{}, RLoadB{};
                uint32 crouchRLoadA{}, crouchRLoadB{};

                std::istringstream iss(line);
                std::string tmp;

                // Пропускаем первый символ
                iss >> tmp;

                iss >> stealthAnimGrp
                    >> aimX >> aimZ
                    >> duckX >> duckZ
                    >> RLoadA >> RLoadB
                    >> crouchRLoadA >> crouchRLoadB;

                g_GunAimingOffsets[CAnimManager::GetAnimationGroupIdByName(stealthAnimGrp) - ANIM_GROUP_PYTHON] = {
                        .AimX = aimX,
                        .AimZ = aimZ,

                        .DuckX = duckX,
                        .DuckZ = duckZ,

                        .RLoadA = (int16)RLoadA,
                        .RLoadB = (int16)RLoadB,

                        .CrouchRLoadA = (int16)crouchRLoadA,
                        .CrouchRLoadB = (int16)crouchRLoadB
                };

                break;
            }
            case 163: { // MELEE DATA
                char weaponName[32]{};
                char fireTypeName[32]{};
                float targetRange{}, weaponRange{};
                int32 modelId1{}, modelId2{};
                uint32 slot{};
                char baseComboName[32]{};
                uint32 numCombos{};
                uint32 flags{};
                char stealthAnimGrpName[32]{};

                std::istringstream iss(line);
                std::string tmp;

                // Пропускаем первый символ
                iss >> tmp;

                iss >> weaponName >> fireTypeName
                    >> targetRange >> weaponRange
                    >> modelId1 >> modelId2 >> slot
                    >> baseComboName
                    >> numCombos
                    >> std::hex >> flags >> std::dec
                    >> stealthAnimGrpName;

                auto wType = FindWeaponType(weaponName);
                auto& wi = aWeaponInfo[(uint32)wType];
                wi.m_nWeaponFire = FindWeaponFireType(fireTypeName);
                wi.m_fTargetRange = targetRange;
                wi.m_fWeaponRange = weaponRange;
                wi.m_nModelId1 = modelId1;
                wi.m_nModelId2 = modelId2;
                wi.m_nSlot = slot;
                wi.m_nBaseCombo = GetBaseComboByName(baseComboName);
                wi.m_nNumCombos = (uint8)numCombos;
                wi.m_nFlags = flags;

                if (!std::string_view{stealthAnimGrpName}.starts_with("null"))
                    wi.m_eAnimGroup = CAnimManager::GetAnimationGroupIdByName(stealthAnimGrpName);

                if (modelId1 > 0)
                    CModelInfo::GetModelInfo(modelId1)->AsWeaponModelInfoPtr()->m_weaponInfo = wType;

                break;
            }
        }
    }
    CFileMgr::CloseFile(f);
}

eWeaponType CWeaponInfo::FindWeaponType(const char* type) {
    constexpr const char* names[] {
            "UNARMED",
            "BRASSKNUCKLE",
            "GOLFCLUB",
            "NIGHTSTICK",
            "KNIFE",
            "BASEBALLBAT",
            "SHOVEL",
            "POOLCUE",
            "KATANA",
            "CHAINSAW",
            "DILDO1",
            "DILDO2",
            "VIBE1",
            "VIBE2",
            "FLOWERS",
            "CANE",
            "GRENADE",
            "TEARGAS",
            "MOLOTOV",
            "ROCKET",
            "ROCKET_HS",
            "FREEFALL_BOMB",
            "PISTOL",
            "PISTOL_SILENCED",
            "DESERT_EAGLE",
            "SHOTGUN",
            "SAWNOFF",
            "SPAS12",
            "MICRO_UZI",
            "MP5",
            "AK47",
            "M4",
            "TEC9",
            "COUNTRYRIFLE",
            "SNIPERRIFLE",
            "RLAUNCHER",
            "RLAUNCHER_HS",
            "FTHROWER",
            "MINIGUN",
            "SATCHEL_CHARGE",
            "DETONATOR",
            "SPRAYCAN",
            "EXTINGUISHER",
            "CAMERA",
            "NIGHTVISION",
            "INFRARED",
            "PARACHUTE",
            "", // Yeah, empty string. Unsure why.
            "ARMOUR",
    };
    for (auto i = 0u; i < std::size(names); i++) {
        if (!strcmp(names[i], type)) {
            return (eWeaponType)i;
        }
    }
    assert(0); // Shouldn't be reachable
    return eWeaponType::WEAPON_UNARMED;
}

CWeaponInfo* CWeaponInfo::GetWeaponInfo(eWeaponType weaponID, eWeaponSkill skill) {
    return &aWeaponInfo[GetWeaponInfoIndex(weaponID, skill)];
}

uint32 CWeaponInfo::GetWeaponInfoIndex(eWeaponType wt, eWeaponSkill skill) {
    assert(TypeIsWeapon(wt) || (skill == eWeaponSkill::STD && (wt >= WEAPON_RAMMEDBYCAR && wt <= WEAPON_FLARE))); // Damage events also have their weapon info entries

    const auto GetNonSTDSkillLevelIndex = [wt](uint32 i) {
        assert(TypeHasSkillStats(wt));
        return WEAPON_LAST_WEAPON           // Offset for std skill level
               + wt - FIRST_WEAPON_WITH_SKILLS // Offset for this weapon [relative to the other weapons with skill levels]
               + i * NUM_WEAPONS_WITH_SKILL;   // Offset for skill level
    };

    switch (skill) {
        case eWeaponSkill::POOR: return GetNonSTDSkillLevelIndex(0);
        case eWeaponSkill::STD:  return (uint32)wt;
        case eWeaponSkill::PRO:  return GetNonSTDSkillLevelIndex(1);
        case eWeaponSkill::COP:  return GetNonSTDSkillLevelIndex(2);
        default:                 DLOG("Invalid weapon skill");
    }
}

bool CWeaponInfo::TypeHasSkillStats(eWeaponType wt) {
    return wt >= FIRST_WEAPON_WITH_SKILLS && wt <= LAST_WEAPON_WITH_SKILLS;
}

bool CWeaponInfo::TypeIsWeapon(eWeaponType type) {
    return type < WEAPON_LAST_WEAPON;
}

auto CWeaponInfo::GetWeaponInfo(CPed* ped) {
    return GetWeaponInfo(ped->GetActiveWeapon().m_nType, ped->GetWeaponSkill());
}

AnimationId CWeaponInfo::GetCrouchReloadAnimationID() const {
    return flags.bCrouchFire && flags.bReload ? ANIM_ID_CROUCHRELOAD : ANIM_ID_WALK;
}

uint32 CWeaponInfo::GetWeaponReloadTime() const {
    if (flags.bReload)
        return flags.bTwinPistol ? 2000u : 1000u;

    if (flags.bLongReload)
        return 1000u;

    const auto& ao = g_GunAimingOffsets[m_nAimOffsetIndex];
    return std::max(400u, (uint32)std::max({ ao.RLoadA, ao.RLoadB, ao.CrouchRLoadA, ao.CrouchRLoadB }) + 100);
}

eStats CWeaponInfo::GetSkillStatIndex(eWeaponType wt) {
    switch (wt) {
        case WEAPON_PISTOL:          return STAT_PISTOL_SKILL;
        case WEAPON_PISTOL_SILENCED: return STAT_SILENCED_PISTOL_SKILL;
        case WEAPON_DESERT_EAGLE:    return STAT_DESERT_EAGLE_SKILL;
        case WEAPON_SHOTGUN:         return STAT_SHOTGUN_SKILL;
        case WEAPON_SAWNOFF_SHOTGUN: return STAT_SAWN_OFF_SHOTGUN_SKILL;
        case WEAPON_SPAS12_SHOTGUN:  return STAT_COMBAT_SHOTGUN_SKILL;
        case WEAPON_MICRO_UZI:       return STAT_MACHINE_PISTOL_SKILL;
        case WEAPON_MP5:             return STAT_SMG_SKILL;
        case WEAPON_AK47:            return STAT_AK_47_SKILL;
        case WEAPON_M4:              return STAT_M4_SKILL;
        case WEAPON_TEC9:            return STAT_MACHINE_PISTOL_SKILL;
    }
}

void CWeaponInfo::InjectHooks() {
    CHook::Write(g_libGTASA + (VER_x32 ? 0x00678ECC : 0x84FDC8), &aWeaponInfo);
    CHook::Write(g_libGTASA + (VER_x32 ? 0x006790A0 : 0x850170), &g_GunAimingOffsets);

    CHook::Redirect("_ZN11CWeaponInfo14LoadWeaponDataEv", &LoadWeaponData);
}