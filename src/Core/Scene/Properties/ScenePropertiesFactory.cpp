#include "ScenePropertiesFactory.h"
#include "Core/Asset/Database/AssetComponents.h"
#include "Core/Asset/Database/AssetReference.h"
#include "SceneProperties.h"

namespace Dwarf
{
  ScenePropertiesFactory::ScenePropertiesFactory(
    std::shared_ptr<ISceneSettingsFactory> sceneSettingsFactory)
    : m_sceneSettingsFactory(sceneSettingsFactory)
  {
  }

  std::shared_ptr<ISceneProperties>
  ScenePropertiesFactory::Create(AssetReference<SceneAsset> sceneAsset,
                                 nlohmann::json serializedProperties)
  {
    return std::make_shared<SceneProperties>(
      sceneAsset,
      serializedProperties["Name"],
      m_sceneSettingsFactory->Create(serializedProperties["Settings"]));
  }

  std::shared_ptr<ISceneProperties>
  ScenePropertiesFactory::Create(std::string name)
  {
    return std::make_shared<SceneProperties>(
      nullptr, name, m_sceneSettingsFactory->Create());
  }
}