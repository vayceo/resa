//
// Created by traw-GG on 11.07.2025.
//

#include "ResourceCrypt.h"
#include "ResourceCrypt/aes/CTEA.h"
#include "main.h"

bool CResourceCrypt::IsEcryptedImg(uint8_t* pFirstChunk)
{
    auto pHeader = reinterpret_cast<SEncryptedHeader*>(pFirstChunk);
    if(!pHeader)
        return false;

    if (pHeader->dwVersion == UNOBFUSCATE_DATA(g_iIdentifierImg))
        return true;

    return false;
}

bool CResourceCrypt::IsEncryptedFile(uint8_t* pFirstChunk)
{
    auto pHeader = reinterpret_cast<SEncryptedHeader*>(pFirstChunk);
    if(!pHeader)
        return false;

    if (pHeader->dwVersion == UNOBFUSCATE_DATA(g_iIdentifierImg))
        return true;

    if (pHeader->dwVersion == UNOBFUSCATE_DATA(g_iIdentifierOther))
        return true;

    return false;
}

#pragma optimize("", off)
void CResourceCrypt::DecryptStream(char* pBuffer)
{
	auto pHeader = (SEncryptedHeader*)pBuffer;
	auto fileSize = pHeader->dwFileSize;

	memcpy(pBuffer, pBuffer + sizeof(SEncryptedHeader), fileSize - sizeof(SEncryptedHeader));

	CTEA encr{};
	encr.SetKey(&g_iEncryptionKey[0]);

	for (uint32_t i = 0; i < fileSize / PART_SIZE; i++)
	{
		encr.DecryptData(pBuffer, PART_SIZE, 8);
		pBuffer += PART_SIZE;
	}
	uint32_t remainingBytes = fileSize % PART_SIZE;
	if (remainingBytes > 0)
	{
		encr.DecryptData(pBuffer, remainingBytes, 8);
		pBuffer += remainingBytes;
	}

	memset(pBuffer, '\n', sizeof(SEncryptedHeader));
}

void CResourceCrypt::GetImgHeader(char* pBuffer)
{
    auto* pHeader = (SEncryptedHeader*)pBuffer;
  //  Log("img head = %s", pHeader->img_header);
//    auto fileSize = pHeader->dwFileSize;
//    auto numChunks = pHeader->dwChunksEncrypted;

    *(int*)pBuffer = pHeader->img_header;
   // memcpy(pBuffer, pHeader->img_header, 4);
 //   buffer = pHeader->img_header;
//    memmove(pBuffer, pBuffer + sizeof(SEncryptedHeader), fileSize - sizeof(SEncryptedHeader));
//
//    auto started = pBuffer;
//    CTEA encr;
//    encr.SetKey(&g_iEncryptionKey[0]);
//
//    encr.DecryptData(pBuffer, 4, 8);
//
//    pBuffer = started;
}

#pragma optimize("", on)


