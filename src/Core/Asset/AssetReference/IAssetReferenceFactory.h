#pragma once

#include "Core/Asset/AssetReference/IAssetReference.h"
#include "Core/Asset/AssetTypes.h"
#include "Core/Asset/Database/AssetComponents.h"

namespace Dwarf
{
  class IAssetReferenceFactory
  {
  public:
    virtual ~IAssetReferenceFactory() = default;

    virtual std::unique_ptr<IAssetReference>
    Create(entt::entity    assetHandle,
           entt::registry& registry,
           ASSET_TYPE      type) = 0;

    virtual std::unique_ptr<IAssetReference>
    CreateNew(entt::entity          assetHandle,
              entt::registry&       registry,
              const UUID&           uid,
              std::filesystem::path path,
              std::string           name) = 0;
  };
}