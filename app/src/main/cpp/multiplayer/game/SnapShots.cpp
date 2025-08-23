//
// Created on 19.09.2023.
//

#include "SnapShots.h"
#include "RW/rwcore.h"
#include "game/Core/Vector.h"
#include "main.h"
#include "PedSamp.h"
#include "Entity/Ped/Ped.h"
#include "util.h"
#include "game/Plugins/RpAnimBlendPlugin/RpAnimBlend.h"
#include "VisibilityPlugins.h"
#include "game/Models/ModelInfo.h"
#include "util/patch.h"
#include "net/netgame.h"
#include "Pools.h"
#include "Streaming.h"
#include "ObjectSamp.h"
#include "game.h"
#include "SkyBox.h"

static uint32_t VehicleColoursTableRGBA[256] = {
        // The existing colours from San Andreas
        0x000000FF, 0xF5F5F5FF, 0x2A77A1FF, 0x840410FF, 0x263739FF, 0x86446EFF, 0xD78E10FF, 0x4C75B7FF, 0xBDBEC6FF, 0x5E7072FF,
        0x46597AFF, 0x656A79FF, 0x5D7E8DFF, 0x58595AFF, 0xD6DAD6FF, 0x9CA1A3FF, 0x335F3FFF, 0x730E1AFF, 0x7B0A2AFF, 0x9F9D94FF,
        0x3B4E78FF, 0x732E3EFF, 0x691E3BFF, 0x96918CFF, 0x515459FF, 0x3F3E45FF, 0xA5A9A7FF, 0x635C5AFF, 0x3D4A68FF, 0x979592FF,
        0x421F21FF, 0x5F272BFF, 0x8494ABFF, 0x767B7CFF, 0x646464FF, 0x5A5752FF, 0x252527FF, 0x2D3A35FF, 0x93A396FF, 0x6D7A88FF,
        0x221918FF, 0x6F675FFF, 0x7C1C2AFF, 0x5F0A15FF, 0x193826FF, 0x5D1B20FF, 0x9D9872FF, 0x7A7560FF, 0x989586FF, 0xADB0B0FF,
        0x848988FF, 0x304F45FF, 0x4D6268FF, 0x162248FF, 0x272F4BFF, 0x7D6256FF, 0x9EA4ABFF, 0x9C8D71FF, 0x6D1822FF, 0x4E6881FF,
        0x9C9C98FF, 0x917347FF, 0x661C26FF, 0x949D9FFF, 0xA4A7A5FF, 0x8E8C46FF, 0x341A1EFF, 0x6A7A8CFF, 0xAAAD8EFF, 0xAB988FFF,
        0x851F2EFF, 0x6F8297FF, 0x585853FF, 0x9AA790FF, 0x601A23FF, 0x20202CFF, 0xA4A096FF, 0xAA9D84FF, 0x78222BFF, 0x0E316DFF,
        0x722A3FFF, 0x7B715EFF, 0x741D28FF, 0x1E2E32FF, 0x4D322FFF, 0x7C1B44FF, 0x2E5B20FF, 0x395A83FF, 0x6D2837FF, 0xA7A28FFF,
        0xAFB1B1FF, 0x364155FF, 0x6D6C6EFF, 0x0F6A89FF, 0x204B6BFF, 0x2B3E57FF, 0x9B9F9DFF, 0x6C8495FF, 0x4D8495FF, 0xAE9B7FFF,
        0x406C8FFF, 0x1F253BFF, 0xAB9276FF, 0x134573FF, 0x96816CFF, 0x64686AFF, 0x105082FF, 0xA19983FF, 0x385694FF, 0x525661FF,
        0x7F6956FF, 0x8C929AFF, 0x596E87FF, 0x473532FF, 0x44624FFF, 0x730A27FF, 0x223457FF, 0x640D1BFF, 0xA3ADC6FF, 0x695853FF,
        0x9B8B80FF, 0x620B1CFF, 0x5B5D5EFF, 0x624428FF, 0x731827FF, 0x1B376DFF, 0xEC6AAEFF, 0x000000FF,
        // SA-MP extended colours (0.3x)
        0x177517FF, 0x210606FF, 0x125478FF, 0x452A0DFF, 0x571E1EFF, 0x010701FF, 0x25225AFF, 0x2C89AAFF, 0x8A4DBDFF, 0x35963AFF,
        0xB7B7B7FF, 0x464C8DFF, 0x84888CFF, 0x817867FF, 0x817A26FF, 0x6A506FFF, 0x583E6FFF, 0x8CB972FF, 0x824F78FF, 0x6D276AFF,
        0x1E1D13FF, 0x1E1306FF, 0x1F2518FF, 0x2C4531FF, 0x1E4C99FF, 0x2E5F43FF, 0x1E9948FF, 0x1E9999FF, 0x999976FF, 0x7C8499FF,
        0x992E1EFF, 0x2C1E08FF, 0x142407FF, 0x993E4DFF, 0x1E4C99FF, 0x198181FF, 0x1A292AFF, 0x16616FFF, 0x1B6687FF, 0x6C3F99FF,
        0x481A0EFF, 0x7A7399FF, 0x746D99FF, 0x53387EFF, 0x222407FF, 0x3E190CFF, 0x46210EFF, 0x991E1EFF, 0x8D4C8DFF, 0x805B80FF,
        0x7B3E7EFF, 0x3C1737FF, 0x733517FF, 0x781818FF, 0x83341AFF, 0x8E2F1CFF, 0x7E3E53FF, 0x7C6D7CFF, 0x020C02FF, 0x072407FF,
        0x163012FF, 0x16301BFF, 0x642B4FFF, 0x368452FF, 0x999590FF, 0x818D96FF, 0x99991EFF, 0x7F994CFF, 0x839292FF, 0x788222FF,
        0x2B3C99FF, 0x3A3A0BFF, 0x8A794EFF, 0x0E1F49FF, 0x15371CFF, 0x15273AFF, 0x375775FF, 0x060820FF, 0x071326FF, 0x20394BFF,
        0x2C5089FF, 0x15426CFF, 0x103250FF, 0x241663FF, 0x692015FF, 0x8C8D94FF, 0x516013FF, 0x090F02FF, 0x8C573AFF, 0x52888EFF,
        0x995C52FF, 0x99581EFF, 0x993A63FF, 0x998F4EFF, 0x99311EFF, 0x0D1842FF, 0x521E1EFF, 0x42420DFF, 0x4C991EFF, 0x082A1DFF,
        0x96821DFF, 0x197F19FF, 0x3B141FFF, 0x745217FF, 0x893F8DFF, 0x7E1A6CFF, 0x0B370BFF, 0x27450DFF, 0x071F24FF, 0x784573FF,
        0x8A653AFF, 0x732617FF, 0x319490FF, 0x56941DFF, 0x59163DFF, 0x1B8A2FFF, 0x38160BFF, 0x041804FF, 0x355D8EFF, 0x2E3F5BFF,
        0x561A28FF, 0x4E0E27FF, 0x706C67FF, 0x3B3E42FF, 0x2E2D33FF, 0x7B7E7DFF, 0x4A4442FF, 0x28344EFF
};

