#include "AssetReferenceFactory.h"
#include "AssetReference.h"

namespace Dwarf
{
  template<typename T>
  std::unique_ptr<IAssetReference<T>>
  AssetReferenceFactory::Create(entt::entity          assetHandle,
                                const entt::registry& registry)
  {
    return std::make_unique<AssetReference<T>>(assetHandle, registry);
  }
}