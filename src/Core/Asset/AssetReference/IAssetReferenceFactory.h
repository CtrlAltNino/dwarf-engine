#pragma once

#include "Core/Asset/AssetReference/IAssetReference.h"
#include "Core/Asset/AssetTypes.h"

namespace Dwarf
{
  class IAssetReferenceFactory
  {
  public:
    virtual ~IAssetReferenceFactory() = default;

    virtual auto
    Create(entt::entity assetHandle, entt::registry& registry, ASSET_TYPE type)
      -> std::unique_ptr<IAssetReference> = 0;

    virtual auto
    CreateNew(entt::entity          assetHandle,
              entt::registry&       registry,
              const UUID&           uid,
              std::filesystem::path path,
              std::string name) -> std::unique_ptr<IAssetReference> = 0;
  };
}