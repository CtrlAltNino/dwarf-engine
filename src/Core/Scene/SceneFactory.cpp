#include "SceneFactory.h"
#include "Scene.h"
#include "Core/Asset/Database/IAssetDatabase.h"

namespace Dwarf
{
  SceneFactory::SceneFactory(
    std::shared_ptr<IScenePropertiesFactory> scenePropertiesFactory,
    std::shared_ptr<IAssetDatabase>          assetDatabase,
    std::shared_ptr<IFileHandler>            fileHandler)
    : m_ScenePropertiesFactory(scenePropertiesFactory)
    , m_AssetDatabase(assetDatabase)
    , m_FileHandler(fileHandler)
  {
  }

  std::unique_ptr<IScene>
  SceneFactory::FromAsset(IAssetReference& sceneAsset)
  {
    nlohmann::json serializedScene =
      nlohmann::json::parse(m_FileHandler->ReadFile(sceneAsset.GetPath()));
    return std::make_unique<Scene>(serializedScene["Graph"],
                                   m_ScenePropertiesFactory->Create(
                                     sceneAsset, serializedScene["Properties"]),
                                   m_AssetDatabase);
  }

  // TODO: Create default scene here
  std::unique_ptr<IScene>
  SceneFactory::CreateDefaultScene()
  {
    return std::make_unique<Scene>(
      nlohmann::json(""),
      m_ScenePropertiesFactory->Create("Unnamed Scene"),
      m_AssetDatabase);
  }
} // namespace Dwarf