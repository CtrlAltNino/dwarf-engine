#pragma once

#include "Core/Base.h"

#include <string_view>

#include "Core/Scene/Scene.h"

namespace Dwarf {
  enum class CURRENT_SELECTION_TYPE
  {
    NONE,
    ASSET,
    ENTITY
  };

  class EditorSelection
  {
  private:
    Ref<Scene> m_Scene;
    CURRENT_SELECTION_TYPE m_SelectionType = CURRENT_SELECTION_TYPE::NONE;
    std::filesystem::path m_SelectedAsset;
    std::vector<Entity> m_SelectedEntities;

    /// @brief Returns the tree index of a given entity. Used for sorting based
    /// on their graph positions.
    /// @param entity Entity instance.
    /// @return The full tree index.
    std::string GetTreeIndex(Entity entity) const;

  public:
    EditorSelection() = default;
    EditorSelection(Ref<Scene> scene);

    void SelectEntity(Entity entity);

    void SelectAsset(std::filesystem::path assetPath);

    void AddEntityToSelection(Entity entity);

    void ClearEntitySelection();

    void ClearAssetSelection();

    void RemoveEntityFromSelection(Entity entity);

    bool IsEntitySelected(Entity entity);

    bool IsAssetSelected(std::filesystem::path assetPath);

    std::vector<Entity>& GetSelectedEntities();

    const std::filesystem::path& GetAssetPath() const;

    CURRENT_SELECTION_TYPE GetSelectionType() const;
  };
  /// @brief Model of the Editor's MVC structure.
  class EditorModel
  {
  private:
    /// @brief The currently opened scene instance.
    Ref<Scene> m_Scene;
    double m_DeltaTime;
    std::string m_ProjectName;
    std::filesystem::path m_ProjectPath;
    bool m_ReturnToLauncher = false;
    bool m_CloseSignal = false;
    EditorSelection m_Selection;

  public:
    EditorModel(std::string_view name, std::filesystem::path projectPath);

    /// @brief Returns the currently opened scene.
    /// @return A pointer to the scene.
    Ref<Scene> GetScene() const;

    void SetScene(Ref<Scene> scene);

    std::string GetName() const;

    std::filesystem::path GetProjectPath() const;

    void SetDeltaTime(double deltaTime);

    double GetDeltaTime() const;

    bool GetCloseSignal() const;

    bool GetReturnToLauncher() const;

    void CloseEditor(bool returnToLauncher);

    EditorSelection& GetSelection();
  };
}