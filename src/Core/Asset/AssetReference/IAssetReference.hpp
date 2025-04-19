#pragma once

#include "Core/Asset/AssetTypes.hpp"
#include "Core/Asset/Database/IAssetComponent.hpp"
#include "Core/UUID.hpp"
#include <entt/entity/fwd.hpp>
#include <entt/entt.hpp>

namespace Dwarf
{
  /**
   * @brief Virtual base class for efficient interfacing of the asset database
   * entries
   *
   */
  class IAssetReference
  {
  public:
    virtual ~IAssetReference() = default;

    /**
     * @brief Returns the entity handle
     *
     * @return The entt entity handle
     */
    virtual auto
    GetHandle() const -> entt::entity = 0;

    /**
     * @brief Returns the UUID identifier of the asset
     *
     * @return The uuid
     */
    [[nodiscard]] virtual auto
    GetUID() const -> const UUID& = 0;

    /**
     * @brief Returns the Path to the asset
     *
     * @return The asset path
     */
    [[nodiscard]] virtual auto
    GetPath() const -> const std::filesystem::path& = 0;

    /**
     * @brief Returns the component itself that is stores in the ECS
     *
     * @return Reference to the stored component
     */
    virtual auto
    GetAsset() -> IAssetComponent& = 0;

    /**
     * @brief Returns the type of the asset
     *
     * @return Asset type
     */
    [[nodiscard]] virtual auto
    GetType() const -> ASSET_TYPE = 0;

    /**
     * @brief Checks if this AssetReference is still valid
     *
     * @return true If it's valid
     * @return false If it's not present in the registry
     */
    [[nodiscard]] virtual auto
    IsValid() const -> bool = 0;
  };
}