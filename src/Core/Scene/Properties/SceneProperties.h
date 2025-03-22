#pragma once

#include "Core/Scene/Properties/ISceneProperties.h"

namespace Dwarf
{
  /// @brief Interface for scene properties.
  class SceneProperties : public ISceneProperties
  {
  private:
    std::string                     mName;
    std::optional<UUID>             mAssetID;
    std::unique_ptr<ISceneSettings> mSettings;

  public:
    /// @brief Constructor.
    /// @param name The name of the scene.
    /// @param asset The asset reference of the scene.
    /// @param settings The settings of the scene.
    SceneProperties(std::optional<UUID>             assetID,
                    std::string_view                name,
                    std::unique_ptr<ISceneSettings> settings);
    ~SceneProperties() override = default;

    /// @copydoc ISerializable::Serialize
    auto
    Serialize() -> nlohmann::json override;

    /// @copydoc ISceneProperties::GetName
    [[nodiscard]] auto
    GetName() const -> std::string override;

    /// @copydoc ISceneProperties::GetAsset
    [[nodiscard]] auto
    GetAssetID() const -> const std::optional<UUID>& override;

    /// @copydoc ISceneProperties::GetSettings
    [[nodiscard]] auto
    GetSettings() const -> ISceneSettings& override;
  };
}