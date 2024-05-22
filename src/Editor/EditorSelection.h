#pragma once
#include "Editor/IEditorSelection.h"

namespace Dwarf
{
  class EditorSelection : public IEditorSelection
  {
  private:
    std::shared_ptr<Scene> m_Scene;
    CURRENT_SELECTION_TYPE m_SelectionType = CURRENT_SELECTION_TYPE::NONE;
    std::filesystem::path  m_SelectedAsset;
    std::vector<Entity>    m_SelectedEntities;

    /// @brief Returns the tree index of a given entity. Used for sorting based
    /// on their graph positions.
    /// @param entity Entity instance.
    /// @return The full tree index.
    std::string
    GetTreeIndex(const Entity& entity) const;

  public:
    EditorSelection() = default;
    EditorSelection(std::shared_ptr<Scene> scene);

    void
    SelectEntity(const Entity& entity) override;

    void
    SelectAsset(const std::filesystem::path& assetPath) override;

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

    const std::filesystem::path&
    GetAssetPath() const override;

    CURRENT_SELECTION_TYPE
    GetSelectionType() const override;
  };
}