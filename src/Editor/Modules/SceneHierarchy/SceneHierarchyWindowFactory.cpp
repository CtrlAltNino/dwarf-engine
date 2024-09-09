#include "SceneHierarchyWindowFactory.h"

namespace Dwarf
{
  SceneHierarchyWindowFactory::SceneHierarchyWindowFactory(
    std::shared_ptr<ILoadedScene>     loadedScene,
    std::shared_ptr<IEditorSelection> editorSelection,
    std::shared_ptr<IInputManager>    inputManager)
    : m_LoadedScene(loadedScene)
    , m_EditorSelection(editorSelection)
    , m_InputManager(inputManager)
  {
  }

  std::unique_ptr<SceneHierarchyWindow>
  SceneHierarchyWindowFactory::Create() const
  {
    return std::make_unique<SceneHierarchyWindow>(
      m_LoadedScene, m_EditorSelection, m_InputManager);
  }

  std::unique_ptr<SceneHierarchyWindow>
  SceneHierarchyWindowFactory::Create(SerializedModule serializedModule) const
  {
    return std::make_unique<SceneHierarchyWindow>(
      m_LoadedScene, m_EditorSelection, m_InputManager, serializedModule);
  }
} // namespace Dwarf