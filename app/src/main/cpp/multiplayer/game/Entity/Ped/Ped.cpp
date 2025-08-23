//
// Created on 05.05.2023.
//

#include "Ped.h"
#include "../../../util/patch.h"
#include "game/Plugins/RpAnimBlendPlugin/RpAnimBlend.h"
#include "game/Timer.h"
#include "game/Streaming.h"
#include "chatwindow.h"

CVector CPed::GetBonePosition(ePedBones bone, bool updateSkinBones) {
    if (!bCalledPreRender) {
        if (!updateSkinBones) {
            return m_matrix->TransformPoint(GetPedBoneStdPosition(bone));
        }
        UpdateRpHAnim();
        bCalledPreRender = true;
    }
    if (const auto* const m = GetBoneMatrix(bone)) {
        return *RwMatrixGetPos(m);
    }
    return GetPosition();
}

void CPed::GetBonePosition(CVector* outVec, ePedBones bone, bool updateSkinBones) {
    *outVec = GetBonePosition(bone, updateSkinBones);
}

RwMatrix* CPed::GetBoneMatrix(ePedBones bone) const {
    if (const auto h = GetAnimHierarchyFromClump(m_pRwClump)) {
        return RpHAnimHierarchyGetNodeMatrix(h, bone);
    }
    return nullptr;
}

void CPed::RemoveFromVehicleAndPutAt(const CVector& pos)
{
    if(!pVehicle) return;

    m_pIntelligence->m_TaskMgr.FlushImmediately();

    Teleport(pos, false);

    UpdateRW();
    UpdateRwFrame();//? ���� ��
}

/*!
* @brief Clear gun aiming flag
*/
void CPed::ClearAimFlag() {
    if (bIsAimingGun) {
        bIsAimingGun = false;
        bIsRestoringGun = true;
        m_pedIK.bUseArm = false;
        m_nLookTime = 0;
    }

//    if (m_pPlayerData) {
//        m_pPlayerData->m_fLookPitch = 0.f;
//    }
}

/*!
* @addr 0x5DED90
*/
bool CPed::CanUseTorsoWhenLooking() const {
    switch (m_nPedState) {
        case PEDSTATE_DRIVING:
        case PEDSTATE_DRAGGED_FROM_CAR:
            return false;
    }

    if (bIsDucking) {
        return false;
    }

    return true;
}

/*!
* @addr 0x5E1950
* @brief Clear ped look, and start restoring it
*/
void CPed::ClearLookFlag() {
    if (!bIsLooking) {
        return;
    }

    // Originally there's a do-while loop, but it will never iterate more than once, so I won't add it.
    // do { ...

    bIsLooking = false;
    bIsDrowning = false;
    bIsRestoringLook = true;

    if (CanUseTorsoWhenLooking()) {
        m_pedIK.bTorsoUsed = false;
    }

    m_nLookTime = CTimer::GetTimeInMS() + (IsPlayer() ? 4000 : 2000);

    // .. } while ((PEDSTATE_LOOK_HEADING || PEDSTATE_LOOK_ENTITY) && bIsLooking), but `bIsLooking` will never be true at this point.
}


void CPed::GiveWeapon(int iWeaponID, int iAmmo)
{
    int iModelID = 0;
    iModelID = CUtil::GameGetWeaponModelIDFromWeaponID(iWeaponID);

    if (iModelID == -1) return;

    if (!CStreaming::TryLoadModel(iModelID))
        return;

    CHook::CallFunction<void>(g_libGTASA + (VER_x32 ? 0x0049F588 + 1 : 0x59525C), this, iWeaponID, iAmmo); // CPed::GiveWeapon(thisptr, weapoid, ammo)
    CHook::CallFunction<void>(g_libGTASA + (VER_x32 ? 0x004A521C + 1 : 0x59B86C), this, iWeaponID);	// CPed::SetCurrentWeapon(thisptr, weapid)
}

