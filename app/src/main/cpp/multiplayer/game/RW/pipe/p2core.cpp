//
// Created on 02.05.2024.
//

#include "p2core.h"
#include "util/patch.h"

RxPipeline* RxPipelineCreate() {
    return CHook::CallFunction<RxPipeline*>("_Z16RxPipelineCreatev");
}