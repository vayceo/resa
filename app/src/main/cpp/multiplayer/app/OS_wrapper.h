//
// Created on 13.03.2024.
//

#pragma once

#include <unistd.h>

class COS_wrapper {
public:
    static void InjectHooks();
};

extern void OS_ThreadSleep(uint32_t microsecond);
