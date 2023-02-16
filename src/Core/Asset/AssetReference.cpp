#include "AssetReference.h"

namespace Dwarf {

    AssetReference::AssetReference(entt::entity assetHandle, EntityProvider* entProvider)
        : assetHandle(assetHandle), entProvider(entProvider){}
}