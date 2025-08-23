//
// Created on 15.07.2023.
//



#include <jni.h>
#include "GuiWrapper.h"


class CAchivments : public CGuiWrapper<CAchivments>{
public:

public:
    static void updateItem(int index, int progress, int step);
    static void packetAchivments(Packet *p);
};
