#include "AssetReference.h"

AssetReference::AssetReference(entt::entity assetHandle, EntityProvider* entProvider)
    : assetHandle(assetHandle),
        entProvider(entProvider){}