//
// Created on 28.04.2023.
//

#ifndef RUSSIA_MEMORYMGR_H
#define RUSSIA_MEMORYMGR_H


class CMemoryMgr {

public:
    static void Init();
    static void *Malloc(unsigned int size, unsigned int nHint);

    static void Free(void *memory);

    static void *Malloc(unsigned int size);

    static void FreeAlign(void *memory);

    static unsigned char *MallocAlign(unsigned int size, unsigned int align, unsigned int nHint);

    static void *MallocAlign(unsigned int size, unsigned int align);

    static void *MoveMemory(void *memory);
};


#endif //RUSSIA_MEMORYMGR_H
