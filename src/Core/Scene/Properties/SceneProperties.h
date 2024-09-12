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
    SceneProperties(std::unique_ptr<IAssetReference<SceneAsset>> asset,
                    const std::string&                           name,
                    std::unique_ptr<ISceneSettings>              settings);

    /// @copydoc ISerializable::Serialize
    nlohmann::json
    Serialize() const override;

    /// @copydoc ISceneProperties::GetName
    std::string
    GetName() const override;

    /// @copydoc ISceneProperties::GetAsset
    IAssetReference<SceneAsset>&
    GetAsset() const override;

    /// @copydoc ISceneProperties::GetSettings
    ISceneSettings&
    GetSettings() const override;

  private:
    std::string                                  m_Name;
    std::unique_ptr<IAssetReference<SceneAsset>> m_Asset;
    std::unique_ptr<ISceneSettings>              m_Settings;
  };
}