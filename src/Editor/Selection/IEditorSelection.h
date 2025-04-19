#pragma once

#include "Core/Asset/AssetReference/IAssetReference.h"
#include <entt/entt.hpp>

namespace Dwarf
{
  /**
   * @brief Enum representing a selection state
   *
   */
  enum class CURRENT_SELECTION_TYPE : uint8_t
  {
    NONE,
    ASSET,
    ENTITY
  };

  /**
   * @brief Class storing selection data of the editor to present them in the
   * inspector. Scene entities and assets can be selected simultaniously.
   *
   */
  class IEditorSelection
  {
  public:
    virtual ~IEditorSelection() = default;

    /**
     * @brief Selects an entity
     *
     * @param entity Entity to select
     */
    virtual void
    SelectEntity(const entt::entity& entity) = 0;

    /**
     * @brief Selects an asset
     *
     * @param asset The selected asset
     */
    virtual void
    SelectAsset(std::unique_ptr<IAssetReference> asset) = 0;

    /**
     * @brief Adds an entity to the selection
     *
     * @param entity Entity handle to add
     */
    virtual void
    AddEntityToSelection(const entt::entity& entity) = 0;

    /**
     * @brief Clears the entity selection
     *
     */
    virtual void
    ClearEntitySelection() = 0;

    /**
     * @brief Clears the asset selection
     *
     */
    virtual void
    ClearAssetSelection() = 0;

    /**
     * @brief Removes an entity from the selection
     *
     * @param entity Entity handle to remove
     */
    virtual void
    RemoveEntityFromSelection(const entt::entity& entity) = 0;

    /**
     * @brief Checks if an entity is among the selected entities
     *
     * @param entity Entity handle to check for
     * @return true If it is in the selection
     * @return false If it is not
     */
    virtual auto
    IsEntitySelected(const entt::entity& entity) -> bool = 0;

    /**
     * @brief Checks if the asset at a given path is currently selected
     *
     * @param assetPath Path to an asset
     * @return true The asset is currently selected
     * @return false The asset is not selected
     */
    virtual auto
    IsAssetSelected(const std::filesystem::path& assetPath) -> bool = 0;

    /**
     * @brief Returns a list of the selected scene entities
     *
     * @return Reference to the list of selected scene entities
     */
    virtual auto
    GetSelectedEntities() -> std::vector<entt::entity>& = 0;

    /**
     * @brief Returns the currently selected asset
     *
     * @return Reference to the selected asset
     */
    [[nodiscard]] virtual auto
    GetSelectedAsset() -> std::unique_ptr<IAssetReference>& = 0;

    /**
     * @brief Returns the type of the current selection
     *
     * @return Current selection type
     */
    [[nodiscard]] virtual auto
    GetSelectionType() const -> CURRENT_SELECTION_TYPE = 0;
  };
}