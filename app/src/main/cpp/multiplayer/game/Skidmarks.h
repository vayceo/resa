//
// Created on 23.09.2023.
//

#ifndef RUSSIA_SKIDMARKS_H
#define RUSSIA_SKIDMARKS_H


#include "common.h"

class CSkidmarks {
public:
    static void InjectHooks();

    static void Init();
    static void Shutdown();
    static void Clear();
    static void Update();
    static void Render();
};


#endif //RUSSIA_SKIDMARKS_H
