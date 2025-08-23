//
// Created on 18.08.2023.
//

#ifndef RUSSIA_SKINSHOP_H
#define RUSSIA_SKINSHOP_H


#include "GuiWrapper.h"

class CSkinShop : public CGuiWrapper<CSkinShop> {

public:
    static void Update(int type, int price);
};


#endif //RUSSIA_SKINSHOP_H
