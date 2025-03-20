#include "SceneIO.h"

#include "Core/Asset/AssetReference/IAssetReference.h"
#include <fmt/format.h>
#include <nfd.h>

#include <utility>

namespace Dwarf
{
  SceneIO::SceneIO(std::shared_ptr<IDwarfLogger>     logger,
                   std::shared_ptr<IProjectSettings> projectSettings,
                   std::shared_ptr<ISceneFactory>    sceneFactory,
                   std::shared_ptr<IAssetDatabase>   assetDatabase,
                   std::shared_ptr<IFileHandler>     fileHandler)
    : mLogger(std::move(logger))
    , mSceneFactory(std::move(sceneFactory))
    , mProjectSettings(std::move(projectSettings))
    , mAssetDatabase(std::move(assetDatabase))
    , mFileHandler(std::move(fileHandler))
  {
  }

  void
  SceneIO::SaveScene(IScene& scene) const
  {
    if (scene.GetProperties().GetAssetID() != std::nullopt)
    {
      // TODO: Add error handling and logging.
      nlohmann::json sceneJson = scene.Serialize();
      if (mAssetDatabase->Exists(scene.GetProperties().GetAssetID().value()))
      {
        std::filesystem::path path =
          mAssetDatabase->Retrieve(scene.GetProperties().GetAssetID().value())
            ->GetPath();
        mLogger->LogDebug(
          Log(fmt::format("Saving serialized scene:\n{}", sceneJson.dump(2)),
              "SceneIO"));
        mLogger->LogDebug(Log(
          fmt::format("Writing scene to file: {}", path.string()), "SceneIO"));
        WriteSceneToFile(sceneJson, path);
      }
      else
      {
        mLogger->LogError(Log("Asset ID not found", "SceneIO"));
      }
    }
    else
    {
      SaveSceneDialog(scene);
    }
  }

  void
  SceneIO::SaveSceneDialog(IScene& scene) const
  {
    nfdu8char_t*                     savePath = nullptr;
    std::array<nfdu8filteritem_t, 1> filters = { { "Dwarf scene", "dscene" } };
    nfdsavedialogu8args_t            args = { 0 };
    std::string                      assetDirectoryPath =
      mAssetDatabase->GetAssetDirectoryPath().string();
    args.filterList = filters.data();
    args.filterCount = 1;
    args.defaultPath = assetDirectoryPath.c_str();

    nfdresult_t    result = NFD_SaveDialogU8_With(&savePath, &args);
    nlohmann::json sceneJson = scene.Serialize();

    mLogger->LogDebug(
      Log(fmt::format("Saving serialized scene: {}", sceneJson.dump(2)),
          "SceneIO"));

    if (result == NFD_OKAY)
    {
      std::filesystem::path path(savePath);

      if (!path.has_extension())
      {
        path.concat(".dscene");
      }

      WriteSceneToFile(sceneJson, path);
      NFD_FreePathU8(savePath);
    }
    else if (result == NFD_CANCEL)
    {
      mLogger->LogDebug(
        Log("User pressed cancel during File Dialog", "SceneIO"));
    }
    else
    {
      mLogger->LogError(
        Log(fmt::format("Error during NativeFileDialog: {}", NFD_GetError()),
            "SceneIO"));
    }
  }

  auto
  SceneIO::LoadScene(IAssetReference& sceneAsset) const
    -> std::unique_ptr<IScene>
  {
    if (mFileHandler->FileExists(sceneAsset.GetPath()))
    {
      mLogger->LogDebug(Log(fmt::format("Loading scene from asset: {}",
                                        sceneAsset.GetPath().string()),
                            "SceneIO"));
      return mSceneFactory->FromAsset(sceneAsset);
    }

    mLogger->LogDebug(Log(
      fmt::format("Asset file at {} not found", sceneAsset.GetPath().string()),
      "SceneIO"));
    return nullptr;
  }

  auto
  SceneIO::LoadSceneDialog() const -> std::unique_ptr<IScene>
  {
    nfdu8char_t*                     savePath = nullptr;
    std::array<nfdu8filteritem_t, 1> filters = { { "Dwarf scene", "dscene" } };
    nfdopendialogu8args_t            args = { 0 };
    std::string                      assetDirectoryPath =
      mAssetDatabase->GetAssetDirectoryPath().string();
    args.filterList = filters.data();
    args.filterCount = 1;
    args.defaultPath = assetDirectoryPath.c_str();

    nfdresult_t result = NFD_OpenDialogU8_With(&savePath, &args);
    if (result == NFD_OKAY)
    {
      std::filesystem::path path(savePath);
      NFD_FreePathU8(savePath);
      std::unique_ptr<IAssetReference> sceneAsset =
        mAssetDatabase->Retrieve(path);
      return LoadScene(*sceneAsset);
    }

    if (result == NFD_CANCEL)
    {
      mLogger->LogDebug(
        Log("User pressed cancel during File Dialog", "SceneIO"));
    }

    if (result == NFD_ERROR)
    {
      mLogger->LogError(
        Log(fmt::format("Error during NativeFileDialog: {}", NFD_GetError()),
            "SceneIO"));
    }

    return nullptr;
  }

  auto
  SceneIO::CreateNewSceneName(const std::filesystem::path& directory)
    -> std::string
  {
    // Check if "New Scene" already exists, if so, increment the number
    std::string sceneName = "New Scene";
    int         sceneNameIndex = 1;
    while (mFileHandler->FileExists(
      directory / (sceneName + std::to_string(sceneNameIndex) + ".scene")))
    {
      sceneNameIndex++;
    }

    return fmt::format(
      "{} ({}).dscene", sceneName, std::to_string(sceneNameIndex));
  }

  void
  SceneIO::WriteSceneToFile(const nlohmann::json&        serializedScene,
                            const std::filesystem::path& scenePath) const
  {
    mFileHandler->WriteToFile(scenePath, serializedScene.dump(2));
  }

  void
  SceneIO::NewSceneAsset(const std::filesystem::path& directory)
  {
    std::unique_ptr<IScene> scene = mSceneFactory->CreateDefaultScene();
    nlohmann::json          sceneJson = scene->Serialize();
    std::string             name = CreateNewSceneName(directory);
    WriteSceneToFile(sceneJson, directory / name);
  }
}