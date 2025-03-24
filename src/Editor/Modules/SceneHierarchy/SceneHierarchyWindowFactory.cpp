#include "pch.h"

#include "SceneHierarchyWindowFactory.h"

namespace Dwarf
{
  SceneHierarchyWindowFactory::SceneHierarchyWindowFactory(
    std::shared_ptr<IDwarfLogger>     logger,
    std::shared_ptr<ILoadedScene>     loadedScene,
    std::shared_ptr<IEditorSelection> editorSelection,
    std::shared_ptr<IInputManager>    inputManager,
    std::shared_ptr<IAssetDatabase>   assetDatabase)
    : mLogger(std::move(logger))
    , mLoadedScene(std::move(loadedScene))
    , mEditorSelection(std::move(editorSelection))
    , mInputManager(std::move(inputManager))
    , mAssetDatabase(std::move(assetDatabase))
  {
    mLogger->LogDebug(Log("SceneHierarchyWindowFactory created",
                          "SceneHierarchyWindowFactory"));
  }

  SceneHierarchyWindowFactory::~SceneHierarchyWindowFactory()
  {
    mLogger->LogDebug(Log("SceneHierarchyWindowFactory destroyed",
                          "SceneHierarchyWindowFactory"));
  }

  auto
  SceneHierarchyWindowFactory::Create() const
    -> std::unique_ptr<SceneHierarchyWindow>
  {
    return std::make_unique<SceneHierarchyWindow>(
      mLogger, mLoadedScene, mEditorSelection, mInputManager, mAssetDatabase);
  }

  auto
  SceneHierarchyWindowFactory::Create(SerializedModule serializedModule) const
    -> std::unique_ptr<SceneHierarchyWindow>
  {
    return std::make_unique<SceneHierarchyWindow>(mLogger,
                                                  mLoadedScene,
                                                  mEditorSelection,
                                                  mInputManager,
                                                  mAssetDatabase,
                                                  serializedModule);
  }
} // namespace Dwarf