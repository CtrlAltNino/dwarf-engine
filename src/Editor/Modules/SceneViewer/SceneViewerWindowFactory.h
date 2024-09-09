#pragma once

#include "Editor/Modules/SceneViewer/ISceneViewerWindowFactory.h"
#include "Editor/Modules/SceneViewer/SceneViewerWindow.h"
#include <boost/di.hpp>

namespace Dwarf
{
  class SceneViewerWindowFactory : public ISceneViewerWindowFactory
  {
  private:
    std::function<boost::di::injector<std::unique_ptr<SceneViewerWindow>>()>
      m_InjectorFactory;

  public:
    BOOST_DI_INJECT(
      SceneViewerWindowFactory,
      std::shared_ptr<ICameraFactory>            cameraFactory,
      std::shared_ptr<IFramebufferFactory>       framebufferFactory,
      std::shared_ptr<IEditorStats>              editorStats,
      std::shared_ptr<IInputManager>             inputManager,
      std::shared_ptr<ILoadedScene>              loadedScene,
      std::shared_ptr<IEditorSelection>          editorSelection,
      std::shared_ptr<IRenderingPipelineFactory> renderingPipelineFactory);
    ~SceneViewerWindowFactory() override = default;
    std::unique_ptr<SceneViewerWindow>
    Create() const override;

    std::unique_ptr<SceneViewerWindow>
    Create(SerializedModule serializedModule) const override;
  };
} // namespace Dwarf