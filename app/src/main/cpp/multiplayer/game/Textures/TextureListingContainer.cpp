//
// Created on 21.11.2023.
//

#include "TextureListingContainer.h"
#include "util/patch.h"

RwRaster *TextureListingContainer::CreateRaster(const TextureDatabaseEntry *forEntry) {
    return CHook::CallFunction<RwRaster*>(g_libGTASA + (VER_x32 ? 0x1E8CF8 + 1 : 0x2851DC), this, forEntry);
}
