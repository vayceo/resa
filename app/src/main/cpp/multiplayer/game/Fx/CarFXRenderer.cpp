//
// Created on 25.04.2024.
//

#include "CarFXRenderer.h"
#include "util/patch.h"
#include "game/Pipelines/CustomCar/CustomCarEnvMapPipeline.h"

bool CCarFXRenderer::Initialise() {
    return CCustomCarEnvMapPipeline::CreatePipe() != 0;
}

