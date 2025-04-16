#include "pch.h"

#include "SceneSettings.h"

namespace Dwarf
{
  SceneSettings::SceneSettings(nlohmann::json serializedSettings)
  {
    mFogSettings = FogSettings(serializedSettings["FogSettings"]);
    mAmbientLightSettings =
      AmbientLightSettings(serializedSettings["AmbientLightSettings"]);
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
    serializedSettings["AmbientLightSettings"] =
      mAmbientLightSettings.Serialize();
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
  SceneSettings::GetAmbientLightSettings() -> AmbientLightSettings&
  {
    return mAmbientLightSettings;
  }

  auto
  SceneSettings::GetSkyboxMaterial() -> std::optional<UUID>&
  {
    return mSkyboxMaterial;
  }
} // namespace Dwarf