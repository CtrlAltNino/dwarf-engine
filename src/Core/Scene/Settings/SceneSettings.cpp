#include "SceneSettings.h"
#include <nlohmann/json.hpp>

namespace Dwarf
{
  SceneSettings::SceneSettings(nlohmann::json serializedSettings)
  {
    mFogSettings = FogSettings(serializedSettings["FogSettings"]);
    mGlobalLightSettings =
      GlobalLightSettings(serializedSettings["GlobalLightSettings"]);
    mSkyboxMaterial =
      serializedSettings.contains("SkyboxMaterial") &&
          serializedSettings.at("SkyboxMaterial") != ""
        ? std::optional<UUID>(
            UUID(serializedSettings.at("SkyboxMaterial").get<std::string>()))
        : std::nullopt;
  }

  auto
  SceneSettings::Serialize() -> nlohmann::json
  {
    nlohmann::json serializedSettings;
    serializedSettings["FogSettings"] = mFogSettings.Serialize();
    serializedSettings["GlobalLightSettings"] =
      mGlobalLightSettings.Serialize();
    serializedSettings["SkyboxMaterial"] =
      mSkyboxMaterial.has_value() ? mSkyboxMaterial->toString() : "";
    return serializedSettings;
  }

  auto
  SceneSettings::GetFogSettings() -> FogSettings&
  {
    return mFogSettings;
  }

  auto
  SceneSettings::GetGlobalLightSettings() -> GlobalLightSettings&
  {
    return mGlobalLightSettings;
  }

  auto
  SceneSettings::GetSkyboxMaterial() -> std::optional<UUID>&
  {
    return mSkyboxMaterial;
  }
} // namespace Dwarf