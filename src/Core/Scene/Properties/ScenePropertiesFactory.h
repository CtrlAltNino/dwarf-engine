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

    std::unique_ptr<ISceneProperties>
    Create(IAssetReference&      sceneAsset,
           const nlohmann::json& serializedProperties) override;

    std::unique_ptr<ISceneProperties>
    Create(const std::string& name) override;
  };
} // namespace Dwarf