eWeaponSlot CPed::GiveWeapon(eWeaponType weaponType, uint32 ammo, bool likeUnused) {
    return CHook::CallFunction<eWeaponSlot>("_ZN4CPed10GiveWeaponE11eWeaponTypejb", weaponType, ammo, likeUnused);
    /*const auto givenWepInfo = CWeaponInfo::GetWeaponInfo(weaponType);
    auto& wepInSlot = GetWeaponInSlot(givenWepInfo->m_nSlot);
    const auto wepSlot = (eWeaponSlot)givenWepInfo->m_nSlot;

    if (wepInSlot.m_nType != weaponType) { // Another weapon in the slot, remove it, and set this weapon

        // Remove previous weapon (and possibly add any ammo it had to `ammo`)
        if (wepInSlot.m_nType != WEAPON_UNARMED) {
            switch (wepSlot) {
                case eWeaponSlot::SHOTGUN:
                case eWeaponSlot::SMG:
                case eWeaponSlot::RIFLE: {
                    ammo += wepInSlot.m_nTotalAmmo;
                    break;
                }
            }

            RemoveWeaponModel(wepInSlot.GetWeaponInfo().m_nModelId1);

            if (givenWepInfo->m_nSlot == CWeaponInfo::GetWeaponInfo(WEAPON_INFRARED)->m_nSlot) {
                RemoveGogglesModel();
            }

            wepInSlot.Shutdown();
        }

        wepInSlot.Initialise(weaponType, ammo, this);

        // Now `wepInSlot` is the weapon we've given to the player

        if (givenWepInfo->m_nSlot == m_nActiveWeaponSlot && !bInVehicle) {
            AddWeaponModel(givenWepInfo->m_nModelId1);
        }
    } else { // Same weapon already in the slot, update its ammo count and `Reload()` it
        if (wepSlot == eWeaponSlot::GIFT) { // Gifts have no ammo :D
            return eWeaponSlot::GIFT;
        }

        wepInSlot.m_nTotalAmmo = std::min(99'999u, wepInSlot.m_nTotalAmmo + ammo);
        wepInSlot.Reload(this);

        // TODO: Inlined
        if (wepInSlot.m_nState == WEAPONSTATE_OUT_OF_AMMO) {
            if (wepInSlot.m_nTotalAmmo > 0) {
                wepInSlot.m_nState = WEAPONSTATE_READY;
            }
        }
    }

    if (wepInSlot.m_nState != WEAPONSTATE_OUT_OF_AMMO) {
        wepInSlot.m_nState = WEAPONSTATE_READY;
    }

    return wepSlot;*/
}

/*!
* @addr 0x5DEC00
*/
void CPed::SetMoveState(eMoveState moveState) {
    m_nMoveState = moveState;
}

/*!
* @addr  0x5E4500
* @brief Set ped's state. If he's now !IsAlive() blip is deleted (if `bClearRadarBlipOnDeath` is set) and `ReleaseCoverPoint()` is called.
*/
void CPed::SetPedState(ePedState pedState) {
    m_nPedState = pedState;
//    if (!IsAlive()) {
//        ReleaseCoverPoint();
//        if (bClearRadarBlipOnDeath) {
//            CRadar::ClearBlipForEntity(BLIP_CHAR, GetPedPool()->GetRef(this));
//        }
//    }
}

bool CPed::IsPlayer() const
{
//    switch (m_nPedType) {
//        case PED_TYPE_PLAYER1:
//        case PED_TYPE_PLAYER2:
//            return true;
//    }
//    return false;
    return true;
}

/*!
 * �������� ����� �� ������. ��� ��������!
 */
void CPed::RemoveFromVehicle()
{
    if(!IsInVehicle())
        return;

    m_pIntelligence->m_TaskMgr.FlushImmediately();

    auto pos = pVehicle->GetPosition();
    float ang = pVehicle->GetHeading();

    pos.x += (1.0f * sin(ang + 4.71239f)); // 270 deg
    pos.y += (1.0f * sin(-ang + 4.71239f));

    Teleport(pos, false);
}

