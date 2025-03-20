#pragma once

#include "Core/Asset/AssetReference/IAssetReference.h"
#include "Core/Asset/AssetTypes.h"

namespace Dwarf
{
  class IAssetReferenceFactory
  {
  public:
    virtual ~IAssetReferenceFactory() = default;

    /**
     * @brief Creates an asset reference for an existing asset
     *
     * @param assetHandle Handle of the asset entity
     * @param registry The ECS registry of the asset database
     * @param type The known type of the underlying asset
     * @return A unique pointer to the asset reference
     */
    virtual auto
    Create(entt::entity assetHandle, entt::registry& registry, ASSET_TYPE type)
      -> std::unique_ptr<IAssetReference> = 0;

    /**
     * @brief Creates an asset reference and the underlying asset
     *
     * @param assetHandle Handle of the asset entity
     * @param registry The ECS registry of the asset database
     * @param uid The UUID to emplace
     * @param path The path to the asset
     * @param name The name of the asset
     * @return A unique pointer to the asset reference
     */
    virtual auto
    CreateNew(entt::entity          assetHandle,
              entt::registry&       registry,
              const UUID&           uid,
              std::filesystem::path path,
              std::string name) -> std::unique_ptr<IAssetReference> = 0;
  };
}