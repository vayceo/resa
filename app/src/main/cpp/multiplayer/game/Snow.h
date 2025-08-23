//
// Created on 23.11.2023.
//

#pragma once

#include "common.h"

class CSnow
{
    static inline uint32 m_dwParticeHandle1;
    static inline uint32 m_dwParticeHandle2;
    static inline uint32 m_dwLastTickCreated;
    static inline int m_iCurrentSnow = 1;
public:
    static void Initialise();
    static void Process();
    static void SetCurrentSnow(int iSnow);
    static int GetCurrentSnow();
};

