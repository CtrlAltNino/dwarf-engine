#pragma once

#include "Editor/Modules/SceneHierarchy/ISceneHierarchyWindowFactory.h"

namespace Dwarf
{
  class SceneHierarchyWindowFactory : public ISceneHierarchyWindowFactory
  {
  private:
    std::function<boost::di::injector<SceneHierarchyWindow>()>
      m_InjectorFactory;

  public:
    BOOST_DI_INJECT(SceneHierarchyWindowFactory,
                    std::shared_ptr<ILoadedScene>     loadedScene,
                    std::shared_ptr<IEditorSelection> editorSelection,
                    std::shared_ptr<IInputManager>    inputManager);
    ~SceneHierarchyWindowFactory() override = default;
    std::shared_ptr<SceneHierarchyWindow>
    Create() const override;

    std::shared_ptr<SceneHierarchyWindow>
    Create(SerializedModule serializedModule) const override;
  };
}