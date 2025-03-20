#pragma once

#include "Editor/Modules/SceneViewer/ISceneViewerWindowFactory.h"
#include "Editor/Modules/SceneViewer/SceneViewerWindow.h"
#include "Project/IProjectSettings.h"
#include <boost/di.hpp>

namespace Dwarf
{
  class SceneViewerWindowFactory : public ISceneViewerWindowFactory
  {
  private:
    std::shared_ptr<ICameraFactory>            mCameraFactory;
    std::shared_ptr<IFramebufferFactory>       mFramebufferFactory;
    std::shared_ptr<IEditorStats>              mEditorStats;
    std::shared_ptr<IInputManager>             mInputManager;
    std::shared_ptr<ILoadedScene>              mLoadedScene;
    std::shared_ptr<IEditorSelection>          mEditorSelection;
    std::shared_ptr<IRenderingPipelineFactory> mRenderingPipelineFactory;
    std::shared_ptr<IRendererApiFactory>       mRendererApiFactory;
    std::shared_ptr<IShaderRegistry>           mShaderRegistry;
    std::shared_ptr<IShaderSourceCollectionFactory>
      mShaderSourceCollectionFactory;

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
      std::shared_ptr<IRendererApiFactory>       rendererApiFactory,
      std::shared_ptr<IShaderRegistry>           shaderRegistry,
      std::shared_ptr<IShaderSourceCollectionFactory>
        shaderSourceCollectionFactory);

    ~SceneViewerWindowFactory() override = default;

    std::unique_ptr<SceneViewerWindow>
    Create() const override;

    std::unique_ptr<SceneViewerWindow>
    Create(SerializedModule serializedModule) const override;
  };
} // namespace Dwarf