#pragma once

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

  std::shared_ptr<IScene>
  SceneFactory::FromAsset(const AssetReference<SceneAsset>& sceneAsset)
  {
    nlohmann::json serializedScene =
      nlohmann::json::parse(FileHandler::ReadFile(sceneAsset.GetPath()));
    return std::make_shared<Scene>(
      serializedScene["Graph"],
      m_ScenePropertiesFactory->Create(sceneAsset,
                                       serializedScene["Properties"]));
  }

  std::shared_ptr<IScene>
  SceneFactory::New(std::filesystem::path directory)
  {
    return std::make_shared<Scene>(
      nlohmann::json(""),
      m_ScenePropertiesFactory->Create(CreateNewSceneName(directory)));
  }

  std::string
  SceneFactory::CreateNewSceneName(std::filesystem::path directory)
  {
    // Check if "New Scene" already exists, if so, increment the number
    std::string sceneName = "New Scene";
    int         i = 1;
    while (FileHandler::CheckIfFileExists(
      directory / (sceneName + std::to_string(i) + ".json")))
    {
      i++;
    }

    return sceneName + std::to_string(i);
  }
} // namespace Dwarf