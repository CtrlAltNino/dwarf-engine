#include "SceneSettings.h"

namespace Dwarf
{
  SceneSettings::SceneSettings(nlohmann::json serializedSettings)
  {
    m_FogSettings = FogSettings(serializedSettings["FogSettings"]);
    m_GlobalLightSettings =
      GlobalLightSettings(serializedSettings["GlobalLightSettings"]);
    m_SkyboxMaterial =
      serializedSettings.contains("SkyboxMaterial") &&
          serializedSettings.at("SkyboxMaterial") != ""
        ? std::optional<UUID>(
            UUID(serializedSettings.at("SkyboxMaterial").get<std::string>()))
        : std::nullopt;
  }

  nlohmann::json
  SceneSettings::Serialize()
  {
    nlohmann::json serializedSettings;
    serializedSettings["FogSettings"] = m_FogSettings.Serialize();
    serializedSettings["GlobalLightSettings"] =
      m_GlobalLightSettings.Serialize();
    serializedSettings["SkyboxMaterial"] =
      m_SkyboxMaterial.has_value() ? m_SkyboxMaterial->ToString() : "";
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

  std::optional<UUID>&
  SceneSettings::GetSkyboxMaterial()
  {
    return m_SkyboxMaterial;
  }
}