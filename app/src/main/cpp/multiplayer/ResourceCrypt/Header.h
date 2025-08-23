#pragma once
#include <string>
typedef unsigned int uint32_t;

#define OBFUSCATE_KEY		0x12913AFB
#define OBFUSCATE_DATA(a)	((((a) << 19) | ((a) >> 13)) ^ OBFUSCATE_KEY)
#define UNOBFUSCATE_DATA(a) ((((a) ^ OBFUSCATE_KEY) >> 19) | (((a) ^ OBFUSCATE_KEY) << 13))

#define XOR_OBFUSCATE_KEY	0x10
#define XOR_OBFUSCATE(a) ((a) ^ XOR_OBFUSCATE_KEY)
#define XOR_UNOBFUSCATE(a) ((a) ^ XOR_OBFUSCATE_KEY)


const uint32_t g_i64Encrypt = OBFUSCATE_DATA(0x28FAAB82);


struct SEncryptedHeader
{
    uint32_t dwVersion;
    uint32_t dwFileSize;
    uint32_t dwChunksEncrypted;
    uint32_t img_header;
};

#define PART_SIZE		2048

#define ENCRYPTION_DELTA 0x11FA8275

const uint32_t g_iIdentifierOther = OBFUSCATE_DATA(0x4C121A90);
const uint32_t g_iIdentifierImg = OBFUSCATE_DATA(0x9C128A90);


const uint32_t g_iEncryptionKey[8] = {
        OBFUSCATE_DATA(0x1a902f89u), OBFUSCATE_DATA(0xF83BA92Fu), OBFUSCATE_DATA(0xE536DF24u), OBFUSCATE_DATA(0x2AA34E23u)
};