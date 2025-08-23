//
// Created on 27.04.2023.
//
#include "BaseModelInfo.h"
#include "game/TxdStore.h"
#include "util/patch.h"

void CBaseModelInfo::SetTexDictionary(const char* txdName, const char *dbName) {
    m_nTxdIndex = CTxdStore::FindOrAddTxdSlot(txdName, dbName);
}


int32 CBaseModelInfo::GetAnimFileIndex() {
    return CHook::CallVTableFunctionByNum<int32>(this, 15);
}

void CBaseModelInfo::DeleteRwObject() {
    CHook::CallVTableFunctionByNum<int32>(this, 9);
}

CAtomicModelInfo *CBaseModelInfo::AsAtomicModelInfoPtr() {
    return nullptr;
}

CDamageAtomicModelInfo *CBaseModelInfo::AsDamageAtomicModelInfoPtr() {
    return nullptr;
}

CLodAtomicModelInfo *CBaseModelInfo::AsLodAtomicModelInfoPtr() {
    return nullptr;
}
