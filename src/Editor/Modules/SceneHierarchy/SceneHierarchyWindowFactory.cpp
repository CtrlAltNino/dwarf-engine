#include "SceneHierarchyWindowFactory.h"

namespace Dwarf
{
  SceneHierarchyWindowFactory::SceneHierarchyWindowFactory(
    std::shared_ptr<ILoadedScene>     loadedScene,
    std::shared_ptr<IEditorSelection> editorSelection,
    std::shared_ptr<IInputManager>    inputManager,
    std::shared_ptr<IAssetDatabase>   assetDatabase)
    : mLoadedScene(loadedScene)
    , mEditorSelection(editorSelection)
    , mInputManager(inputManager)
    , mAssetDatabase(assetDatabase)
  {
  }

  std::unique_ptr<SceneHierarchyWindow>
  SceneHierarchyWindowFactory::Create() const
  {
    return std::make_unique<SceneHierarchyWindow>(
      mLoadedScene, mEditorSelection, mInputManager, mAssetDatabase);
  }

  std::unique_ptr<SceneHierarchyWindow>
  SceneHierarchyWindowFactory::Create(SerializedModule serializedModule) const
  {
    return std::make_unique<SceneHierarchyWindow>(mLoadedScene,
                                                  mEditorSelection,
                                                  mInputManager,
                                                  mAssetDatabase,
                                                  serializedModule);
  }
} // namespace Dwarf