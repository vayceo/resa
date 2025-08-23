//
// Created on 22.01.2024.
//

#include "WarPoints.h"
#include "../game/common.h"
#include "../net/netgame.h"

void CNetGame::packetMafiaWar(Packet* p)
{
    RakNet::BitStream bs((unsigned char*)p->data, p->length, false);
    bs.IgnoreBits(40); // skip packet and rpc id

    uint16 time, myScore, enemyScore;

    bs.Read(time);
    if(time <= 0){
        CWarPoints::Destroy();
        return;
    }

    bs.Read(myScore);
    bs.Read(enemyScore);

    CWarPoints::Update(time, myScore, enemyScore);
}

void CWarPoints::Update(int time, int myScore, int enemyScore) {
    if(!thiz)
        Constructor();

    auto env = g_pJavaWrapper->GetEnv();

    auto method = env->GetMethodID(clazz, "update", "(III)V");
    env->CallVoidMethod(thiz, method, time, myScore, enemyScore);
}
