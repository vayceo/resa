//
// Created on 07.04.2023.
//

#pragma once

enum eMoveState : int32_t
{
    PEDMOVE_NONE = 0x0,
    PEDMOVE_STILL = 0x1,
    PEDMOVE_TURN_L = 0x2,
    PEDMOVE_TURN_R = 0x3,
    PEDMOVE_WALK = 0x4,
    PEDMOVE_JOG = 0x5,
    PEDMOVE_RUN = 0x6,
    PEDMOVE_SPRINT = 0x7,
};