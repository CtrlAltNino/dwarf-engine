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

  std::shared_ptr<SceneHierarchyWindow>
  SceneHierarchyWindowFactory::Create() const
  {
    return std::make_shared<SceneHierarchyWindow>(
      m_InjectorFactory().create<SceneHierarchyWindow>());
  }

  std::shared_ptr<SceneHierarchyWindow>
  SceneHierarchyWindowFactory::Create(SerializedModule serializedModule) const
  {
    auto injector = boost::di::make_injector(
      m_InjectorFactory(),
      boost::di::bind<SerializedModule>.to(serializedModule));

    return std::make_shared<SceneHierarchyWindow>(
      injector.create<SceneHierarchyWindow>());
  }
} // namespace Dwarf