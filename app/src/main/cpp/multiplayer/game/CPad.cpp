//
// Created on 14.03.2024.
//

#include "CPad.h"
#include "util/patch.h"

void CPad::Initialise() {
    CHook::CallFunction<void>("_ZN4CPad10InitialiseEv");
}
