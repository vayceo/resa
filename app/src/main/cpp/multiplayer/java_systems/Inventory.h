//
// Created on 18.11.2022.
//

#pragma once


#include "GuiWrapper.h"

class CInventory : public CGuiWrapper<CInventory>{
public:
    enum class RpcTypes : uint8 {
        TOGGLE,
        ITEM_SELECT,
        UPDATE_CARRYING,
        UPDATE_ITEM,
        CLICK_CELL,
        CLICK_BUTTON,
        SHOW_ADDITIONAL
    };

public:
    static void ReceivePacket(Packet* p);
    static void ToggleShow(bool toggle);

    static void UpdateItem(int matrixId, int pos, const std::string& sprite, const std::string& caption, const std::string &count, int rare, bool active);
    static void UpdateCarryng(int matrixindex, std::string& carryng);

    static void updateItem(int matrixindex, int pos, bool active);

    static void ToggleAdditionalMatrix(bool toggle, const char *caption);
};
