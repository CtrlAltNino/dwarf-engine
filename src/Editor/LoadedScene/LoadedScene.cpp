#include "pch.h"

#include "Editor/LoadedScene/LoadedScene.h"

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
    for (auto& callback : mSceneUnloadCallback)
    {
      callback.Callback();
    }

    mScene = std::move(scene);

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

      for (auto& callback : mSceneLoadCallback)
      {
        callback.Callback();
      }
    }
    else
    {
      mLogger->LogDebug(Log("Unloading scene", "LoadedScene"));
    }
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
    return mScene != nullptr;
  }

  auto
  LoadedScene::AddSceneLoadCallback(std::function<void()> callback) -> UUID
  {
    CallbackContainer newCallback(UUID(), callback);
    mSceneLoadCallback.emplace_back(newCallback);
    return newCallback.Id;
  }

  void
  LoadedScene::RemoveSceneLoadCallback(UUID id)
  {
    std::erase_if(mSceneLoadCallback,
                  [id](CallbackContainer& callback)
                  { return callback.Id == id; });
  }

  auto
  LoadedScene::AddSceneUnloadCallback(std::function<void()> callback) -> UUID
  {
    CallbackContainer newCallback(UUID(), callback);
    mSceneUnloadCallback.emplace_back(newCallback);
    return newCallback.Id;
  }

  void
  LoadedScene::RemoveSceneUnloadCallback(UUID id)
  {
    std::erase_if(mSceneUnloadCallback,
                  [id](CallbackContainer& callback)
                  { return callback.Id == id; });
  }

  auto
  LoadedScene::AddSceneChangeCallback(std::function<void()> callback) -> UUID
  {
    CallbackContainer newCallback(UUID(), callback);
    mSceneChangeCallback.emplace_back(newCallback);
    return newCallback.Id;
  }

  void
  LoadedScene::RemoveSceneChangeCallback(UUID id)
  {
    std::erase_if(mSceneChangeCallback,
                  [id](CallbackContainer& callback)
                  { return callback.Id == id; });
  }

  void
  LoadedScene::PropagateSceneChange()
  {
    for (auto& callback : mSceneChangeCallback)
    {
      callback.Callback();
    }
  }
}