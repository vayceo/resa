//
// Created on 07.04.2023.
//

#pragma once

#include "WeaponInfo.h"
#include "Enums/eWeaponType.h"
#include "Enums/eWeaponSkill.h"

struct CColPoint;
struct CPed;
struct CEntity;

enum class eWeaponSlot : uint32 {
    UNARMED,
    MELEE,
    HANDGUN,
    SHOTGUN,
    SMG,        // Used for drive-by's
    RIFLE,
    SNIPER,
    HEAVY,
    THROWN,
    SPECIAL,
    GIFT,
    PARACHUTE,
    DETONATOR,
};
constexpr auto NUM_WEAPON_SLOTS = (size_t)eWeaponSlot::DETONATOR + 1u;

enum eWeaponState : uint32 {
    WEAPONSTATE_READY = 0,
    WEAPONSTATE_FIRING,
    WEAPONSTATE_RELOADING,
    WEAPONSTATE_OUT_OF_AMMO,
    WEAPONSTATE_MELEE_MADECONTACT,
};

struct CWeapon
{
    eWeaponType m_nType;
    eWeaponState m_nState;
    union {
        uint32_t dwAmmoInClip;
        uint32_t m_nAmmoInClip;
    };
    uint32_t m_nTotalAmmo;
    uint32_t m_nTimer;
    bool m_bFirstPersonWeaponModeSelected;
    bool m_bDontPlaceInHand;
    uint8_t pad[2];
    uintptr_t *m_pWeaponFxSys;

public:
    static void InjectHooks();

    CWeapon() = default;
    CWeapon(eWeaponType weaponType, uint32 ammo);
    CWeapon(const CWeapon&) = delete;

    bool IsTypeMelee();
    bool IsType2Handed();
    bool IsTypeProjectile();

    bool HasWeaponAmmoToBeUsed();

    bool IsFiredWeapon() const;

    void StopWeaponEffect();

    void CheckForShootingVehicleOccupant(CEntity** pCarEntity, CColPoint* colPoint, eWeaponType weaponType, const CVector& origin, const CVector& target);
    float TargetWeaponRangeMultiplier(CEntity* victim, CEntity* weaponOwner);
    bool FireM16_1stPerson(CPed* owner);
    bool FireProjectile(CEntity* firedBy, const CVector& origin, CEntity* targetEntity, const CVector* targetPos, float force = 0.f);
    bool FireInstantHit(CEntity* firingEntity, CVector* origin, CVector* muzzlePosn, CEntity* targetEntity, CVector* target, CVector* originForDriveBy, bool arg6, bool muzzle);
    void AddGunshell(CEntity *pEntity, const CVector *posGunshell, const CVector2D *dirGunshell, float fGunshellSize);
    void DoBulletImpact(CEntity* firedBy, CEntity* victim, const CVector& startPoint, const CVector& endPoint, const CColPoint& hitCP, int32 incrementalHit);
    bool FireSniper(CPed* shooter, CEntity* victim, CVector* target);
    bool FireAreaEffect(CEntity* firingEntity, const CVector& origin, CEntity* targetEntity, CVector* target);
    bool Fire(CEntity* firedBy, CVector* startPosn, CVector* barrelPosn, CEntity* targetEnt, CVector* targetPosn, CVector* altPosn);
    void Update(CPed* owner);

    CWeaponInfo& GetWeaponInfo(CPed* owner = nullptr) const;
    CWeaponInfo& GetWeaponInfo(eWeaponSkill skill) const;

    static void DoTankDoomAiming(CEntity* vehicle, CEntity* owner, CVector* startPoint, CVector* endPoint);
    void Reload(CPed *owner = nullptr);

    auto GetType()          const noexcept { return m_nType; }
    auto GetState()         const noexcept { return m_nState; }
    auto GetAmmoInClip()    const noexcept { return dwAmmoInClip; }
    auto GetTotalAmmo()     const noexcept { return m_nTotalAmmo; }
};
static_assert(sizeof(CWeapon) == (VER_x32 ? 0x1C : 0x20), "Invalid size CWeapon");