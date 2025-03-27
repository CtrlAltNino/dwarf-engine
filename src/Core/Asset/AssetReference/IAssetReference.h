#pragma once

#include "Core/Asset/AssetTypes.h"
#include "Core/Asset/Database/IAssetComponent.h"
#include "Core/UUID.h"
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
    virtual auto
    GetUID() const -> const UUID& = 0;

    /**
     * @brief Returns the Path to the asset
     *
     * @return The asset path
     */
    virtual auto
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
    virtual auto
    GetType() const -> ASSET_TYPE = 0;
  };
}