#include "SceneHierarchyWindowFactory.h"

namespace Dwarf
{
  SceneHierarchyWindowFactory::SceneHierarchyWindowFactory(
    std::shared_ptr<ILoadedScene>     loadedScene,
    std::shared_ptr<IEditorSelection> editorSelection,
    std::shared_ptr<IInputManager>    inputManager,
    std::shared_ptr<IAssetDatabase>   assetDatabase)
    : m_LoadedScene(loadedScene)
    , m_EditorSelection(editorSelection)
    , m_InputManager(inputManager)
    , m_AssetDatabase(assetDatabase)
  {
  }

  std::unique_ptr<SceneHierarchyWindow>
  SceneHierarchyWindowFactory::Create() const
  {
    return std::make_unique<SceneHierarchyWindow>(
      m_LoadedScene, m_EditorSelection, m_InputManager, m_AssetDatabase);
  }

  std::unique_ptr<SceneHierarchyWindow>
  SceneHierarchyWindowFactory::Create(SerializedModule serializedModule) const
  {
    return std::make_unique<SceneHierarchyWindow>(m_LoadedScene,
                                                  m_EditorSelection,
                                                  m_InputManager,
                                                  m_AssetDatabase,
                                                  serializedModule);
  }
} // namespace Dwarf