#include "SceneHierarchyWindowFactory.h"

namespace Dwarf
{
  SceneHierarchyWindowFactory::SceneHierarchyWindowFactory(
    std::shared_ptr<ILoadedScene>     loadedScene,
    std::shared_ptr<IEditorSelection> editorSelection,
    std::shared_ptr<IInputManager>    inputManager)
    : m_InjectorFactory(
        [&loadedScene, &editorSelection, &inputManager]()
        {
          return boost::di::make_injector(
            boost::di::bind<ILoadedScene>.to(loadedScene),
            boost::di::bind<IEditorSelection>.to(editorSelection),
            boost::di::bind<IInputManager>.to(inputManager));
        })
  {
  }

  std::unique_ptr<SceneHierarchyWindow>
  SceneHierarchyWindowFactory::Create() const
  {
    return m_InjectorFactory().create<std::unique_ptr<SceneHierarchyWindow>>();
  }

  std::unique_ptr<SceneHierarchyWindow>
  SceneHierarchyWindowFactory::Create(SerializedModule serializedModule) const
  {
    auto injector = boost::di::make_injector(
      m_InjectorFactory(),
      boost::di::bind<SerializedModule>.to(serializedModule));

    return injector.create<std::unique_ptr<SceneHierarchyWindow>>();
  }
} // namespace Dwarf