void CPed::StopNonPartialAnims() {
    for (auto assoc = RpAnimBlendClumpGetFirstAssociation(m_pRwClump); assoc; assoc = RpAnimBlendGetNextAssociation(assoc)) {
        if ((assoc->m_nFlags & ANIMATION_PARTIAL) == 0) {
            assoc->SetFlag(ANIMATION_STARTED, false);
        }
    }
}

CPed::CPed(ePedType pedType) : CPhysical(), m_pedIK{CPedIK(this)} {
    m_vecAnimMovingShiftLocal = CVector2D();

    m_fHealth = 100.0f;
    m_fMaxHealth = 100.0f;
    m_fArmour = 0.0f;

    m_nPedType = pedType;
    m_nType = ENTITY_TYPE_PED;

    // 0x5E8196
    physicalFlags.bCanBeCollidedWith = true;
    physicalFlags.bDisableTurnForce = true;

    m_nCreatedBy = PED_GAME;
    pVehicle = nullptr;
    m_nAntiSpazTimer = 0;
    m_nUnconsciousTimer = 0;
    m_nAttackTimer = 0;
    m_nLookTime = 0;
    m_nDeathTimeMS = 0;

    m_vecAnimMovingShift = CVector2D();
//    field_56C = CVector();
//    field_578 = CVector(0.0f, 0.0f, 1.0f);

    m_nPedState = PEDSTATE_IDLE;
    m_nMoveState = PEDMOVE_STILL;
    m_fCurrentRotation = 0.0f;
    m_fHeadingChangeRate = 15.0f;
    m_fMoveAnim = 0.1f;
    m_fAimingRotation = 0.0f;
  //  m_standingOnEntity = nullptr;
    m_nWeaponShootingRate = 40;
 //   field_594 = 0;
   // m_pEntityIgnoredCollision = nullptr;
   // m_nSwimmingMoveState = 0;
    m_pFire = nullptr;
    m_fireDmgMult = 1.0f;
   // m_pTargetedObject = nullptr;
    m_pLookTarget = nullptr;
    m_fLookDirection = 0.0f;
  //  m_pContactEntity = nullptr;
   // field_588 = 99999.992f;
    m_fMass = 70.0f;
    m_fTurnMass = 100.0f;
    m_fAirResistance = 1.f / 175.f;
    m_fElasticity = 0.05f;
//    m_nBodypartToRemove = -1;
    bHasACamera = CGeneral::GetRandomNumber() % 4 != 0;

   // m_weaponAudio.Initialise(this);
   // m_pedAudio.Initialise(this);

  //  m_acquaintance = CPedType::GetPedTypeAcquaintances(m_nPedType);
    m_nSavedWeapon = WEAPON_UNIDENTIFIED;
    m_nDelayedWeapon = WEAPON_UNIDENTIFIED;
    m_nActiveWeaponSlot = 0;

    for (auto& weapon : m_aWeapons ) {
        weapon.m_nType = WEAPON_UNARMED;
        weapon.m_nState = WEAPONSTATE_READY;
        weapon.dwAmmoInClip = 0;
        weapon.m_nTotalAmmo = 0;
      //  weapon.m_nTimeForNextShot = 0;
    }

    m_nWeaponSkill = eWeaponSkill::STD;
    m_nFightingStyle = STYLE_STANDARD;
    m_nAllowedAttackMoves = 0;

   // GiveWeapon(WEAPON_UNARMED, 0, true);

    m_nWeaponAccuracy = 60;
  //  m_nLastWeaponDamage = -1;
    m_pLastEntityDamage = nullptr;
  //  field_768 = 0;
    m_pAttachedTo = nullptr;
  //  m_nTurretAmmo = 0;
  //  field_460 = nullptr;
  // field_468 = 0;
    m_nWeaponModelId = -1;
 //   m_nMoneyCount = 0;
    field_72F = 0;
//    m_nTimeTillWeNeedThisPed = 0;
    m_VehDeadInFrontOf = nullptr;

    m_pWeaponObject = nullptr;
    m_pGunflashObject = nullptr;
    m_pGogglesObject = nullptr;
    m_pGogglesState = nullptr;

    m_nWeaponGunflashAlphaMP1 = 0;
    m_nWeaponGunFlashAlphaProgMP1 = 0;
    m_nWeaponGunflashAlphaMP2 = 0;
    m_nWeaponGunFlashAlphaProgMP2 = 0;

    m_pCoverPoint = nullptr;
 //   m_pEnex = nullptr;
  //  field_798 = -1;

//    m_pIntelligence = new CPedIntelligence(this);
    m_pPlayerData = nullptr;

//    if (!IsPlayer()) {
//        GetTaskManager().SetTaskSecondary(new CTaskComplexFacial{}, TASK_SECONDARY_FACIAL_COMPLEX);
//    }
  //  GetTaskManager().SetTask(new CTaskSimpleStandStill{ 0, true, false, 8.0 }, TASK_PRIMARY_DEFAULT, false);

   // field_758 = 0;
    m_fRemovalDistMultiplier = 1.0f;
    m_nSpecialModelIndex = -1;

 //   CPopulation::UpdatePedCount(this, 0);

//    if (CCheat::IsActive(CHEAT_HAVE_ABOUNTY_ON_YOUR_HEAD)) {
//        if (!IsPlayer()) {
//            GetAcquaintance().SetAsAcquaintance(ACQUAINTANCE_HATE, CPedType::GetPedFlag(ePedType::PED_TYPE_PLAYER1));
//
//            CEventAcquaintancePedHate event(FindPlayerPed());
//            GetEventGroup().Add(&event);
//        }
//    }
}

