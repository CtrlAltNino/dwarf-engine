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
    std::shared_ptr<IRenderingPipelineFactory> renderingPipelineFactory,
    std::shared_ptr<IRendererApiFactory>       rendererApiFactory,
    std::shared_ptr<IShaderRegistry>           shaderRegistry,
    std::shared_ptr<IShaderSourceCollectionFactory>
      shaderSourceCollectionFactory)
    : m_CameraFactory(cameraFactory)
    , m_FramebufferFactory(framebufferFactory)
    , m_EditorStats(editorStats)
    , m_InputManager(inputManager)
    , m_LoadedScene(loadedScene)
    , m_EditorSelection(editorSelection)
    , m_RenderingPipelineFactory(renderingPipelineFactory)
    , m_RendererApiFactory(rendererApiFactory)
    , m_ShaderRegistry(shaderRegistry)
    , m_ShaderSourceCollectionFactory(shaderSourceCollectionFactory)
  {
  }

  std::unique_ptr<SceneViewerWindow>
  SceneViewerWindowFactory::Create() const
  {
    return std::make_unique<SceneViewerWindow>(m_CameraFactory,
                                               m_FramebufferFactory,
                                               m_EditorStats,
                                               m_InputManager,
                                               m_LoadedScene,
                                               m_EditorSelection,
                                               m_RenderingPipelineFactory,
                                               m_RendererApiFactory,
                                               m_ShaderRegistry,
                                               m_ShaderSourceCollectionFactory);
  }

  std::unique_ptr<SceneViewerWindow>
  SceneViewerWindowFactory::Create(SerializedModule serializedModule) const
  {
    return std::make_unique<SceneViewerWindow>(serializedModule,
                                               m_CameraFactory,
                                               m_FramebufferFactory,
                                               m_EditorStats,
                                               m_InputManager,
                                               m_LoadedScene,
                                               m_EditorSelection,
                                               m_RenderingPipelineFactory,
                                               m_RendererApiFactory,
                                               m_ShaderRegistry,
                                               m_ShaderSourceCollectionFactory);
  }
} // namespace Dwarf