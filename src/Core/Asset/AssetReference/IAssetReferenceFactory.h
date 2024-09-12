#pragma once

#include "Core/Asset/AssetReference/IAssetReference.h"

namespace Dwarf
{
  class IAssetReferenceFactory
  {
  public:
    virtual ~IAssetReferenceFactory() = default;

    template<typename T>
    std::unique_ptr<IAssetReference<T>>
    Create(entt::entity assetHandle, const entt::registry& registry);
  };
}