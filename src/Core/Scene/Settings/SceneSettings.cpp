#include "SceneSettings.h"

namespace Dwarf
{
  SceneSettings::SceneSettings(nlohmann::json serializedSettings)
  {
    m_FogSettings = FogSettings(serializedSettings["FogSettings"]);
    m_GlobalLightSettings =
      GlobalLightSettings(serializedSettings["GlobalLightSettings"]);
    m_SkyboxMaterial =
      std::make_shared<UUID>(serializedSettings["SkyboxMaterial"]);
  }

  nlohmann::json
  SceneSettings::Serialize() const
  {
    nlohmann::json serializedSettings;
    serializedSettings["FogSettings"] = m_FogSettings.Serialize();
    serializedSettings["GlobalLightSettings"] =
      m_GlobalLightSettings.Serialize();
    serializedSettings["SkyboxMaterial"] = (uint32_t)*m_SkyboxMaterial;
    return serializedSettings;
  }

  FogSettings&
  SceneSettings::GetFogSettings()
  {
    return m_FogSettings;
  }

  GlobalLightSettings&
  SceneSettings::GetGlobalLightSettings()
  {
    return m_GlobalLightSettings;
  }

  std::shared_ptr<UUID>
  SceneSettings::GetSkyboxMaterial() const
  {
    return m_SkyboxMaterial;
  }

  void
  SceneSettings::SetSkyboxMaterial(std::shared_ptr<UUID> skyboxMaterial)
  {
    m_SkyboxMaterial = skyboxMaterial;
  }
}