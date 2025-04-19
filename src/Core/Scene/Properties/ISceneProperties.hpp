#pragma once

#include "Core/Scene/Settings/ISceneSettings.hpp"
#include "Utilities/ISerializable.hpp"

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

    /**
     * @brief Updates the name of the scene
     *
     * @param sceneName New scene name
     */
    virtual void
    SetName(const std::string& sceneName) = 0;

    /// @brief Returns the asset reference of the scene.
    /// @return The asset reference of the scene.
    [[nodiscard]] virtual auto
    GetAssetId() const -> const std::optional<UUID>& = 0;

    /**
     * @brief Sets the asset id
     *
     * @param id UUID for the asset database
     */
    virtual void
    SetAssetId(const UUID& id) = 0;

    /// @brief Returns the settings of the scene.
    /// @return The settings of the scene.
    [[nodiscard]] virtual auto
    GetSettings() const -> ISceneSettings& = 0;
  };
}