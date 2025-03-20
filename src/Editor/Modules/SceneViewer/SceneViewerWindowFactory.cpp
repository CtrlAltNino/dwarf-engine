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
    : mCameraFactory(cameraFactory)
    , mFramebufferFactory(framebufferFactory)
    , mEditorStats(editorStats)
    , mInputManager(inputManager)
    , mLoadedScene(loadedScene)
    , mEditorSelection(editorSelection)
    , mRenderingPipelineFactory(renderingPipelineFactory)
    , mRendererApiFactory(rendererApiFactory)
    , mShaderRegistry(shaderRegistry)
    , mShaderSourceCollectionFactory(shaderSourceCollectionFactory)
  {
  }

  std::unique_ptr<SceneViewerWindow>
  SceneViewerWindowFactory::Create() const
  {
    return std::make_unique<SceneViewerWindow>(mCameraFactory,
                                               mFramebufferFactory,
                                               mEditorStats,
                                               mInputManager,
                                               mLoadedScene,
                                               mEditorSelection,
                                               mRenderingPipelineFactory,
                                               mRendererApiFactory,
                                               mShaderRegistry,
                                               mShaderSourceCollectionFactory);
  }

  std::unique_ptr<SceneViewerWindow>
  SceneViewerWindowFactory::Create(SerializedModule serializedModule) const
  {
    return std::make_unique<SceneViewerWindow>(serializedModule,
                                               mCameraFactory,
                                               mFramebufferFactory,
                                               mEditorStats,
                                               mInputManager,
                                               mLoadedScene,
                                               mEditorSelection,
                                               mRenderingPipelineFactory,
                                               mRendererApiFactory,
                                               mShaderRegistry,
                                               mShaderSourceCollectionFactory);
  }
} // namespace Dwarf