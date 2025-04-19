#include "pch.h"

#include "SceneSettingsWindowFactory.h"

namespace Dwarf
{
  SceneSettingsWindowFactory::SceneSettingsWindowFactory(
    std::shared_ptr<IDwarfLogger>        logger,
    std::shared_ptr<ILoadedScene>        loadedScene,
    std::shared_ptr<IAssetDatabase>      assetDatabase,
    std::shared_ptr<IRendererApiFactory> rendererApiFactory)
    : mLogger(std::move(logger))
    , mLoadedScene(std::move(loadedScene))
    , mAssetDatabase(std::move(assetDatabase))
    , mRendererApiFactory(std::move(rendererApiFactory))
  {
    mLogger->LogDebug(
      Log("SceneSettingsWindowFactory created", "SceneSettingsWindowFactory"));
  }

  SceneSettingsWindowFactory::~SceneSettingsWindowFactory()
  {
    mLogger->LogDebug(Log("SceneSettingsWindowFactory destroyed",
                          "SceneSettingsWindowFactory"));
  }

  auto
  SceneSettingsWindowFactory::Create() const
    -> std::unique_ptr<SceneSettingsWindow>
  {
    return std::make_unique<SceneSettingsWindow>(
      mLogger, mLoadedScene, mAssetDatabase, mRendererApiFactory->Create());
  }

  auto
  SceneSettingsWindowFactory::Create(SerializedModule serializedModule) const
    -> std::unique_ptr<SceneSettingsWindow>
  {
    return std::make_unique<SceneSettingsWindow>(serializedModule,
                                                 mLogger,
                                                 mLoadedScene,
                                                 mAssetDatabase,
                                                 mRendererApiFactory->Create());
  }
} // namespace Dwarf