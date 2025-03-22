#include "ScenePropertiesFactory.h"

#include "SceneProperties.h"
#include <utility>

namespace Dwarf
{
  ScenePropertiesFactory::ScenePropertiesFactory(
    std::shared_ptr<IDwarfLogger>          logger,
    std::shared_ptr<ISceneSettingsFactory> sceneSettingsFactory)
    : mLogger(std::move(logger))
    , mSceneSettingsFactory(std::move(sceneSettingsFactory))
  {
    mLogger->LogDebug(
      Log("ScenePropertiesFactory created.", "ScenePropertiesFactory"));
  }
  ScenePropertiesFactory::~ScenePropertiesFactory()
  {
    mLogger->LogDebug(
      Log("ScenePropertiesFactory destroyed.", "ScenePropertiesFactory"));
  }

  auto
  ScenePropertiesFactory::Create(IAssetReference&      sceneAsset,
                                 const nlohmann::json& serializedProperties)
    const -> std::unique_ptr<ISceneProperties>
  {
    return std::make_unique<SceneProperties>(
      sceneAsset.GetUID(),
      sceneAsset.GetPath().stem().string(),
      mSceneSettingsFactory->Create(serializedProperties));
  }

  auto
  ScenePropertiesFactory::Create(const std::string& name) const
    -> std::unique_ptr<ISceneProperties>
  {
    return std::make_unique<SceneProperties>(
      std::nullopt, name, mSceneSettingsFactory->Create());
  }
}