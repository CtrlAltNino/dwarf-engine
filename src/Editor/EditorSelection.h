#pragma once
#include "Core/Scene/Scene.h"

namespace Dwarf
{
  enum class CURRENT_SELECTION_TYPE
  {
    NONE,
    ASSET,
    ENTITY
  };

  class EditorSelection
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
    GetTreeIndex(Entity entity) const;

  public:
    EditorSelection() = default;
    EditorSelection(std::shared_ptr<Scene> scene);

    void
    SelectEntity(Entity entity);

    void
    SelectAsset(std::filesystem::path assetPath);

    void
    AddEntityToSelection(Entity entity);

    void
    ClearEntitySelection();

    void
    ClearAssetSelection();

    void
    RemoveEntityFromSelection(Entity entity);

    bool
    IsEntitySelected(Entity entity);

    bool
    IsAssetSelected(std::filesystem::path assetPath);

    std::vector<Entity>&
    GetSelectedEntities();

    const std::filesystem::path&
    GetAssetPath() const;

    CURRENT_SELECTION_TYPE
    GetSelectionType() const;
  };
}