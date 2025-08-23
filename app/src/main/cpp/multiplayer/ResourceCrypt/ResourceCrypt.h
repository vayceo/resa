//
// Created by traw-GG on 11.07.2025.
//

#pragma once

#include "game/common.h"
#include "Header.h"

class CResourceCrypt {
public:
    static bool IsEncryptedFile(uint8* pFirstChunk);
    static bool IsEcryptedImg(uint8_t* pFirstChunk);

    static void GetImgHeader(char* pBuffer);
    static void DecryptStream(char *pStream);
};