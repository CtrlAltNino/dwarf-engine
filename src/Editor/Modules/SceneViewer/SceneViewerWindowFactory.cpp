#include "pch.h"

#include "SceneViewerWindowFactory.h"

namespace Dwarf
{
  SceneViewerWindowFactory::SceneViewerWindowFactory(
    std::shared_ptr<IDwarfLogger>              logger,
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
    : mLogger(std::move(logger))
    , mCameraFactory(std::move(cameraFactory))
    , mFramebufferFactory(std::move(framebufferFactory))
    , mEditorStats(std::move(editorStats))
    , mInputManager(std::move(inputManager))
    , mLoadedScene(std::move(loadedScene))
    , mEditorSelection(std::move(editorSelection))
    , mRenderingPipelineFactory(std::move(renderingPipelineFactory))
    , mRendererApiFactory(std::move(rendererApiFactory))
    , mShaderRegistry(std::move(shaderRegistry))
    , mShaderSourceCollectionFactory(std::move(shaderSourceCollectionFactory))
  {
    mLogger->LogDebug(
      Log("SceneViewerWindowFactory created", "SceneViewerWindowFactory"));
  }

  SceneViewerWindowFactory::~SceneViewerWindowFactory()
  {
    mLogger->LogDebug(
      Log("SceneViewerWindowFactory destroyed", "SceneViewerWindowFactory"));
  }

  auto
  SceneViewerWindowFactory::Create() const -> std::unique_ptr<SceneViewerWindow>
  {
    return std::make_unique<SceneViewerWindow>(mLogger,
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

  auto
  SceneViewerWindowFactory::Create(SerializedModule serializedModule) const
    -> std::unique_ptr<SceneViewerWindow>
  {
    return std::make_unique<SceneViewerWindow>(serializedModule,
                                               mLogger,
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