//
// Created on 23.09.2023.
//

#ifndef RUSSIA_SPECIALFX_H
#define RUSSIA_SPECIALFX_H


class CSpecialFX {
public:
    static void InjectHooks();

    static void Init();
    static void Update();
    static void Shutdown();
    static void Render();
    static void Render2DFXs();
    static void ReplayStarted();
};


#endif //RUSSIA_SPECIALFX_H
