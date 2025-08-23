#include "../net/netgame.h"
#include "../gui/gui.h"

#include "../CSettings.h"
#include "SkyBox.h"
#include "game/Models/ModelInfo.h"
#include "game/game.h"
#include "game/Clock.h"
#include "game/Camera.h"
#include "game/TimeCycle.h"
#include "game/Entity/Ped/Ped.h"
#include "app/app_light.h"

extern CGUI *pGUI;

void CSkyBox::Init()
{

}

void CSkyBox::Process() {

}

CObjectSamp* CSkyBox::CreateObjectScaled(int iModel, float fScale)
{
    if (!pNetGame)
        return nullptr;

    CModelInfo::GetModelInfo(iModel)->m_nRefCount = 899;

    CVector vecRot(0.f, 0.f, 0.f);
    CVector vecScale(fScale);

    auto *object = new CObjectSamp(iModel, 0.0f, 0.0f, 0.0f, vecRot, 0.0f);
    CEntity* entity = object->m_pEntity;

    entity->m_bUsesCollision = false;
    entity->Remove();

    RwMatrix matrix;
    entity->GetMatrix(&matrix);
    RwMatrixScale(&matrix, &vecScale);

    entity->SetMatrix(reinterpret_cast<CMatrix&>(matrix));
    entity->UpdateRW();
    entity->UpdateRwFrame();

    entity->Add();
    entity->m_bUsesCollision = false;

    return object;
}

void CSkyBox::ReTexture()
{
    int iHours = CClock::GetGameClockHours();

    if (m_dwChangeTime != iHours)
    {
        m_dwChangeTime = iHours;

        if (iHours >= 0 && iHours < 6 || iHours > 18 ) { // ����
            SetTexture("skybox_1");
        } else if (iHours >= 6 && iHours < 8) { // �������
            SetTexture("skybox_2");
        } else if (iHours >= 8 && iHours < 11) {
            SetTexture("skybox_3");
        } else if (iHours >= 11 && iHours <= 15) {
            SetTexture("skybox_4");
        } else if (iHours == 16) {
            SetTexture("skybox_5");
        } else if (iHours == 17) {
            SetTexture("skybox_6");
        } else if (iHours == 18) { // �����
            SetTexture("skybox_7");
        }
//        else if (iHours == 20) { // ����� 2
//            SetTexture("skybox_8");
//        }


    }
    // ---

    auto pAtomic = m_pSkyObject->m_pEntity->m_pRwObject;

    if (!pAtomic || !pAtomic->parent)
        return;

    DeActivateDirectional();
    SetFullAmbient();
    SetAmbientColours();

    RwFrameForAllObjects((RwFrame*)pAtomic->parent, RwFrameForAllObjectsCallback, m_pSkyObject);
}

RwObject* CSkyBox::RwFrameForAllObjectsCallback(RwObject* object, void* data)
{
    if (!object || object->type != rpATOMIC || !m_pTex)
        return object;

    auto* atomic = reinterpret_cast<RpAtomic*>(object);
    if (!atomic->geometry)
        return object;

    auto& geometry = *atomic->geometry;
    auto materials = std::span(geometry.matList.materials,
                               std::min<size_t>(geometry.matList.numMaterials, 16));

    const auto& skyColor = CTimeCycle::m_CurrentColours;
    const RwRGBA rgbaColor = {
            static_cast<RwUInt8>(skyColor.m_nSkyBottomRed),
            static_cast<RwUInt8>(skyColor.m_nSkyBottomGreen),
            static_cast<RwUInt8>(skyColor.m_nSkyBottomBlue),
            150
    };

    for (auto* material : materials)
    {
        if (!material)
            continue;

        material->texture = m_pTex;
        material->color = rgbaColor;
    }

    return object;
}

void CSkyBox::SetTexture(const char *texName)
{
    if (texName == nullptr)
        return;

    if(m_pTex) {
        RwTextureDestroy(m_pTex);
    }

    m_pTex = CUtil::LoadTextureFromDB("gta3", texName);
}

void CSkyBox::SetRotSpeed(float speed)
{
    m_fRotSpeed = speed;
}

bool CSkyBox::IsNeedRender()
{
    return m_bNeedRender;
}

CObjectSamp *CSkyBox::GetSkyObject()
{
    return m_pSkyObject;
}

