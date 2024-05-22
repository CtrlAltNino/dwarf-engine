#include "Editor/EditorModel.h"

namespace Dwarf
{
  EditorModel::EditorModel(ProjectPath projectPath)
    : m_ProjectPath(projectPath)
  {
  }

  std::shared_ptr<Scene>
  EditorModel::GetScene() const
  {
    return m_Scene;
  }

  void
  EditorModel::SetScene(std::shared_ptr<Scene> scene)
  {
    m_Scene = scene;
    m_Selection = EditorSelection(scene);
  }

  std::string
  EditorModel::GetName() const
  {
    return m_ProjectName;
  }

  std::filesystem::path
  EditorModel::GetProjectPath() const
  {
    return m_ProjectPath;
  }

  void
  EditorModel::SetDeltaTime(double deltaTime)
  {
    m_DeltaTime = deltaTime;
  }

  double
  EditorModel::GetDeltaTime() const
  {
    return m_DeltaTime;
  }

  bool
  EditorModel::GetCloseSignal() const
  {
    return m_CloseSignal;
  }

  bool
  EditorModel::GetReturnToLauncher() const
  {
    return m_ReturnToLauncher;
  }

  EditorSelection&
  EditorModel::GetSelection()
  {
    return m_Selection;
  }

  std::vector<Entity>&
  EditorSelection::GetSelectedEntities()
  {
    return m_SelectedEntities;
  }

  const std::filesystem::path&
  EditorSelection::GetAssetPath() const
  {
    return m_SelectedAsset;
  }

  CURRENT_SELECTION_TYPE
  EditorSelection::GetSelectionType() const
  {
    return m_SelectionType;
  }

  void
  EditorModel::CloseEditor(bool returnToLauncher)
  {
    m_CloseSignal = true;
    m_ReturnToLauncher = returnToLauncher;
  }
}