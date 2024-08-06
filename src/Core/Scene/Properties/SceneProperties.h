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
    SceneProperties(std::shared_ptr<AssetReference<SceneAsset>> asset,
                    std::string                                 name,
                    std::shared_ptr<ISceneSettings>             settings);

    /// @copydoc ISerializable::Serialize
    nlohmann::json
    Serialize() const override;

    /// @copydoc ISceneProperties::GetName
    std::string
    GetName() const override;

    /// @copydoc ISceneProperties::GetAsset
    std::shared_ptr<AssetReference<SceneAsset>>
    GetAsset() const override;

    /// @copydoc ISceneProperties::GetSettings
    std::shared_ptr<ISceneSettings>
    GetSettings() const override;

  private:
    std::string                                 m_Name;
    std::shared_ptr<AssetReference<SceneAsset>> m_Asset;
    std::shared_ptr<ISceneSettings>             m_Settings;
  };
}