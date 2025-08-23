#pragma once

#include "include/util/Memory.hpp"

namespace Voice {

#pragma pack(push, 1)
struct ControlPacket
{
    uint16_t packet;
    uint16_t length;
    uint8_t* data;
};
#pragma pack(pop)

}