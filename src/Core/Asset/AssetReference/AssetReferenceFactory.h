#pragma once

#include "Core/Asset/AssetReference/IAssetReferenceFactory.h"

namespace Dwarf
{
  class AssetReferenceFactory : public IAssetReferenceFactory
  {
  public:
    ~AssetReferenceFactory() override = default;

    template<typename T>
    IAssetReference<T>
    Create(entt::entity assetHandle, const entt::registry& registry);
  };
}