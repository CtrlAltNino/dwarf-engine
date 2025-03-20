#pragma once
#include "Core/Asset/AssetReference/IAssetReference.h"
#include "Core/Asset/Database/IAssetDatabase.h"
#include "Editor/LoadedScene/ILoadedScene.h"
#include "Editor/Selection/IEditorSelection.h"
#include <boost/di/extension/injections/lazy.hpp>

namespace Dwarf
{
  class EditorSelection : public IEditorSelection
  {
  private:
    std::shared_ptr<ILoadedScene> mLoadedScene;
    CURRENT_SELECTION_TYPE        mSelectionType = CURRENT_SELECTION_TYPE::NONE;
    std::unique_ptr<IAssetReference> mSelectedAsset;
    std::vector<entt::entity>        mSelectedEntities;

    /// @brief Returns the tree index of a given entity. Used for sorting based
    /// on their graph positions.
    /// @param entity Entity instance.
    /// @return The full tree index.
    std::string
    GetTreeIndex(const entt::entity& entity) const;

  public:
    EditorSelection(std::shared_ptr<ILoadedScene> loadedScene);

    void
    SelectEntity(const entt::entity& entity) override;

    void
    SelectAsset(std::unique_ptr<IAssetReference> assetPath) override;

    void
    AddEntityToSelection(const entt::entity& entity) override;

    void
    ClearEntitySelection() override;

    void
    ClearAssetSelection() override;

    void
    RemoveEntityFromSelection(const entt::entity& entity) override;

    bool
    IsEntitySelected(const entt::entity& entity) override;

    bool
    IsAssetSelected(const std::filesystem::path& assetPath) override;

    std::vector<entt::entity>&
    GetSelectedEntities() override;

    CURRENT_SELECTION_TYPE
    GetSelectionType() const override;

    IAssetReference&
    GetSelectedAsset() const override;
  };
}