RwTexture* CSnapShots::CreateVehicleSnapShot(uint32 modelId, uint32_t dwColor, CVector* vecRot, CVector* offset)
{
    auto colorId = CGeneral::GetRandomNumberInRange(0, 7);
    auto color = VehicleColoursTableRGBA[colorId];

    constexpr auto tempId = 9999;

    auto pNewVehicle = NewVehiclePacket{static_cast<VEHICLEID>(tempId), modelId};
    CVehiclePool::New(&pNewVehicle);

    auto pVehicle = CVehiclePool::GetAt(tempId);

    pVehicle->m_pVehicle->SetCollisionChecking(false);
    //pVehicle->SetCollisionChecking();

    CVector vecCenter = CModelInfo::GetModelInfo(modelId)->m_pColModel->GetBoundCenter();
    float fRadius = CModelInfo::GetModelInfo(modelId)->m_pColModel->GetBoundRadius();

    float posY = ( -(fRadius + fRadius)) + offset->y;

    if(pVehicle->GetVehicleSubtype() == VEHICLE_SUBTYPE_BOAT)
    {
        posY = -5.5f - fRadius * 2.5f;
    }

    CVector pos(vecCenter.x, posY, 50.f + vecCenter.z + offset->z);
    pVehicle->m_pVehicle->Teleport(pos, false);

    pVehicle->mainColor = color;
    pVehicle->mainColor.a = 255;
    pVehicle->tonerColor = 0x000000BE;

    RwMatrix mat;
    pVehicle->m_pVehicle->GetMatrix(&mat);

    CVector axis { 1.0f, 0.0f, 0.0f };
    if (vecRot->x != 0.0f)
    {
        RwMatrixRotate(&mat, &axis, vecRot->x);
    }
    axis.Set( 0.0f, 1.0f, 0.0f );
    if (vecRot->y != 0.0f)
    {
        RwMatrixRotate(&mat, &axis, vecRot->y);
    }
    axis.Set( 0.0f, 0.0f, 1.0f );
    if (vecRot->z != 0.0f)
    {
        RwMatrixRotate(&mat, &axis, vecRot->z);
    }

    pVehicle->m_pVehicle->SetMatrix((CMatrix&)mat);

    pVehicle->m_pVehicle->UpdateRW();

    CRenderTarget::Begin(500, 500, (RwRGBA*)&dwColor, false);

    CUtil::RenderEntity(pVehicle->m_pVehicle);

    CVehiclePool::Delete(tempId);

    return CRenderTarget::End();
}

