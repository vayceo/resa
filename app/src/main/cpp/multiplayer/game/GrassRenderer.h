//
// Created on 25.04.2024.
//

#pragma once

#include "common.h"


class CGrassRenderer {

public:
    static void InjectHooks();

    CGrassRenderer() = default;
    ~CGrassRenderer() = default;

    static bool Initialise();
    static void Shutdown();
};