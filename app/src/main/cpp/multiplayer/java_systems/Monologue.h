//
// Created on 19.11.2023.
//

#ifndef RUSSIA_MONOLOGUE_H
#define RUSSIA_MONOLOGUE_H


#include "GuiWrapper.h"

class CMonologue : public CGuiWrapper<CMonologue> {
public:
    static inline uint16_t monologueId {};

    enum class eRpcType : uint8_t {
        SHOW,
        HIDE
    };

public:
    static void ReceivePacket(Packet* p);
    static void Show(int skinId, const char *text, const char *author, const char *button);
};


#endif //RUSSIA_MONOLOGUE_H
