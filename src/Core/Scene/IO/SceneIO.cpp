#include "SceneIO.h"

#include "Core/Asset/AssetReference/IAssetReference.h"
#include "Core/Asset/Database/AssetComponents.h"
#include <nfd.h>
#include <fmt/format.h>

namespace Dwarf
{
  SceneIO::SceneIO(std::shared_ptr<IDwarfLogger>     logger,
                   std::shared_ptr<IProjectSettings> projectSettings,
                   std::shared_ptr<ISceneFactory>    sceneFactory,
                   std::shared_ptr<IAssetDatabase>   assetDatabase,
                   std::shared_ptr<IFileHandler>     fileHandler)
    : m_Logger(logger)
    , m_SceneFactory(sceneFactory)
    , m_ProjectSettings(projectSettings)
    , m_AssetDatabase(assetDatabase)
    , m_FileHandler(fileHandler)
  {
  }

  void
  SceneIO::SaveScene(IScene& scene) const
  {
    if (scene.GetProperties().GetAssetID() != std::nullopt)
    {
      // TODO: Add error handling and logging.
      nlohmann::json sceneJson = scene.Serialize();
      if (m_AssetDatabase->Exists(scene.GetProperties().GetAssetID().value()))
      {
        std::filesystem::path path =
          m_AssetDatabase->Retrieve(scene.GetProperties().GetAssetID().value())
            ->GetPath();
        m_Logger->LogDebug(
          Log(fmt::format("Saving serialized scene:\n{}", sceneJson.dump(2)),
              "SceneIO"));
        m_Logger->LogDebug(Log(
          fmt::format("Writing scene to file: {}", path.string()), "SceneIO"));
        WriteSceneToFile(sceneJson, path);
      }
      else
      {
        m_Logger->LogError(Log("Asset ID not found", "SceneIO"));
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
    nfdu8char_t*          savePath = nullptr;
    nfdu8filteritem_t     filters[1] = { { "Dwarf scene", "dscene" } };
    nfdsavedialogu8args_t args = { 0 };
    args.filterList = filters;
    args.filterCount = 1;
    args.defaultPath =
      m_AssetDatabase->GetAssetDirectoryPath().string().c_str();

    nfdresult_t    result = NFD_SaveDialogU8_With(&savePath, &args);
    nlohmann::json sceneJson = scene.Serialize();

    m_Logger->LogDebug(
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
      delete savePath;
    }
    else if (result == NFD_CANCEL)
    {
      // return false;
      //  TODO: Add logging.
    }
    else
    {
      printf("Error: %s\n", NFD_GetError());
      // return false;
      //  TODO: Add logging.
    }
  }

  std::unique_ptr<IScene>
  SceneIO::LoadScene(IAssetReference& sceneAsset) const
  {
    if (m_FileHandler->FileExists(sceneAsset.GetPath()))
    {
      m_Logger->LogDebug(Log(fmt::format("Loading scene from asset: {}",
                                         sceneAsset.GetPath().string()),
                             "SceneIO"));
      return m_SceneFactory->FromAsset(sceneAsset);
    }
    else
    {
      // TODO: Add logging.
      return nullptr;
    }
  }

  std::unique_ptr<IScene>
  SceneIO::LoadSceneDialog() const
  {
    nfdu8char_t*          savePath = nullptr;
    nfdu8filteritem_t     filters[1] = { { "Dwarf scene", "dscene" } };
    nfdopendialogu8args_t args = { 0 };
    args.filterList = filters;
    args.filterCount = 1;
    args.defaultPath =
      m_AssetDatabase->GetAssetDirectoryPath().string().c_str();

    if (nfdresult_t result = NFD_OpenDialogU8_With(&savePath, &args);
        result == NFD_OKAY)
    {
      std::filesystem::path path(savePath);
      delete savePath;
      std::unique_ptr<IAssetReference> sceneAsset =
        m_AssetDatabase->Retrieve(path);
      return LoadScene(*sceneAsset);
    }
    else if (result == NFD_CANCEL)
    {
      // TODO: Cancel stuff?
      return nullptr;
    }
    else
    {
      printf("Error: %s\n", NFD_GetError());
      return nullptr;
    }
  }

  std::string
  SceneIO::CreateNewSceneName(const std::filesystem::path& directory)
  {
    // Check if "New Scene" already exists, if so, increment the number
    std::string sceneName = "New Scene";
    int         i = 1;
    while (m_FileHandler->FileExists(
      directory / (sceneName + std::to_string(i) + ".scene")))
    {
      i++;
    }

    return fmt::format("{} ({}).dscene", sceneName, std::to_string(i));
  }

  void
  SceneIO::WriteSceneToFile(const nlohmann::json&        serializedScene,
                            const std::filesystem::path& scenePath) const
  {
    m_FileHandler->WriteToFile(scenePath, serializedScene.dump(2));
  }

  void
  SceneIO::NewSceneAsset(const std::filesystem::path& directory)
  {
    std::unique_ptr<IScene> scene = m_SceneFactory->CreateDefaultScene();
    nlohmann::json          sceneJson = scene->Serialize();
    std::string             name = CreateNewSceneName(directory);
    WriteSceneToFile(sceneJson, directory / name);
  }
}