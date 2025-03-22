#pragma once

#include "ISceneSettingsFactory.h"
#include "Logging/IDwarfLogger.h"

namespace Dwarf
{
  class SceneSettingsFactory : public ISceneSettingsFactory
  {
  private:
    std::shared_ptr<IDwarfLogger> mLogger;

  public:
    SceneSettingsFactory(std::shared_ptr<IDwarfLogger> logger);
    ~SceneSettingsFactory() override;

    /**
     * @brief Creates an empty SceneSettings instance
     *
     * @return Unique pointer to the created SceneSettings instance
     */
    [[nodiscard]] auto
    Create() const -> std::unique_ptr<ISceneSettings> override;

    /**
     * @brief Creates a SceneSettings instance from a serialized structure
     *
     * @param serializedSettings Serializes SceneSettings
     * @return Unique pointer to the created SceneSettings instance
     */
    [[nodiscard]] auto
    Create(nlohmann::json serializedSettings) const
      -> std::unique_ptr<ISceneSettings> override;
  };
} // namespace Dwarf