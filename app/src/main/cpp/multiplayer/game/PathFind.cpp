//
// Created on 15.10.2023.
//

#include "PathFind.h"
#include "util/patch.h"

void CPathFind::InjectHooks() {
    SET_TO(ThePaths, CHook::getSym("ThePaths"));
  //  ThePaths = CHook::CallFunction<>()
}

void CPathFind::DoPathSearch(
        ePathType pathType,
        CVector originPos,
        CNodeAddress originAddrAddrHint, // If invalid/area not loaded the closest node to `originPos` is used.
        CVector targetPos,
        CNodeAddress* outResultNodes,
        int16* outNodesCount,
        int32 maxNodesToFind,
        float* outDistance,
        float maxSearchDistance,
        CNodeAddress* targetNodeAddrHint, // If null/invalid/area not loaded the closest node to `targetPos` is used.
        float maxSearchDepth,
        bool sameLaneOnly,
        CNodeAddress forbiddenNodeAddr,
        bool bAllowWaterNodeTransitions,
        bool forBoats
) {
    // Moved this up here, as it's set in every return path
    CHook::CallFunction<void>(g_libGTASA + (VER_x32 ? 0x00315898 + 1 : 0x3DBC30),
                              this,
                              pathType,
                              originPos,
                              originAddrAddrHint,
                              targetPos,
                              outResultNodes,
                              outNodesCount,
                              maxNodesToFind,
                              outDistance,
                              maxSearchDistance,
                              targetNodeAddrHint,
                              maxSearchDepth,
                              sameLaneOnly,
                              forbiddenNodeAddr, bAllowWaterNodeTransitions, forBoats);

}


CPathNode* CPathFind::GetPathNode(CNodeAddress address) {
    assert(address.IsValid());
    assert(IsAreaNodesAvailable(address));
    return &m_pPathNodes[address.m_wAreaId][address.m_wNodeId];
}

void CPathFind::UpdateStreaming(bool bForceStreaming) {
    CHook::CallFunction<void>(g_libGTASA + (VER_x32 ? 0x003195F4 + 1 : 0x3E0208), this, bForceStreaming);
}

std::span<CPathNode> CPathFind::GetPathNodesInArea(size_t areaId, ePathType ptype) const {
    if (const auto allNodes = m_pPathNodes[areaId]) {
        const auto numVehNodes = m_anNumVehicleNodes[areaId];
        switch (ptype) {
            case ePathType::PATH_TYPE_VEH: // Vehicles, boats, race tracks
                return std::span{ allNodes, m_anNumVehicleNodes[areaId] };
            case ePathType::PATH_TYPE_PED: // Peds only
                assert(m_anNumPedNodes[areaId] == m_anNumNodes[areaId] - numVehNodes); // Pirulax: I'm assuming this is true, so if this doesnt assert for a long time remove it
                return std::span{ allNodes + numVehNodes, m_anNumPedNodes[areaId] };
            case ePathType::PATH_TYPE_ALL: // All of the above
                return std::span{ allNodes, m_anNumNodes[areaId] };
            default:
                Log("Invalid pathType: %d", (int)ptype);
        }
    }
    return {}; // Area not loaded, return nothing.. Perhaps assert here instead?
}

bool CPathFind::IsWaterNodeNearby(CVector position, float radius) {
    for (auto areaId = 0u; areaId < NUM_PATH_MAP_AREAS; areaId++) {
        for (const auto& node : GetPathNodesInArea(areaId, PATH_TYPE_VEH)) {
            if (node.m_bWaterNode) {
                if ((node.GetNodeCoors() - position).SquaredMagnitude() <= sq(radius)) {
                    return true;
                }
            }
        }
    }
    return false;
}