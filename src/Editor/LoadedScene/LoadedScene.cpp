#include "Editor/LoadedScene/LoadedScene.h"
#include <fmt/format.h>

namespace Dwarf
{
  LoadedScene::LoadedScene(std::shared_ptr<IDwarfLogger>     logger,
                           std::shared_ptr<IProjectSettings> projectSettings)
    : mLogger(logger)
    , mProjectSettings(projectSettings)
  {
    mLogger->LogDebug(Log("LoadedScene created", "LoadedScene"));
  }

  LoadedScene::~LoadedScene()
  {
    mLogger->LogDebug(Log("LoadedScene destroyed", "LoadedScene"));
  }

  IScene&
  LoadedScene::GetScene()
  {
    return *mScene;
  }

  void
  LoadedScene::SetScene(std::unique_ptr<IScene> scene)
  {
    mScene = std::move(scene);
    if (scene)
    {
      mLogger->LogDebug(
        Log(fmt::format("Setting scene: {}", mScene->GetProperties().GetName()),
            "LoadedScene"));
      if (mScene->GetProperties().GetAssetID().has_value())
      {
        mProjectSettings->UpdateLastOpenedScene(
          mScene->GetProperties().GetAssetID().value());
      }
    }
    else
    {
      mLogger->LogDebug(Log("Unloading scene", "LoadedScene"));
    }
  }
}