int CPed::GetSampSeatId() {
    if(!pVehicle)
        return -1;

    if( pVehicle->pDriver == this) return 0;

    if(pVehicle->m_apPassengers[0] == this) return 1;
    if(pVehicle->m_apPassengers[1] == this) return 2;
    if(pVehicle->m_apPassengers[2] == this) return 3;
    if(pVehicle->m_apPassengers[3] == this) return 4;
    if(pVehicle->m_apPassengers[4] == this) return 5;
    if(pVehicle->m_apPassengers[5] == this) return 6;
    if(pVehicle->m_apPassengers[6] == this) return 7;

    return (-1);
}

bool CPed::IsEnteringCar() {
    if ( GetTaskManager().CTaskManager::FindActiveTaskByType(TASK_COMPLEX_ENTER_CAR_AS_DRIVER) )
        return true;

    return GetTaskManager().CTaskManager::FindActiveTaskByType(TASK_COMPLEX_ENTER_CAR_AS_PASSENGER) != nullptr;
}

bool CPed::IsExitingVehicle() {
    if ( GetTaskManager().CTaskManager::FindActiveTaskByType(TASK_COMPLEX_LEAVE_CAR) )
        return true;

    return false;
}

CPed::~CPed() {
    CHook::CallFunction<void>(g_libGTASA + (VER_x32 ? 0x49F6A4 + 1 : 0x59541C), this);
}

eWeaponSkill CPed::GetWeaponSkill() {
    return GetWeaponSkill(GetActiveWeapon().m_nType);
}

