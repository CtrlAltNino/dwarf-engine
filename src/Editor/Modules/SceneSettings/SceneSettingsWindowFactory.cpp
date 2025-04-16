#include "pch.h"

#include "SceneSettingsWindowFactory.h"

namespace Dwarf
{
  SceneSettingsWindowFactory::SceneSettingsWindowFactory(
    std::shared_ptr<IDwarfLogger>   logger,
    std::shared_ptr<ILoadedScene>   loadedScene,
    std::shared_ptr<IAssetDatabase> assetDatabase)
    : mLogger(std::move(logger))
    , mLoadedScene(std::move(loadedScene))
    , mAssetDatabase(std::move(assetDatabase))
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
      mLogger, mLoadedScene, mAssetDatabase);
  }

  auto
  SceneSettingsWindowFactory::Create(SerializedModule serializedModule) const
    -> std::unique_ptr<SceneSettingsWindow>
  {
    return std::make_unique<SceneSettingsWindow>(
      serializedModule, mLogger, mLoadedScene, mAssetDatabase);
  }
} // namespace Dwarf