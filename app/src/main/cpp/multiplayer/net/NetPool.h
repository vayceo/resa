//
// Created on 29.01.2024.
//

#pragma once

#include "game/common.h"


template<class T>
class CNetPool {
public:
    static inline std::unordered_map<uint32, T> list {};
    static inline std::unordered_map<CEntity*, uint32_t> entityToIdMap {};
    static inline std::unordered_map<RwObject*, uint32_t> rwObjectToIdMap {};

public:
    static T GetAt(uint32 id)
    {
        auto it = list.find(id);
        return it != list.end() ? it->second : nullptr;
    }

    static uint32_t GetEntity(CEntity* entity)
    {
        auto it = entityToIdMap.find(entity);
        return it != entityToIdMap.end() ? it->second : 0xFFFF;
    }

    static uint32_t GetRwObject(RwObject* rwObject)
    {
        auto it = rwObjectToIdMap.find(rwObject);
        return it != rwObjectToIdMap.end() ? it->second : 0xFFFF;
    }

    static std::vector<uint32> GetAllIds()
    {
        std::vector<uint32> keysToDelete;

        for (auto& element : list) {
            keysToDelete.push_back(element.first);
        }

        return keysToDelete;
    }
};
