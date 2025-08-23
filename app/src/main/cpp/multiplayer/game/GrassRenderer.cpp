//
// Created on 25.04.2024.
//

#include "GrassRenderer.h"
#include "util/patch.h"

bool CGrassRenderer::Initialise() {
    return CHook::CallFunction<bool>("_ZN14CGrassRenderer10InitialiseEv");
}
