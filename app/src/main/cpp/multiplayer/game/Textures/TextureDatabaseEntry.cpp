//
// Created on 21.11.2023.
//

#include "TextureDatabaseEntry.h"
#include "util/patch.h"

void TextureDatabaseEntry::InjectHooks() {
    //CHook::Redirect("_ZN20TextureDatabaseEntry12LoadInstanceERK23TextureListingContainer", &TextureDatabaseEntry__LoadInstance);
}
