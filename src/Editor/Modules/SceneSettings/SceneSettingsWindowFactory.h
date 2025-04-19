#pragma once

#include "Core/Rendering/RendererApi/IRendererApiFactory.h"
#include "Core/Scene/Settings/SceneSettings.h"
#include "ISceneSettingsWindowFactory.h"
#include "SceneSettingsWindow.h"

namespace Dwarf
{
  class SceneSettingsWindowFactory : public ISceneSettingsWindowFactory
  {
  private:
    std::shared_ptr<IDwarfLogger>        mLogger;
    std::shared_ptr<ILoadedScene>        mLoadedScene;
    std::shared_ptr<IAssetDatabase>      mAssetDatabase;
    std::shared_ptr<IRendererApiFactory> mRendererApiFactory;

  public:
    SceneSettingsWindowFactory(
      std::shared_ptr<IDwarfLogger>        logger,
      std::shared_ptr<ILoadedScene>        loadedScene,
      std::shared_ptr<IAssetDatabase>      assetDatabase,
      std::shared_ptr<IRendererApiFactory> rendererApiFactory);

    ~SceneSettingsWindowFactory() override;

    /**
     * @brief Creates a default SceneSettingsWindow instance
     *
     * @return Unique pointer to the created instance
     */
    [[nodiscard]] auto
    Create() const -> std::unique_ptr<SceneSettingsWindow> override;

    /**
     * @brief Creates a SceneSettingsWindow instance based off serialized data
     *
     * @param serializedModule Serialized data of a SceneSettingsWindow
     * @return Unique pointer to the created instance
     */
    [[nodiscard]] auto
    Create(SerializedModule serializedModule) const
      -> std::unique_ptr<SceneSettingsWindow> override;
  };
}