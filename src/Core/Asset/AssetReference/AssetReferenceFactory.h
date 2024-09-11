#pragma once

#include "Core/Asset/AssetReference/IAssetReferenceFactory.h"

namespace Dwarf
{
  class AssetReferenceFactory : public IAssetReferenceFactory
  {
  public:
    ~AssetReferenceFactory() override = default;

    template<typename T>
    std::unique_ptr<IAssetReference<T>>
    Create(entt::entity assetHandle, std::shared_ptr<entt::registry> registry);
  };
}