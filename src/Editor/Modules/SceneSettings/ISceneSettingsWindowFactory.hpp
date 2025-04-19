#pragma once

#include "SceneSettingsWindow.hpp"

namespace Dwarf
{
  class ISceneSettingsWindowFactory
  {
  public:
    virtual ~ISceneSettingsWindowFactory() = default;

    /**
     * @brief Creates a default SceneSettingsWindowSceneSettingsWindow instance
     *
     * @return Unique pointer to the created instance
     */
    [[nodiscard]] virtual auto
    Create() const -> std::unique_ptr<SceneSettingsWindow> = 0;

    /**
     * @brief Creates a SceneSettingsWindow instance based off serialized data
     *
     * @param serializedModule Serialized data of a SceneSettingsWindow
     * @return Unique pointer to the created instance
     */
    [[nodiscard]] virtual auto
    Create(SerializedModule serializedModule) const
      -> std::unique_ptr<SceneSettingsWindow> = 0;
  };
}