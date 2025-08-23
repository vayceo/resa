//
// Created on 20.09.2023.
//

#ifndef RUSSIA_POSTEFFECTS_H
#define RUSSIA_POSTEFFECTS_H


class CPostEffects {

public:

    static void ImmediateModeRenderStatesSet();
    static void ImmediateModeRenderStatesStore();
    static void ImmediateModeRenderStatesReStore();

    static void Render();
    static void MobileRender();
};


#endif //RUSSIA_POSTEFFECTS_H
