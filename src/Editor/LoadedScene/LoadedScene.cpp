#include "pch.hpp"

#include "Editor/LoadedScene/LoadedScene.hpp"

namespace Dwarf
{
  LoadedScene::LoadedScene(GraphicsApi                       graphicsApi,
                           std::shared_ptr<IDwarfLogger>     logger,
                           std::shared_ptr<IWindow>          window,
                           std::shared_ptr<IProjectSettings> projectSettings)
    : mGraphicsApi(graphicsApi)
    , mLogger(std::move(logger))
    , mWindow(std::move(window))
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
    mLogger->LogDebug(Log("SetScene Start", "LoadedScene"));
    mHasLoadedScene = false;
    if (mScene)
    {
      for (auto* observer : mObservers)
      {
        observer->OnSceneUnload();
      }
    }
    mLogger->LogDebug(Log("SetScene After OnSceneUnload", "LoadedScene"));
    mScene = std::move(scene);
    mLogger->LogDebug(Log("SetScene After Assignment", "LoadedScene"));

    if (mScene)
    {
      mLogger->LogDebug(
        Log(fmt::format("Setting scene: {}", mScene->GetProperties().GetName()),
            "LoadedScene"));
      if (mScene->GetProperties().GetAssetId().has_value())
      {
        mProjectSettings->UpdateLastOpenedScene(
          mScene->GetProperties().GetAssetId().value());
      }

      UpdateWindowTitle();

      mHasLoadedScene = true;

      for (auto* observer : mObservers)
      {
        observer->OnSceneLoad();
      }
      mLogger->LogDebug(Log("SetScene After OnSceneLoad", "LoadedScene"));
    }
    else
    {
      mLogger->LogDebug(Log("Unloading scene", "LoadedScene"));
    }
    mLogger->LogDebug(Log("SetScene End", "LoadedScene"));
  }

  void
  LoadedScene::UpdateWindowTitle()
  {
    std::string windowTitle = "Dwarf Engine Editor - ";
    windowTitle.append(mProjectSettings->GetProjectName());
    windowTitle.append(" - ");
    windowTitle.append(GetScene().GetProperties().GetName());
    windowTitle.append(" <");
    windowTitle.append(magic_enum::enum_name(mGraphicsApi));
    windowTitle.append(">");

    mWindow->SetWindowTitle(windowTitle);
  }

  auto
  LoadedScene::HasLoadedScene() -> bool
  {
    return mHasLoadedScene;
  }

  void
  LoadedScene::RegisterLoadedSceneObserver(ILoadedSceneObserver* observer)
  {
    mObservers.push_back(observer);
  }

  void
  LoadedScene::UnregisterLoadedSceneObserver(ILoadedSceneObserver* observer)
  {
    std::erase(mObservers, observer);
  }
}