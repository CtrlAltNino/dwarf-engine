#include "dpch.h"

#include "Editor/EditorModel.h"

#include <format>

namespace Dwarf {
  EditorModel::EditorModel(std::string_view name,
                           std::filesystem::path projectPath)
    : m_ProjectName(name)
    , m_ProjectPath(projectPath)
  {
  }

  Ref<Scene> EditorModel::GetScene() const
  {
    return m_Scene;
  }

  void EditorModel::SetScene(Ref<Scene> scene)
  {
    m_Scene = scene;
    m_Selection = EditorSelection(scene);
  }

  std::string EditorModel::GetName() const
  {
    return m_ProjectName;
  }

  std::filesystem::path EditorModel::GetProjectPath() const
  {
    return m_ProjectPath;
  }

  void EditorModel::SetDeltaTime(double deltaTime)
  {
    m_DeltaTime = deltaTime;
  }

  double EditorModel::GetDeltaTime() const
  {
    return m_DeltaTime;
  }

  bool EditorModel::GetCloseSignal() const
  {
    return m_CloseSignal;
  }

  bool EditorModel::GetReturnToLauncher() const
  {
    return m_ReturnToLauncher;
  }

  EditorSelection& EditorModel::GetSelection()
  {
    return m_Selection;
  }

  std::vector<Entity>& EditorSelection::GetSelectedEntities()
  {
    return m_SelectedEntities;
  }

  const std::filesystem::path& EditorSelection::GetAssetPath() const
  {
    return m_SelectedAsset;
  }

  CURRENT_SELECTION_TYPE EditorSelection::GetSelectionType() const
  {
    return m_SelectionType;
  }

  void EditorModel::CloseEditor(bool returnToLauncher)
  {
    m_CloseSignal = true;
    m_ReturnToLauncher = returnToLauncher;
  }

  EditorSelection::EditorSelection(Ref<Scene> scene)
    : m_Scene(scene)
  {
  }

  void EditorSelection::SelectEntity(Entity entity)
  {
    ClearEntitySelection();
    AddEntityToSelection(entity);
    m_SelectionType = CURRENT_SELECTION_TYPE::ENTITY;
  }

  void EditorSelection::SelectAsset(std::filesystem::path assetPath)
  {
    m_SelectedAsset = assetPath;
    m_SelectionType = CURRENT_SELECTION_TYPE::ASSET;
  }

  void EditorSelection::AddEntityToSelection(Entity entity)
  {
    std::string index = GetTreeIndex(entity);

    auto cursor = m_SelectedEntities.begin();
    std::string cursorIndex;

    while ((cursor != m_SelectedEntities.end()) &&
           ((cursorIndex = GetTreeIndex(*cursor)) < index)) {
      cursor++;
    }

    if (cursor == m_SelectedEntities.end()) {
      m_SelectedEntities.push_back(entity);
    } else {
      m_SelectedEntities.insert(cursor, entity);
    }

    m_SelectionType = CURRENT_SELECTION_TYPE::ENTITY;
  }

  void EditorSelection::ClearEntitySelection()
  {
    m_SelectedEntities.clear();
    m_SelectionType = m_SelectedAsset.empty() ? CURRENT_SELECTION_TYPE::NONE
                                              : CURRENT_SELECTION_TYPE::ASSET;
  }

  void EditorSelection::ClearAssetSelection()
  {
    m_SelectedAsset.clear();
    m_SelectionType = m_SelectedEntities.empty()
                        ? CURRENT_SELECTION_TYPE::NONE
                        : CURRENT_SELECTION_TYPE::ENTITY;
  }

  void EditorSelection::RemoveEntityFromSelection(Entity entity)
  {
    m_SelectedEntities.erase(
      std::find(m_SelectedEntities.begin(), m_SelectedEntities.end(), entity));
    if (m_SelectedEntities.empty()) {
      m_SelectionType = m_SelectedAsset.empty() ? CURRENT_SELECTION_TYPE::NONE
                                                : CURRENT_SELECTION_TYPE::ASSET;
    }
  }

  bool EditorSelection::IsEntitySelected(Entity entity)
  {
    return std::find(m_SelectedEntities.begin(),
                     m_SelectedEntities.end(),
                     entity) != m_SelectedEntities.end();
  }

  bool EditorSelection::IsAssetSelected(std::filesystem::path assetPath)
  {
    return m_SelectedAsset == assetPath;
  }

  std::string EditorSelection::GetTreeIndex(Entity entity) const
  {
    std::string index = "";
    Entity cursor = entity;

    while (cursor.GetHandle() != m_Scene->GetRootEntity()->GetHandle()) {
      index =
        std::format("{}{}", std::to_string(cursor.GetChildIndex()), index);
      cursor = Entity(cursor.GetParent(), m_Scene->GetRegistry());
    }

    return index;
  }
}