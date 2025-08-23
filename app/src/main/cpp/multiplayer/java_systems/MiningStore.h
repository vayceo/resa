//
// Created on 24.11.2023.
//

#pragma once

#include "GuiWrapper.h"

class CMiningStore : public CGuiWrapper<CMiningStore> {
    enum class ePacketType : uint8_t  {
        SHOW,
        HIDE
    };

    enum class eButtonId : uint8_t {
        RIGHT,
        LEFT,
        BUY,
        EXIT
    };

public:
    static void ReceivePacket(Packet* p);
    static void Show(const std::string& price, const std::string& caption, const std::string& specs, const std::string& description);
};