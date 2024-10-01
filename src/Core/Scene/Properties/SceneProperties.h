#pragma once

#include "Core/Scene/Properties/ISceneProperties.h"

namespace Dwarf
{
  /// @brief Interface for scene properties.
  class SceneProperties : public ISceneProperties
  {
  public:
    /// @brief Constructor.
    /// @param name The name of the scene.
    /// @param asset The asset reference of the scene.
    /// @param settings The settings of the scene.
    SceneProperties(std::optional<UUID>             assetID,
                    const std::string&              name,
                    std::unique_ptr<ISceneSettings> settings);

    /// @copydoc ISerializable::Serialize
    nlohmann::json
    Serialize() override;

    /// @copydoc ISceneProperties::GetName
    std::string
    GetName() const override;

    /// @copydoc ISceneProperties::GetAsset
    const std::optional<UUID>&
    GetAssetID() const override;

    /// @copydoc ISceneProperties::GetSettings
    ISceneSettings&
    GetSettings() const override;

  private:
    std::string                     m_Name;
    std::optional<UUID>             m_AssetID;
    std::unique_ptr<ISceneSettings> m_Settings;
  };
}