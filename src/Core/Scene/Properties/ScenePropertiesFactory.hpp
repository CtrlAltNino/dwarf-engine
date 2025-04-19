#pragma once

#include "Core/Scene/Settings/ISceneSettingsFactory.hpp"
#include "IScenePropertiesFactory.hpp"
#include "Logging/IDwarfLogger.hpp"

namespace Dwarf
{
  class ScenePropertiesFactory : public IScenePropertiesFactory
  {
  private:
    std::shared_ptr<IDwarfLogger>          mLogger;
    std::shared_ptr<ISceneSettingsFactory> mSceneSettingsFactory;

  public:
    ScenePropertiesFactory(
      std::shared_ptr<IDwarfLogger>          logger,
      std::shared_ptr<ISceneSettingsFactory> sceneSettingsFactory);
    ~ScenePropertiesFactory() override;

    [[nodiscard]] auto
    Create(IAssetReference&      sceneAsset,
           const nlohmann::json& serializedProperties) const
      -> std::unique_ptr<ISceneProperties> override;

    [[nodiscard]] auto
    Create(const std::string& name) const
      -> std::unique_ptr<ISceneProperties> override;
  };
} // namespace Dwarf