RwTexture* CSnapShots::CreatePedSnapShot(int iModel, uint32_t dwColor, CVector* vecRot, CVector* offset)
{
    Log("CreatePedSnapShot: %d, %f, %f, %f", iModel, vecRot->x, vecRot->y, vecRot->z);

    float posZ = 49.75f + offset->z;
    float posY = -2.20f + offset->y;


    auto pPed = new CPedSamp(208, iModel, 0.0f, posY, posZ, 0.0f);

    pPed->m_pPed->SetCollisionChecking(false);

    RwMatrix mat;
    pPed->m_pPed->GetMatrix(&mat);

    CVector axis { 1.0f, 0.0f, 0.0f };
    if (vecRot->x != 0.0f)
    {
        RwMatrixRotate(&mat, &axis, vecRot->x);
    }
    axis.Set( 0.0f, 1.0f, 0.0f );
    if (vecRot->y != 0.0f)
    {
        RwMatrixRotate(&mat, &axis, vecRot->y);
    }
    axis.Set( 0.0f, 0.0f, 1.0f );
    if (vecRot->z != 0.0f)
    {
        RwMatrixRotate(&mat, &axis, vecRot->z);
    }

    pPed->m_pPed->SetMatrix((CMatrix&)mat);
    pPed->m_pPed->UpdateRW();

    CRenderTarget::Begin(500, 500, (RwRGBA*)&dwColor, false);

    RpAnimBlendClumpUpdateAnimations(pPed->m_pPed->m_pRwClump, 100.f, true);

    CUtil::RenderEntity(pPed->m_pPed);

    delete pPed;

    return CRenderTarget::End();
}

void RenderClumpOrAtomic(uintptr_t rwObject)
{
    if (rwObject)
    {
        if (*(uint8_t *) rwObject == 1)
        {
            // Atomic
            AtomicDefaultRenderCallBack(reinterpret_cast<RpAtomic *>(rwObject));
        }
        else if (*(uint8_t *) rwObject == 2)
        {
            // rpClumpRender
            ((void (*)(uintptr_t))(g_libGTASA + (VER_x32 ? 0x21425C + 1 : 0x2BA6A4))) (rwObject);
        }
    }
}


void DestroyAtomicOrClump(RwObject* rwObject)
{
    if (rwObject)
    {
        int type = *(int *)(rwObject);

        if (type == 1)
        {
            RpAtomicDestroy(reinterpret_cast<RpAtomic *>(rwObject));

            auto parent = rwObject->parent;
            if (parent)
            {
                RwFrameDestroy(reinterpret_cast<RwFrame *>(parent));
            }

        }
        else if (type == 2)
        {
            RpClumpDestroy(reinterpret_cast<RpClump *>(rwObject));
        }
    }
}

RwObject* ModelInfoCreateInstance(int iModel)
{
    auto modelInfo = CModelInfo::GetModelInfo(iModel);
    if (modelInfo)
    {
        return CHook::CallVTableFunctionByNum<RwObject*>(modelInfo, 11);
    }

    return nullptr;
}

RwTexture* CSnapShots::CreateObjectSnapShot(int modelId, CVector* vecRot, CVector* offset)
{
    Log("CreateObjectSnapShot: %d, %f, %f, %f", modelId, vecRot->x, vecRot->y, vecRot->z);
    CStreaming::TryLoadModel(modelId);

    auto pRwObject = ModelInfoCreateInstance(modelId);
    if(pRwObject == nullptr)
    {
        Log("pRwObject = null");
        return nullptr;
    }

    float fRadius = CModelInfo::GetModelInfo(modelId)->m_pColModel->GetBoundRadius();
    CVector vecCenter = CModelInfo::GetModelInfo(modelId)->m_pColModel->GetBoundCenter();

    auto parent = (RwFrame*)pRwObject->parent;

    if (parent == nullptr)
        return nullptr;

    RwV3d v = {
            -vecCenter.x + offset->x,
            (-0.1f - (fRadius * 2.25f)) + offset->y,
            50.0f - vecCenter.z - offset->z
    };
    RwFrameTranslate(parent, &v, rwCOMBINEPRECONCAT);
    if (vecRot->x != 0.0f)
    {
        v = {1.0f, 0.f, 0.f};
        RwFrameRotate(parent, &v, vecRot->x, rwCOMBINEPRECONCAT);
    }

    if (vecRot->y != 0.0f)
    {
        v = {0.0f, 1.f, 0.f};
        RwFrameRotate(parent, &v, vecRot->y, rwCOMBINEPRECONCAT);
    }

    if (vecRot->z != 0.0f)
    {
        v = {0.0f, 0.f, 1.f};
        RwFrameRotate(parent, &v, vecRot->z, rwCOMBINEPRECONCAT);
    }

    uint32 dwColor = 0;
    CRenderTarget::Begin(500, 500, (RwRGBA*)&dwColor, false);
    RenderClumpOrAtomic((uintptr_t)pRwObject);

    DestroyAtomicOrClump(pRwObject);

    CStreaming::RemoveModelIfNoRefs(modelId);
    return CRenderTarget::End();
}

