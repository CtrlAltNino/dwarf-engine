#pragma once

#include "Editor/Modules/SceneViewer/ISceneViewerWindowFactory.h"
#include "Editor/Modules/SceneViewer/SceneViewerWindow.h"
#include <boost/di.hpp>

namespace Dwarf
{
  class SceneViewerWindowFactory : public ISceneViewerWindowFactory
  {
  private:
    std::shared_ptr<ICameraFactory>            m_CameraFactory;
    std::shared_ptr<IFramebufferFactory>       m_FramebufferFactory;
    std::shared_ptr<IEditorStats>              m_EditorStats;
    std::shared_ptr<IInputManager>             m_InputManager;
    std::shared_ptr<ILoadedScene>              m_LoadedScene;
    std::shared_ptr<IEditorSelection>          m_EditorSelection;
    std::shared_ptr<IRenderingPipelineFactory> m_RenderingPipelineFactory;
    std::shared_ptr<IRendererApiFactory>       m_RendererApiFactory;

  public:
    BOOST_DI_INJECT(
      SceneViewerWindowFactory,
      std::shared_ptr<ICameraFactory>            cameraFactory,
      std::shared_ptr<IFramebufferFactory>       framebufferFactory,
      std::shared_ptr<IEditorStats>              editorStats,
      std::shared_ptr<IInputManager>             inputManager,
      std::shared_ptr<ILoadedScene>              loadedScene,
      std::shared_ptr<IEditorSelection>          editorSelection,
      std::shared_ptr<IRenderingPipelineFactory> renderingPipelineFactory,
      std::shared_ptr<IRendererApiFactory>       rendererApiFactory);

    ~SceneViewerWindowFactory() override = default;

    std::unique_ptr<SceneViewerWindow>
    Create() const override;

    std::unique_ptr<SceneViewerWindow>
    Create(SerializedModule serializedModule) const override;
  };
} // namespace Dwarf