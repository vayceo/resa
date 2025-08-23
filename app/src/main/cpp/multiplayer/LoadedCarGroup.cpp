/*
    Plugin-SDK file
    Authors: GTA Community. See more here
    https://github.com/DK22Pac/plugin-sdk
    Do not delete this comment block. Respect others' work!
*/

#include "LoadedCarGroup.h"

constexpr auto SENTINEL_VALUE_OF_UNUSED = (int16)(MODEL_INVALID);

void CLoadedCarGroup::Clear() {
    std::ranges::fill(m_models, -1);
}

uint32 CLoadedCarGroup::CountMembers() const {
    return (uint32)(std::ranges::distance(m_models.begin(), std::ranges::find(m_models, SENTINEL_VALUE_OF_UNUSED)));
}