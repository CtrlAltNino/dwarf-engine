#include "pch.hpp"

#include "SceneViewerWindowFactory.hpp"

namespace Dwarf
{
  SceneViewerWindowFactory::SceneViewerWindowFactory(
    std::shared_ptr<IDwarfLogger>              logger,
    std::shared_ptr<ICameraFactory>            cameraFactory,
    std::shared_ptr<IEditorStats>              editorStats,
    std::shared_ptr<IInputManager>             inputManager,
    std::shared_ptr<ILoadedScene>              loadedScene,
    std::shared_ptr<IEditorSelection>          editorSelection,
    std::shared_ptr<IRenderingPipelineFactory> renderingPipelineFactory,
    std::shared_ptr<IWindow>                   window)
    : mLogger(std::move(logger))
    , mCameraFactory(std::move(cameraFactory))
    , mEditorStats(std::move(editorStats))
    , mInputManager(std::move(inputManager))
    , mLoadedScene(std::move(loadedScene))
    , mEditorSelection(std::move(editorSelection))
    , mRenderingPipelineFactory(std::move(renderingPipelineFactory))
    , mWindow(std::move(window))
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
                                               mEditorStats,
                                               mInputManager,
                                               mLoadedScene,
                                               mEditorSelection,
                                               mCameraFactory,
                                               mRenderingPipelineFactory,
                                               mWindow);
  }

  auto
  SceneViewerWindowFactory::Create(SerializedModule serializedModule) const
    -> std::unique_ptr<SceneViewerWindow>
  {
    return std::make_unique<SceneViewerWindow>(serializedModule,
                                               mLogger,
                                               mEditorStats,
                                               mInputManager,
                                               mLoadedScene,
                                               mEditorSelection,
                                               mCameraFactory,
                                               mRenderingPipelineFactory,
                                               mWindow);
  }
} // namespace Dwarf