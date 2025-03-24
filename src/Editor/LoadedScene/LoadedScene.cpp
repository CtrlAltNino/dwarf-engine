#include "pch.h"

#include "Editor/LoadedScene/LoadedScene.h"

namespace Dwarf
{
  LoadedScene::LoadedScene(std::shared_ptr<IDwarfLogger>     logger,
                           std::shared_ptr<IProjectSettings> projectSettings)
    : mLogger(std::move(logger))
    , mProjectSettings(std::move(projectSettings))
  {
    mLogger->LogDebug(Log("LoadedScene created", "LoadedScene"));
  }

  LoadedScene::~LoadedScene()
  {
    mLogger->LogDebug(Log("LoadedScene destroyed", "LoadedScene"));
  }

  auto
  LoadedScene::GetScene() -> IScene&
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

  auto
  LoadedScene::HasLoadedScene() -> bool
  {
    return mScene != nullptr;
  }
}