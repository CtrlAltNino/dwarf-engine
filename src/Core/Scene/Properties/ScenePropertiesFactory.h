#pragma once

#include "Core/Scene/Settings/ISceneSettingsFactory.h"
#include "IScenePropertiesFactory.h"

namespace Dwarf
{
  class ScenePropertiesFactory : public IScenePropertiesFactory
  {
  private:
    std::shared_ptr<ISceneSettingsFactory> m_sceneSettingsFactory;

  public:
    ScenePropertiesFactory(
      std::shared_ptr<ISceneSettingsFactory> sceneSettingsFactory);
    std::shared_ptr<ISceneProperties>
    Create(std::shared_ptr<AssetReference<SceneAsset>> sceneAsset,
           nlohmann::json serializedProperties) override;

    std::shared_ptr<ISceneProperties>
    Create(std::string name) override;
  };
} // namespace Dwarf