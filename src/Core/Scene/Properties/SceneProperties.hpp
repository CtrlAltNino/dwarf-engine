#pragma once

#include "Core/Scene/Properties/ISceneProperties.hpp"

namespace Dwarf
{
  /// @brief Interface for scene properties.
  class SceneProperties : public ISceneProperties
  {
  private:
    std::string                     mName;
    std::optional<UUID>             mAssetId;
    std::unique_ptr<ISceneSettings> mSettings;

  public:
    /// @brief Constructor.
    /// @param name The name of the scene.
    /// @param asset The asset reference of the scene.
    /// @param settings The settings of the scene.
    SceneProperties(std::optional<UUID>             assetId,
                    std::string_view                name,
                    std::unique_ptr<ISceneSettings> settings);
    ~SceneProperties() override = default;

    /// @copydoc ISerializable::Serialize
    auto
    Serialize() -> nlohmann::json override;

    /// @copydoc ISceneProperties::GetName
    [[nodiscard]] auto
    GetName() const -> std::string override;

    /**
     * @brief Updates the name of the scene
     *
     * @param sceneName New scene name
     */
    void
    SetName(const std::string& sceneName) override;

    /// @copydoc ISceneProperties::GetAsset
    [[nodiscard]] auto
    GetAssetId() const -> const std::optional<UUID>& override;

    /**
     * @brief Sets the asset id
     *
     * @param id UUID for the asset database
     */
    void
    SetAssetId(const UUID& id) override;

    /// @copydoc ISceneProperties::GetSettings
    [[nodiscard]] auto
    GetSettings() const -> ISceneSettings& override;
  };
}