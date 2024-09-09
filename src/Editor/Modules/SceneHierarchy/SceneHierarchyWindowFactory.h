#pragma once

#include "Editor/Modules/SceneHierarchy/ISceneHierarchyWindowFactory.h"

namespace Dwarf
{
  class SceneHierarchyWindowFactory : public ISceneHierarchyWindowFactory
  {
  private:
    std::function<boost::di::injector<std::unique_ptr<SceneHierarchyWindow>>()>
      m_InjectorFactory;

  public:
    BOOST_DI_INJECT(SceneHierarchyWindowFactory,
                    std::shared_ptr<ILoadedScene>     loadedScene,
                    std::shared_ptr<IEditorSelection> editorSelection,
                    std::shared_ptr<IInputManager>    inputManager);
    ~SceneHierarchyWindowFactory() override = default;
    std::unique_ptr<SceneHierarchyWindow>
    Create() const override;

    std::unique_ptr<SceneHierarchyWindow>
    Create(SerializedModule serializedModule) const override;
  };
}