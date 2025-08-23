//
// Created on 13.03.2024.
//

#include <unistd.h>
#include "OS_wrapper.h"

void OS_ThreadSleep(uint32_t microsecond)
{
    usleep(microsecond);
}



void COS_wrapper::InjectHooks() {

}
