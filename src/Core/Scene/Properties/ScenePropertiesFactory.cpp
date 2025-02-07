#include "ScenePropertiesFactory.h"
#include "Core/Asset/Database/AssetComponents.h"
#include "SceneProperties.h"

namespace Dwarf
{
  ScenePropertiesFactory::ScenePropertiesFactory(
    std::shared_ptr<ISceneSettingsFactory> sceneSettingsFactory)
    : m_sceneSettingsFactory(sceneSettingsFactory)
  {
  }

  std::unique_ptr<ISceneProperties>
  ScenePropertiesFactory::Create(IAssetReference&      sceneAsset,
                                 const nlohmann::json& serializedProperties)
  {
    return std::make_unique<SceneProperties>(
      sceneAsset.GetUID(),
      sceneAsset.GetPath().stem().string(),
      m_sceneSettingsFactory->Create(serializedProperties));
  }

  std::unique_ptr<ISceneProperties>
  ScenePropertiesFactory::Create(const std::string& name)
  {
    return std::make_unique<SceneProperties>(
      std::nullopt, name, m_sceneSettingsFactory->Create());
  }
}