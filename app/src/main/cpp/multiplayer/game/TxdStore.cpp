//
// Created on 01.07.2023.
//

#include "TxdStore.h"
#include "../util/patch.h"
#include "constants.h"

int32 CTxdStore::GetNumRefs(int32 index){
    return CHook::CallFunction<int32>(g_libGTASA + (VER_x32 ? 0x5D3E34 + 1 : 0x6F8DF4), index);
}

void CTxdStore::RemoveTxd(int32 index) {
    CHook::CallFunction<void>(g_libGTASA + (VER_x32 ? 0x5D40B8 + 1 : 0x6F9130), index);
}

void CTxdStore::InjectHooks() {
    CHook::Write(g_libGTASA + (VER_x32 ? 0x679A64 : 0x8514E8), &ms_pTxdPool);
  //  CHook::Redirect(g_libGTASA, 0x0055BF14, &CTxdStore::GetNumRefs);
}

int32 CTxdStore::FindTxdSlot(const char *name) {
    return CHook::CallFunction<int32>(g_libGTASA + (VER_x32 ? 0x005D3EB0 + 1 : 0x6F8EA0), name);
}

int32 CTxdStore::FindTxdSlot(uint32 hash) {
    assert("NO x64 call");
}

int32 CTxdStore::AddTxdSlot(const char *name, const char *dbName, bool keepCPU) {
    return CHook::CallFunction<int32>(g_libGTASA + (VER_x32 ? 0x005D3B84 + 1 : 0x6F8A68), name, dbName, keepCPU);
}

// allocate new slot for this txd
int32 CTxdStore::AddTxdSlot(const char* name) {
    TxdDef* txd = ms_pTxdPool->New();
    txd->m_pRwDictionary = nullptr;
    txd->m_wRefsCount = 0;
    txd->m_wParentIndex = -1;
    txd->m_hash = CKeyGen::GetUppercaseKey(name);

    return ms_pTxdPool->GetIndex(txd);
}

RwTexture* CTxdStore::TxdStoreFindCB(const char* name) {
//    RwTexDictionary* txd = RwTexDictionaryGetCurrent();
//    for (; txd; txd = GetTxdParent(txd)) {
//        RwTexture* tex = RwTexDictionaryFindNamedTexture(txd, name);
//        if (tex)
//            return tex;
//    }
    return CHook::CallFunction<RwTexture*>(g_libGTASA + (VER_x32 ? 0x5D3C34 + 1 : 0x6F8B54), name);
}

RwTexture* CTxdStore::TxdStoreLoadCB(const char* name, const char* mask) {
    return nullptr;
}

void CTxdStore::Initialise() {
    //CHook::CallFunction<void>(g_libGTASA + (VER_x32 ? 0x005D3A90 + 1 : 0x6F8928));
    DLOG("CTxdStore::Initialise");

    int16 (&defaultTxds)[4] = *reinterpret_cast<int16 (*)[4]>(g_libGTASA + (VER_x32 ? 0xA83F60 : 0xD28608));

    if (!ms_pTxdPool)
        ms_pTxdPool = new CTxdPool(TOTAL_TXD_MODEL_IDS, "TexDictionary"); // need TOTAL_TXD_MODEL_IDS - samp 20000

    for (auto& txd : defaultTxds)
        txd = static_cast<int16>(AddTxdSlot("*", "player", 0));

    RwTextureSetFindCallBack(TxdStoreFindCB);
    RwTextureSetReadCallBack(TxdStoreLoadCB);
}

void CTxdStore::PushCurrentTxd() {

    CHook::CallFunction<void>(g_libGTASA + (VER_x32 ? 0x005D41D4 + 1 : 0x6F92A8));
}

void CTxdStore::PopCurrentTxd() {
    CHook::CallFunction<void>(g_libGTASA + (VER_x32 ? 0x005D4214 + 1 : 0x6F92D4));
}

void CTxdStore::SetCurrentTxd(int32 index) {
    CHook::CallFunction<void>(g_libGTASA + (VER_x32 ? 0x5D4144 + 1 : 0x6F91EC), index, nullptr);
}
