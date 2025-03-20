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

  nlohmann::json
  SceneSettings::Serialize()
  {
    nlohmann::json serializedSettings;
    serializedSettings["FogSettings"] = mFogSettings.Serialize();
    serializedSettings["GlobalLightSettings"] =
      mGlobalLightSettings.Serialize();
    serializedSettings["SkyboxMaterial"] =
      mSkyboxMaterial.has_value() ? mSkyboxMaterial->toString() : "";
    return serializedSettings;
  }

  FogSettings&
  SceneSettings::GetFogSettings()
  {
    return mFogSettings;
  }

  GlobalLightSettings&
  SceneSettings::GetGlobalLightSettings()
  {
    return mGlobalLightSettings;
  }

  std::optional<UUID>&
  SceneSettings::GetSkyboxMaterial()
  {
    return mSkyboxMaterial;
  }
} // namespace Dwarf