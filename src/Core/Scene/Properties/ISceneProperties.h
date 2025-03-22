#pragma once

#include "Core/Scene/Settings/ISceneSettings.h"
#include "Utilities/ISerializable.h"

namespace Dwarf
{
  /// @brief Interface for scene properties.
  class ISceneProperties : public ISerializable
  {
  public:
    /// @brief Destructor.
    virtual ~ISceneProperties() = default;

    /// @brief Returns the name of the scene.
    /// @return The name of the scene.
    [[nodiscard]] virtual auto
    GetName() const -> std::string = 0;

    /// @brief Returns the asset reference of the scene.
    /// @return The asset reference of the scene.
    [[nodiscard]] virtual auto
    GetAssetID() const -> const std::optional<UUID>& = 0;

    /// @brief Returns the settings of the scene.
    /// @return The settings of the scene.
    [[nodiscard]] virtual auto
    GetSettings() const -> ISceneSettings& = 0;
  };
}