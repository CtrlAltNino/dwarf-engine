#include "SceneIO.h"

#include "Utilities/FileHandler.h"
#include <nfd.h>

namespace Dwarf
{
  SceneIO::SceneIO(std::shared_ptr<IProjectSettings> projectSettings,
                   std::shared_ptr<ISceneFactory>    sceneFactory,
                   std::shared_ptr<IAssetDatabase>   assetDatabase)
    : m_SceneFactory(sceneFactory)
    , m_ProjectSettings(projectSettings)
    , m_AssetDatabase(assetDatabase)
  {
  }

  void
  SceneIO::SaveScene(IScene& scene) const
  {
    // TODO: Add error handling and logging.
    nlohmann::json sceneJson = scene.Serialize();

    if (scene.GetProperties().GetAsset().GetPath().empty())
    {
      WriteSceneToFile(scene.GetProperties().GetAsset().GetPath(), sceneJson);
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

    if (result == NFD_OKAY)
    {
      std::filesystem::path path(savePath);

      if (!path.has_extension())
      {
        path.concat(".dscene");
      }

      WriteSceneToFile(path, sceneJson);
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
  SceneIO::LoadScene(IAssetReference<SceneAsset>& sceneAsset) const
  {
    if (FileHandler::FileExists(sceneAsset.GetAsset().m_Path))
    {
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
      return LoadScene(*m_AssetDatabase->Retrieve<SceneAsset>(path));
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

  std::unique_ptr<IScene>
  SceneIO::LoadDefaultScene() const
  {
    return m_SceneFactory->NewEmptyScene();
  }

  void
  SceneIO::WriteSceneToFile(const nlohmann::json&        serializedScene,
                            const std::filesystem::path& scenePath) const
  {
    FileHandler::WriteToFile(scenePath, serializedScene.get<std::string>());
  }
}