//
// Created on 20.11.2023.
//

#ifndef RUSSIA_MODELSDEBUGMODULE_H
#define RUSSIA_MODELSDEBUGMODULE_H


#include "DebugModule.h"

class CModelsDebugModule : public DebugModule {

public:
    static inline bool m_bEnable{};

public:
    void RenderWindow() override final;
    void RenderMenuEntry() override final;

    static void DrawModelInfo(CEntity *entity);
};


#endif //RUSSIA_MODELSDEBUGMODULE_H
