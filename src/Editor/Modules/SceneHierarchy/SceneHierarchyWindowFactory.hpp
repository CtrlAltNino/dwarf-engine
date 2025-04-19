#pragma once

#include "Editor/Modules/SceneHierarchy/ISceneHierarchyWindowFactory.hpp"
#include "Logging/IDwarfLogger.hpp"

namespace Dwarf
{
  /**
   * @brief Factory class to create SceneHierarchyWindow instances
   *
   */
  class SceneHierarchyWindowFactory : public ISceneHierarchyWindowFactory
  {
  private:
    std::shared_ptr<IDwarfLogger>     mLogger;
    std::shared_ptr<ILoadedScene>     mLoadedScene;
    std::shared_ptr<IEditorSelection> mEditorSelection;
    std::shared_ptr<IInputManager>    mInputManager;
    std::shared_ptr<IAssetDatabase>   mAssetDatabase;

  public:
    SceneHierarchyWindowFactory(
      std::shared_ptr<IDwarfLogger>     logger,
      std::shared_ptr<ILoadedScene>     loadedScene,
      std::shared_ptr<IEditorSelection> editorSelection,
      std::shared_ptr<IInputManager>    inputManager,
      std::shared_ptr<IAssetDatabase>   assetDatabase);

    ~SceneHierarchyWindowFactory() override;

    /**
     * @brief Creates default SceneHierarchyWindow instances
     *
     * @return Unique pointer to the created instance
     */
    [[nodiscard]] auto
    Create() const -> std::unique_ptr<SceneHierarchyWindow> override;

    /**
     * @brief Creates a SceneHierarchyWindow instance based on a serialized
     * version
     *
     * @param serializedModule Serialized SceneHierarchyWindow data
     * @return Unique pointer to the created instance
     */
    [[nodiscard]] auto
    Create(SerializedModule serializedModule) const
      -> std::unique_ptr<SceneHierarchyWindow> override;
  };
}