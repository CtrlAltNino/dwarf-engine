#pragma once

#include "Editor/Modules/SceneHierarchy/ISceneHierarchyWindowFactory.h"
#include <boost/di.hpp>

namespace Dwarf
{
  class SceneHierarchyWindowFactory : public ISceneHierarchyWindowFactory
  {
  private:
    std::shared_ptr<ILoadedScene>     mLoadedScene;
    std::shared_ptr<IEditorSelection> mEditorSelection;
    std::shared_ptr<IInputManager>    mInputManager;
    std::shared_ptr<IAssetDatabase>   mAssetDatabase;

  public:
    BOOST_DI_INJECT(SceneHierarchyWindowFactory,
                    std::shared_ptr<ILoadedScene>     loadedScene,
                    std::shared_ptr<IEditorSelection> editorSelection,
                    std::shared_ptr<IInputManager>    inputManager,
                    std::shared_ptr<IAssetDatabase>   assetDatabase);
    ~SceneHierarchyWindowFactory() override = default;
    std::unique_ptr<SceneHierarchyWindow>
    Create() const override;

    std::unique_ptr<SceneHierarchyWindow>
    Create(SerializedModule serializedModule) const override;
  };
}