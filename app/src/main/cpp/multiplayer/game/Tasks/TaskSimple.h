/*
    Plugin-SDK file
    Authors: GTA Community. See more here
    https://github.com/DK22Pac/plugin-sdk
    Do not delete this comment block. Respect others' work!
*/
#pragma once

#include "Task.h"

class CPed;

class CTaskSimple : public CTask {
public:
    CTaskSimple();
    ~CTaskSimple() override = default;

    CTask* GetSubTask() override;
    bool IsSimple() override;
    virtual bool ProcessPed(CPed* ped) = 0;
    virtual bool SetPedPosition(CPed* ped);
};
VALIDATE_SIZE(CTaskSimple, (VER_x32 ? 0x8 : 0x10));
