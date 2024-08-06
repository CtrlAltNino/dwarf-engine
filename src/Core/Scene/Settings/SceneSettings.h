#include "ISceneSettings.h"

namespace Dwarf
{
  /// @brief Interface for scene properties.
  class SceneSettings : public ISceneSettings
  {
  private:
    FogSettings          m_FogSettings;
    GlobalLightSettings  m_GlobalLightSettings;
    std::shared_ptr<UID> m_SkyboxMaterial;

  public:
    SceneSettings() = default;
    SceneSettings(nlohmann::json serializedSettings);

    /// @copydoc ISerializable::Serialize
    nlohmann::json
    Serialize() const override;

    /// @copydoc ISceneSettings::GetFogSettings
    FogSettings&
    GetFogSettings() override;

    /// @copydoc ISceneSettings::GetGlobalLightSettings
    GlobalLightSettings&
    GetGlobalLightSettings() override;

    /// @copydoc ISceneSettings::GetSkyboxMaterial
    std::shared_ptr<UID>
    GetSkyboxMaterial() const override;

    /// @copydoc ISceneSettings::SetSkyboxMaterial
    void
    SetSkyboxMaterial(std::shared_ptr<UID> skyboxMaterial) override;
  };
} // namespace Dwarf