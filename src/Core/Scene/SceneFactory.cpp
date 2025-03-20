#include "SceneFactory.h"
#include "Core/Asset/Database/IAssetDatabase.h"
#include "Scene.h"

namespace Dwarf
{
  SceneFactory::SceneFactory(
    std::shared_ptr<IScenePropertiesFactory> scenePropertiesFactory,
    std::shared_ptr<IAssetDatabase>          assetDatabase,
    std::shared_ptr<IFileHandler>            fileHandler)
    : mScenePropertiesFactory(scenePropertiesFactory)
    , mAssetDatabase(assetDatabase)
    , mFileHandler(fileHandler)
  {
  }

  std::unique_ptr<IScene>
  SceneFactory::FromAsset(IAssetReference& sceneAsset)
  {
    nlohmann::json serializedScene =
      nlohmann::json::parse(mFileHandler->ReadFile(sceneAsset.GetPath()));
    return std::make_unique<Scene>(
      SerializedGraph(serializedScene["Graph"]),
      mScenePropertiesFactory->Create(sceneAsset, serializedScene["Settings"]),
      mAssetDatabase);
  }

  // TODO: Create default scene here
  std::unique_ptr<IScene>
  SceneFactory::CreateDefaultScene()
  {
    return std::make_unique<Scene>(
      mScenePropertiesFactory->Create("Unnamed Scene"), mAssetDatabase);
  }
} // namespace Dwarf