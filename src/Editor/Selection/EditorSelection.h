#pragma once
#include "Core/Asset/AssetReference/IAssetReference.h"
#include "Core/Asset/Database/IAssetDatabase.h"
#include "Editor/Selection/IEditorSelection.h"
#include "Editor/LoadedScene/ILoadedScene.h"
#include <boost/di/extension/injections/lazy.hpp>

namespace Dwarf
{
  class EditorSelection : public IEditorSelection
  {
  private:
    std::shared_ptr<ILoadedScene> m_LoadedScene;
    CURRENT_SELECTION_TYPE m_SelectionType = CURRENT_SELECTION_TYPE::NONE;
    std::unique_ptr<IAssetReference> m_SelectedAsset;
    std::vector<Entity>              m_SelectedEntities;
    // boost::di::extension::lazy<std::shared_ptr<IAssetDatabase>>
    // m_AssetDatabase;

    /// @brief Returns the tree index of a given entity. Used for sorting based
    /// on their graph positions.
    /// @param entity Entity instance.
    /// @return The full tree index.
    std::string
    GetTreeIndex(const Entity& entity) const;

  public:
    EditorSelection(std::shared_ptr<ILoadedScene> loadedScene,
                    boost::di::extension::lazy<std::shared_ptr<IAssetDatabase>>
                      assetDatabase);

    void
    SelectEntity(const Entity& entity) override;

    void
    SelectAsset(std::unique_ptr<IAssetReference> assetPath) override;

    void
    AddEntityToSelection(const Entity& entity) override;

    void
    ClearEntitySelection() override;

    void
    ClearAssetSelection() override;

    void
    RemoveEntityFromSelection(const Entity& entity) override;

    bool
    IsEntitySelected(const Entity& entity) override;

    bool
    IsAssetSelected(const std::filesystem::path& assetPath) override;

    std::vector<Entity>&
    GetSelectedEntities() override;

    CURRENT_SELECTION_TYPE
    GetSelectionType() const override;

    IAssetReference&
    GetSelectedAsset() const override;
  };
}