eWeaponSkill CPed::GetWeaponSkill(eWeaponType weaponType)
{
    if (!CWeaponInfo::TypeHasSkillStats(weaponType)) {
        return eWeaponSkill::STD;
    }

    if (IsPlayer())
    {
        const auto GetReqStatLevelWith = [this, weaponType](eWeaponSkill skill) {
            return (float)CWeaponInfo::GetWeaponInfo(weaponType, skill)->m_nReqStatLevel;
        };

        const auto statValue = CHook::CallFunction<float>("_ZN6CStats12GetStatValueEt", (eStats)CWeaponInfo::GetSkillStatIndex(weaponType));
        if (statValue >= GetReqStatLevelWith(eWeaponSkill::PRO)) {
            return eWeaponSkill::PRO;
        } else if (statValue <= GetReqStatLevelWith(eWeaponSkill::POOR)) {
            return eWeaponSkill::POOR;
        } else {
            return eWeaponSkill::STD; // Somewhere in-between poor and pro stat levels
        }
    } else {
        if (weaponType == WEAPON_PISTOL && m_nPedType == PEDTYPE_COP)
            return eWeaponSkill::COP;
        return m_nWeaponSkill;
    }
}

void CPed::GetTransformedBonePosition(RwV3d& inOutPos, ePedBones bone, bool updateSkinBones) { // todo: fix this too!!!
    // Pretty much the same as GetBonePosition..
    if (updateSkinBones) {
        if (!bCalledPreRender) {
            UpdateRpHAnim();
            bCalledPreRender = true;
        }
    } else if (!bCalledPreRender) { // Return static local bone position instead
        inOutPos = m_matrix->TransformPoint(GetPedBoneStdPosition(bone));
        return;
    }

    // Return actual position
    RwV3dTransformPoints(&inOutPos, &inOutPos, 1, GetBoneMatrix(bone));
}

bool CPed::DoGunFlash(int32 duration, bool isLeftHand) {
    if (!m_pGunflashObject || !m_pWeaponObject) {
        return false;
    }

    // Really elegant.. ;D
    if (isLeftHand) {
        m_nWeaponGunflashAlphaMP2     = m_sGunFlashBlendStart;
        m_nWeaponGunFlashAlphaProgMP2 = (uint16)m_sGunFlashBlendStart / duration;
    } else {
        m_nWeaponGunflashAlphaMP1     = m_sGunFlashBlendStart;
        m_nWeaponGunFlashAlphaProgMP1 = (uint16)m_sGunFlashBlendStart / duration;
    }

    const auto angle = CGeneral::GetRandomNumberInRange(-360.0f, 360.0f);

    CVector vec = CPedIK::XaxisIK;
    RwMatrixRotate(RwFrameGetMatrix(RwFrameGetParent(m_pGunflashObject)), &vec, angle);
    return true;
}

void CPed::SetCurrentWeapon(int32 slot) {
    CHook::CallFunction<void>("_ZN4CPed16SetCurrentWeaponEi", this, slot);
}

void CPed::SetCurrentWeapon(eWeaponType weaponType) {
    SetCurrentWeapon(GetWeaponSlot(weaponType));
}

int32 CPed::GetWeaponSlot(eWeaponType weaponType) {
    return CWeaponInfo::GetWeaponInfo(weaponType)->m_nSlot;
}

void CPed::SetAmmo(eWeaponType weaponType, uint32 ammo) {
    const auto wepSlot = GetWeaponSlot(weaponType);
    if (wepSlot != -1) {
        auto& wepInSlot = GetWeaponInSlot(wepSlot);

        if (ammo < 0) {
            wepInSlot.m_nTotalAmmo = 0;
            wepInSlot.m_nAmmoInClip = 0;
        } else {
            wepInSlot.m_nTotalAmmo = std::min(ammo, 99'999u);
            wepInSlot.m_nAmmoInClip = std::max(wepInSlot.m_nTotalAmmo, wepInSlot.m_nAmmoInClip);
        }

        if (wepInSlot.m_nState == WEAPONSTATE_OUT_OF_AMMO) {
            if (wepInSlot.m_nTotalAmmo > 0) {
                wepInSlot.m_nState = WEAPONSTATE_READY;
            }
        }
    }
}
