#pragma once
#include "Core/Scene/IScene.h"
#include "Editor/IEditorSelection.h"
#include "Editor/IEditor.h"

namespace Dwarf
{
  class EditorSelection : public IEditorSelection
  {
  private:
    std::shared_ptr<IEditor> m_Editor;
    CURRENT_SELECTION_TYPE   m_SelectionType = CURRENT_SELECTION_TYPE::NONE;
    std::filesystem::path    m_SelectedAsset;
    std::vector<Entity>      m_SelectedEntities;

    /// @brief Returns the tree index of a given entity. Used for sorting based
    /// on their graph positions.
    /// @param entity Entity instance.
    /// @return The full tree index.
    std::string
    GetTreeIndex(const Entity& entity) const;

  public:
    EditorSelection(std::shared_ptr<IEditor> editor);

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