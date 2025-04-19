#pragma once

#include "Editor/Modules/SceneViewer/ISceneViewerWindowFactory.hpp"
#include "Editor/Modules/SceneViewer/SceneViewerWindow.hpp"
#include "Logging/IDwarfLogger.hpp"
#include <boost/di.hpp>

namespace Dwarf
{
  class SceneViewerWindowFactory : public ISceneViewerWindowFactory
  {
  private:
    std::shared_ptr<IDwarfLogger>              mLogger;
    std::shared_ptr<ICameraFactory>            mCameraFactory;
    std::shared_ptr<IEditorStats>              mEditorStats;
    std::shared_ptr<IInputManager>             mInputManager;
    std::shared_ptr<ILoadedScene>              mLoadedScene;
    std::shared_ptr<IEditorSelection>          mEditorSelection;
    std::shared_ptr<IRenderingPipelineFactory> mRenderingPipelineFactory;
    std::shared_ptr<IWindow>                   mWindow;

  public:
    SceneViewerWindowFactory(
      std::shared_ptr<IDwarfLogger>              logger,
      std::shared_ptr<ICameraFactory>            cameraFactory,
      std::shared_ptr<IEditorStats>              editorStats,
      std::shared_ptr<IInputManager>             inputManager,
      std::shared_ptr<ILoadedScene>              loadedScene,
      std::shared_ptr<IEditorSelection>          editorSelection,
      std::shared_ptr<IRenderingPipelineFactory> renderingPipelineFactory,
      std::shared_ptr<IWindow>                   window);

    ~SceneViewerWindowFactory() override;

    /**
     * @brief Creates a default SceneViewerWindow instance
     *
     * @return Unique pointer to the created instance
     */
    [[nodiscard]] auto
    Create() const -> std::unique_ptr<SceneViewerWindow> override;

    /**
     * @brief Creates a SceneViewerWindow instance based off serialized data
     *
     * @param serializedModule Serialized data of a SceneViewerWindow
     * @return Unique pointer to the created instance
     */
    [[nodiscard]] auto
    Create(SerializedModule serializedModule) const
      -> std::unique_ptr<SceneViewerWindow> override;
  };
} // namespace Dwarf