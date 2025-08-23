//
// Created on 14.10.2023.
//

#include "../game/common.h"
#include "Race.h"
#include "net/netgame.h"

void CRace::Update(uint8 curPos, uint8 maxPos, uint8 curChecks, uint8 maxChecks, uint32 time) {
    if(!thiz)
        Constructor();

    auto env = CJavaWrapper::GetEnv();

    auto method = env->GetMethodID(clazz, "update", "(IIIIJ)V");
    env->CallVoidMethod(thiz, method, curPos, maxPos, curChecks, maxChecks, (jlong)time);
}

void CRace::ReceivePacket(Packet* p) {
    RakNet::BitStream bs((unsigned char*)p->data, p->length, false);
    bs.IgnoreBits(40);

    RPC_TYPE type;
    bs.Read(type);

    if(type == RPC_TYPE::TOGGLE) {
        uint8 toggle;
        bs.Read(toggle);

        if(toggle)
            Constructor();
        else
            Destroy();

        return;
    }
    if(type == RPC_TYPE::UPDATE) {
        uint8 curPos, maxPos, curChecks, maxChecks;
        uint32 time;

        bs.Read(curPos);
        bs.Read(maxPos);
        bs.Read(curChecks);
        bs.Read(maxChecks);
        bs.Read(time);

        Update(curPos, maxPos, curChecks, maxChecks, time);
        return;
    }
}
