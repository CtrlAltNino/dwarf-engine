#include "SceneFactory.h"
#include "Scene.h"
#include "Core/Asset/Database/IAssetDatabase.h"

namespace Dwarf
{
  SceneFactory::SceneFactory(
    std::shared_ptr<IScenePropertiesFactory> scenePropertiesFactory)
    : m_ScenePropertiesFactory(scenePropertiesFactory)
  {
  }

  std::unique_ptr<IScene>
  SceneFactory::FromAsset(IAssetReference<SceneAsset>& sceneAsset)
  {
    nlohmann::json serializedScene =
      nlohmann::json::parse(FileHandler::ReadFile(sceneAsset.GetPath()));
    return std::make_unique<Scene>(
      serializedScene["Graph"],
      m_ScenePropertiesFactory->Create(sceneAsset,
                                       serializedScene["Properties"]));
  }

  std::unique_ptr<IScene>
  SceneFactory::NewSceneAsset(const std::filesystem::path& directory)
  {
    return std::make_unique<Scene>(
      nlohmann::json(""),
      m_ScenePropertiesFactory->Create(CreateNewSceneName(directory)));
  }

  // TODO: Create default scene here
  std::unique_ptr<IScene>
  SceneFactory::NewEmptyScene()
  {
    return std::make_unique<Scene>(
      nlohmann::json(""),
      m_ScenePropertiesFactory->Create(CreateNewSceneName("")));
  }

  std::string
  SceneFactory::CreateNewSceneName(const std::filesystem::path& directory)
  {
    // Check if "New Scene" already exists, if so, increment the number
    std::string sceneName = "New Scene";
    int         i = 1;
    while (FileHandler::FileExists(directory /
                                   (sceneName + std::to_string(i) + ".json")))
    {
      i++;
    }

    return sceneName + std::to_string(i);
  }
} // namespace Dwarf