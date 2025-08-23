//
// Created on 18.01.2024.
//

#include "BusStation.h"
#include "../net/netgame.h"
#include "../game/common.h"

void CBusStation::ReceivePacket(Packet *p) {
    RakNet::BitStream bs((unsigned char*)p->data, p->length, false);
    bs.IgnoreBits(40); // skip packet and rpc id

    uint8 toggle;
    uint32 time;
    bs.Read(toggle);
    bs.Read(time);

    if (toggle == 1) {
        Update(time);
    } else {
        Destroy();
    }
}

void CBusStation::Update(int time) {
    if(!thiz)
        Constructor();

    auto env = g_pJavaWrapper->GetEnv();

    auto method = env->GetMethodID(clazz, "update", "(I)V");
    env->CallVoidMethod(thiz, method, time);
}
