/*
    Plugin-SDK file
    Authors: GTA Community. See more here
    https://github.com/DK22Pac/plugin-sdk
    Do not delete this comment block. Respect others' work!
*/
#pragma once

#include "main.h"

class CLoadedCarGroup {
public:
    CLoadedCarGroup() { Clear(); }

    //! Remove all models
    void Clear();
    uint32 CountMembers() const;

private:
    std::array<int16, 23> m_models{}; //< Model IDs. Empty slots are marked by `SENTINEL_VALUE_OF_UNUSED` (See cpp file) (NOTE: Use `GetAllModels()` when iterating, etc!)
};
