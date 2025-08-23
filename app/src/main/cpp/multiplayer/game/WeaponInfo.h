//
// Created on 24.04.2024.
//

#pragma once

#include "common.h"
#include "Enums/eWeaponType.h"
#include "Enums/eWeaponFire.h"
#include "Enums/AnimationEnums.h"
#include "Enums/eWeaponSkill.h"
#include "Enums/eStats.h"

struct CPed;

constexpr static auto FIRST_WEAPON_WITH_SKILLS = WEAPON_PISTOL;
constexpr static auto LAST_WEAPON_WITH_SKILLS  = WEAPON_TEC9;

constexpr static auto NUM_WEAPONS_WITH_SKILL = (LAST_WEAPON_WITH_SKILLS - FIRST_WEAPON_WITH_SKILLS) + 1;
constexpr static auto NUM_WEAPON_INFOS = NUM_WEAPONS + NUM_WEAPONS_WITH_SKILL * (NUM_WEAPON_SKILLS - 1);

#define SCANF_S_STR(s) s, std::size(s)

struct CGunAimingOffset {
    float AimX;
    float AimZ;
    float DuckX;
    float DuckZ;
    int16 RLoadA;
    int16 RLoadB;
    int16 CrouchRLoadA;
    int16 CrouchRLoadB;
};
static inline CGunAimingOffset g_GunAimingOffsets[20];

class CWeaponInfo {
public:
    eWeaponFire m_nWeaponFire;
    float       m_fTargetRange; // max targeting range
    float       m_fWeaponRange; // absolute gun range / default melee attack range
    int32       m_nModelId1;    // modelInfo id
    int32       m_nModelId2;    // second modelInfo id
    int32       m_nSlot;
    union {
        struct {
            uint32 bCanAim : 1;
            uint32 bAimWithArm : 1;
            uint32 b1stPerson : 1;
            uint32 bOnlyFreeAim : 1;
            uint32 bMoveAim : 1;  // can move when aiming
            uint32 bMoveFire : 1; // can move when firing
            uint32  : 1;
            uint32  : 1;

            uint32 bThrow : 1;
            uint32 bHeavy : 1; // can't run fast with this weapon
            uint32 bContinuosFire : 1;
            uint32 bTwinPistol : 1;
            uint32 bReload : 1;       // this weapon can be reloaded
            uint32 bCrouchFire : 1;   // can reload when crouching
            uint32 bReload2Start : 1; // reload directly after firing
            uint32 bLongReload : 1;

            uint32 bSlowdown : 1;
            uint32 bRandSpeed : 1;
            uint32 bExpands : 1;
        } flags;
        uint32 m_nFlags;
    };
    AssocGroupId m_eAnimGroup;      // instead of storing pointers directly to anims, use anim association groups
    uint16       m_nAmmoClip;       // ammo in one clip // todo: should be uint32, see
    uint16       m_nDamage;         // damage inflicted per hit
    CVector      m_vecFireOffset;   // offset from weapon origin to projectile starting point
    uint32       m_nSkillLevel;     // what's the skill level of this weapon type - We can't make the field eWeaponSkill because the game uses uint32 for it...
    uint32       m_nReqStatLevel;   // what stat level is required for this skill level (Yes, this is an int, not a float!)
    float        m_fAccuracy;       // modify accuracy of weapon
    float        m_fMoveSpeed;      // how fast can move with weapon

    // Could probably pack this into a struct like AnimLoopInfo, and use an array.
    float        m_fAnimLoopStart;  // start of animation loop
    float        m_fAnimLoopEnd;    // end of animation loop
    float        m_fAnimLoopFire;   // time in animation when weapon should be fired

    float        m_fAnimLoop2Start; // start of animation2 loop
    float        m_fAnimLoop2End;   // end of animation2 loop
    float        m_fAnimLoop2Fire;  // time in animation2 when weapon should be fired

    float        m_fBreakoutTime;   // time after which player can break out of attack and run off
    float        m_fSpeed;          // speed of projectile
    float        m_fRadius;         // radius affected
    float        m_fLifespan;       // time taken for shot to dissipate
    float        m_fSpread;         // angle inside which shots are created
    uint16       m_nAimOffsetIndex; // index into array of aiming offsets
    uint8        m_nBaseCombo;      // base combo for this melee weapon
    uint8        m_nNumCombos;      // how many further combos are available

public:
    static void InjectHooks();

    static void LoadWeaponData();
    static eWeaponType FindWeaponType(const char* type);
    static eWeaponFire FindWeaponFireType(const char* name);

    static CWeaponInfo *GetWeaponInfo(eWeaponType weaponType, eWeaponSkill skill = eWeaponSkill::STD);
    static uint32 GetWeaponInfoIndex(eWeaponType weaponType, eWeaponSkill skill);

    static bool TypeHasSkillStats(eWeaponType type);
    static bool TypeIsWeapon(eWeaponType type);
    static eStats GetSkillStatIndex(eWeaponType weaponType);

    auto GetCrouchReloadAnimationID() const -> AnimationId;
    auto GetWeaponReloadTime() const -> uint32;

    static auto GetWeaponInfo(CPed* ped);
    auto GetAnimLoopStart(bool isSet2 = false) const { return isSet2 ? m_fAnimLoop2Start : m_fAnimLoopStart; }
    auto GetAnimLoopEnd(bool isSet2 = false) const { return isSet2 ? m_fAnimLoop2End : m_fAnimLoopEnd; }
    const auto& GetAimingOffset() const { return g_GunAimingOffsets[m_nAimOffsetIndex]; }
};
VALIDATE_SIZE(CWeaponInfo, 0x70);

static inline CWeaponInfo aWeaponInfo[NUM_WEAPON_INFOS];
