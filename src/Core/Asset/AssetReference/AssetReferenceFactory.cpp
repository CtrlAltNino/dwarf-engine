#include "AssetReferenceFactory.h"

namespace Dwarf
{
  template<typename T>
  std::unique_ptr<IAssetReference<T>>
  AssetReferenceFactory::Create(entt::entity                    assetHandle,
                                std::shared_ptr<entt::registry> registry)
  {
    return std::make_unique<IAssetReference<T>>(assetHandle, registry);
  }
}