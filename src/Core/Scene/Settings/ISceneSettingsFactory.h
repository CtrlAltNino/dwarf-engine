#pragma once

#include "ISceneSettings.h"
#include <memory>
#include <nlohmann/json.hpp>

namespace Dwarf
{
  /**
   * @brief Factory class creating SceneSettings instances
   *
   */
  class ISceneSettingsFactory
  {
  public:
    virtual ~ISceneSettingsFactory() = default;

    /**
     * @brief Creates an empty SceneSettings instance
     *
     * @return Unique pointer to the created SceneSettings instance
     */
    [[nodiscard]] virtual auto
    Create() const -> std::unique_ptr<ISceneSettings> = 0;

    /**
     * @brief Creates a SceneSettings instance from a serialized structure
     *
     * @param serializedSettings Serializes SceneSettings
     * @return Unique pointer to the created SceneSettings instance
     */
    [[nodiscard]] virtual auto
    Create(nlohmann::json serializedSettings) const
      -> std::unique_ptr<ISceneSettings> = 0;
  };
}