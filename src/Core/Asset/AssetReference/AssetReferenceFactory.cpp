#include "AssetReferenceFactory.h"
#include "AssetReference.h"

namespace Dwarf
{
  template<typename T>
  IAssetReference<T>
  AssetReferenceFactory::Create(entt::entity          assetHandle,
                                const entt::registry& registry)
  {
    return AssetReference<T>(assetHandle, registry);
  }
}