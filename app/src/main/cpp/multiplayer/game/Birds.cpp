//
// Created on 23.09.2023.
//

#include "Birds.h"
#include "../util/patch.h"

void CBirds::Render() {
    CHook::CallFunction<void>(g_libGTASA + (VER_x32 ? 0x0059DA40 + 1 : 0x6C1D6C));
}

void CBirds::CreateNumberOfBirds(CVector vecStartPos, CVector vecTargetPos, int32 iBirdCount, eBirdsBiome biome, bool bCheckObstacles) {

}

void CBirds::Init() {
  //  CHook::InlineHook("_ZN6CBirds19CreateNumberOfBirdsE7CVectorS0_iib", &CBirds::CreateNumberOfBirds, &CreateNumberOfBirds_orig);
}
