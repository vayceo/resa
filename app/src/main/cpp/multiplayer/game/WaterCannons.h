//
// Created on 23.09.2023.
//

#ifndef RUSSIA_WATERCANNONS_H
#define RUSSIA_WATERCANNONS_H


class CWaterCannons {
public:
    static inline bool bFromLocalPlayer{};

public:
    static void InjectHooks();

    static void Init();
    static void Update();
    static void Render();
};


#endif //RUSSIA_WATERCANNONS_H
