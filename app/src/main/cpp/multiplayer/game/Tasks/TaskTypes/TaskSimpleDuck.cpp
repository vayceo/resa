//
// Created on 21.05.2024.
//

#include "TaskSimpleDuck.h"

void CTaskSimpleDuck::ForceStopMove() {
    m_bIsInControl = true;
    m_MoveCmd.y = 0.f;
}
