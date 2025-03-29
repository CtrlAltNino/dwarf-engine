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
    for (auto& callback : mSceneUnloadCallback)
    {
      callback();
    }

    mScene = std::move(scene);

    if (mScene)
    {
      mLogger->LogDebug(
        Log(fmt::format("Setting scene: {}", mScene->GetProperties().GetName()),
            "LoadedScene"));
      if (mScene->GetProperties().GetAssetID().has_value())
      {
        mProjectSettings->UpdateLastOpenedScene(
          mScene->GetProperties().GetAssetID().value());
      }

      for (auto& callback : mSceneLoadCallback)
      {
        callback();
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

  void
  LoadedScene::AddSceneLoadCallback(std::function<void()> callback)
  {
    mSceneLoadCallback.emplace_back(callback);
  }

  void
  LoadedScene::AddSceneUnloadCallback(std::function<void()> callback)
  {
    mSceneUnloadCallback.emplace_back(callback);
  }
}