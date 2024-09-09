#include "SceneViewerWindowFactory.h"

namespace Dwarf
{
  SceneViewerWindowFactory::SceneViewerWindowFactory(
    std::shared_ptr<ICameraFactory>            cameraFactory,
    std::shared_ptr<IFramebufferFactory>       framebufferFactory,
    std::shared_ptr<IEditorStats>              editorStats,
    std::shared_ptr<IInputManager>             inputManager,
    std::shared_ptr<ILoadedScene>              loadedScene,
    std::shared_ptr<IEditorSelection>          editorSelection,
    std::shared_ptr<IRenderingPipelineFactory> renderingPipelineFactory)
    : m_InjectorFactory(
        [&cameraFactory,
         &framebufferFactory,
         &editorStats,
         &inputManager,
         &loadedScene,
         &editorSelection,
         &renderingPipelineFactory]()
        {
          return boost::di::make_injector(
            boost::di::bind<ICameraFactory>.to(cameraFactory),
            boost::di::bind<IFramebufferFactory>.to(framebufferFactory),
            boost::di::bind<IEditorStats>.to(editorStats),
            boost::di::bind<IInputManager>.to(inputManager),
            boost::di::bind<ILoadedScene>.to(loadedScene),
            boost::di::bind<IEditorSelection>.to(editorSelection),
            boost::di::bind<IRenderingPipelineFactory>.to(
              renderingPipelineFactory));
        })
  {
  }

  std::unique_ptr<SceneViewerWindow>
  SceneViewerWindowFactory::Create() const
  {
    return m_InjectorFactory().create<std::unique_ptr<SceneViewerWindow>>();
  }

  std::unique_ptr<SceneViewerWindow>
  SceneViewerWindowFactory::Create(SerializedModule serializedModule) const
  {
    auto injector = boost::di::make_injector(
      m_InjectorFactory(),
      boost::di::bind<SerializedModule>.to(serializedModule));

    return injector.create<std::unique_ptr<SceneViewerWindow>>();
  }
} // namespace Dwarf