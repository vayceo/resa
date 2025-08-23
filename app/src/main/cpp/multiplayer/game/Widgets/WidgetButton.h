//
// Created by plakapenka on 04.09.2023.
//

#ifndef RUSSIA_WIDGETBUTTON_H
#define RUSSIA_WIDGETBUTTON_H


#include "WidgetGta.h"
#include "../sprite2d.h"

struct CWidgetButton : CWidgetGta
{
    uint32      m_nButtonFlags;
    float       m_fPulssateTime;
    CSprite2d   m_SpriteFill;
    float       m_fFillLevel;
    float       m_fTVTop;
    float       m_fTVBottom;
};



#endif //RUSSIA_WIDGETBUTTON_H
