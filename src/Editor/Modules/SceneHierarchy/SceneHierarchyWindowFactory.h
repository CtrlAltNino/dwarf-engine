#pragma once

#include "Editor/Modules/SceneHierarchy/ISceneHierarchyWindowFactory.h"
#include <boost/di.hpp>

namespace Dwarf
{
  class SceneHierarchyWindowFactory : public ISceneHierarchyWindowFactory
  {
  private:
    std::shared_ptr<ILoadedScene>     m_LoadedScene;
    std::shared_ptr<IEditorSelection> m_EditorSelection;
    std::shared_ptr<IInputManager>    m_InputManager;

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