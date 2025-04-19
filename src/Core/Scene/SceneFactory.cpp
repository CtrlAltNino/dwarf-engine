#include "pch.hpp"

#include "Core/Asset/Database/IAssetDatabase.hpp"
#include "Scene.hpp"
#include "SceneFactory.hpp"

namespace Dwarf
{
  SceneFactory::SceneFactory(
    std::shared_ptr<IDwarfLogger>            logger,
    std::shared_ptr<IScenePropertiesFactory> scenePropertiesFactory,
    std::shared_ptr<IAssetDatabase>          assetDatabase,
    std::shared_ptr<IFileHandler>            fileHandler)
    : mLogger(std::move(logger))
    , mScenePropertiesFactory(std::move(scenePropertiesFactory))
    , mAssetDatabase(std::move(assetDatabase))
    , mFileHandler(std::move(fileHandler))
  {
    mLogger->LogDebug(Log("SceneFactory created.", "SceneFactory"));
  }

  SceneFactory::~SceneFactory()
  {
    mLogger->LogDebug(Log("SceneFactory created.", "SceneFactory"));
  }

  auto
  SceneFactory::FromAsset(IAssetReference& sceneAsset) const
    -> std::unique_ptr<IScene>
  {
    nlohmann::json serializedScene =
      nlohmann::json::parse(mFileHandler->ReadFile(sceneAsset.GetPath()));
    return std::make_unique<Scene>(
      SerializedGraph(serializedScene["Graph"]),
      mScenePropertiesFactory->Create(sceneAsset, serializedScene["Settings"]),
      mAssetDatabase);
  }

  // TODO: Create default scene here
  auto
  SceneFactory::CreateDefaultScene() const -> std::unique_ptr<IScene>
  {
    return std::make_unique<Scene>(mScenePropertiesFactory->Create("New Scene"),
                                   mAssetDatabase);
  }
} // namespace Dwarf