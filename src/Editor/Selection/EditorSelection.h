#pragma once

#include "Core/Asset/AssetReference/IAssetReference.h"
#include "Editor/LoadedScene/ILoadedScene.h"
#include "Editor/Selection/IEditorSelection.h"
#include "Logging/IDwarfLogger.h"
#include <boost/di/extension/injections/lazy.hpp>

namespace Dwarf
{
  class EditorSelection : public IEditorSelection
  {
  private:
    std::shared_ptr<IDwarfLogger> mLogger;
    std::shared_ptr<ILoadedScene> mLoadedScene;
    CURRENT_SELECTION_TYPE        mSelectionType = CURRENT_SELECTION_TYPE::NONE;
    std::unique_ptr<IAssetReference> mSelectedAsset;
    std::vector<entt::entity>        mSelectedEntities;

    /// @brief Returns the tree index of a given entity. Used for sorting based
    /// on their graph positions.
    /// @param entity Entity instance.
    /// @return The full tree index.
    [[nodiscard]] auto
    GetTreeIndex(const entt::entity& entity) const -> std::string;

  public:
    EditorSelection(std::shared_ptr<IDwarfLogger> logger,
                    std::shared_ptr<ILoadedScene> loadedScene);
    virtual ~EditorSelection() override;

    /**
     * @brief Selects an entity
     *
     * @param entity Entity to select
     */
    void
    SelectEntity(const entt::entity& entity) override;

    /**
     * @brief Selects an asset
     *
     * @param asset The selected asset
     */
    void
    SelectAsset(std::unique_ptr<IAssetReference> assetPath) override;

    /**
     * @brief Adds an entity to the selection
     *
     * @param entity Entity handle to add
     */
    void
    AddEntityToSelection(const entt::entity& entity) override;

    /**
     * @brief Clears the entity selection
     *
     */
    void
    ClearEntitySelection() override;

    /**
     * @brief Clears the asset selection
     *
     */
    void
    ClearAssetSelection() override;

    /**
     * @brief Removes an entity from the selection
     *
     * @param entity Entity handle to remove
     */
    void
    RemoveEntityFromSelection(const entt::entity& entity) override;

    /**
     * @brief Checks if an entity is among the selected entities
     *
     * @param entity Entity handle to check for
     * @return true If it is in the selection
     * @return false If it is not
     */
    auto
    IsEntitySelected(const entt::entity& entity) -> bool override;

    /**
     * @brief Checks if the asset at a given path is currently selected
     *
     * @param assetPath Path to an asset
     * @return true The asset is currently selected
     * @return false The asset is not selected
     */
    auto
    IsAssetSelected(const std::filesystem::path& assetPath) -> bool override;

    /**
     * @brief Returns a list of the selected scene entities
     *
     * @return Reference to the list of selected scene entities
     */
    auto
    GetSelectedEntities() -> std::vector<entt::entity>& override;

    /**
     * @brief Returns the currently selected asset
     *
     * @return Reference to the selected asset
     */
    [[nodiscard]] auto
    GetSelectionType() const -> CURRENT_SELECTION_TYPE override;

    /**
     * @brief Returns the type of the current selection
     *
     * @return Current selection type
     */
    [[nodiscard]] auto
    GetSelectedAsset() -> std::unique_ptr<IAssetReference>& override;
  };
}