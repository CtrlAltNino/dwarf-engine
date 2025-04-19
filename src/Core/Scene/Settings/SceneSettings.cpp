#include "pch.hpp"

#include "AmbientSettings.hpp"
#include "SceneSettings.hpp"

namespace Dwarf
{
  SceneSettings::SceneSettings(nlohmann::json serializedSettings)
  {
    if (serializedSettings.contains("AmbientLightSettings"))
    {
      mAmbientLightSettings =
        AmbientSettings(serializedSettings["AmbientLightSettings"]);
    }

    if (serializedSettings.contains("AntiAliasingSettings"))
    {
      mAntiAliasingSettings =
        AntiAliasingSettings(serializedSettings["AntiAliasingSettings"]);
    }

    if (serializedSettings.contains("BloomSettings"))
    {
      mBloomSettings = BloomSettings(serializedSettings["BloomSettings"]);
    }

    if (serializedSettings.contains("ExposureSettings"))
    {
      mExposureSettings =
        ExposureSettings(serializedSettings["ExposureSettings"]);
    }

    if (serializedSettings.contains("FogSettings"))
    {
      mFogSettings = FogSettings(serializedSettings["FogSettings"]);
    }

    if (serializedSettings.contains("ShadowSettings"))
    {
      mShadowSettings = ShadowSettings(serializedSettings["ShadowSettings"]);
    }

    if (serializedSettings.contains("TonemapType"))
    {
      mTonemapType = serializedSettings["TonemapType"].get<TonemapType>();
    }
  }

  auto
  SceneSettings::Serialize() -> nlohmann::json
  {
    nlohmann::json serializedSettings;

    serializedSettings["AmbientLightSettings"] =
      mAmbientLightSettings.Serialize();

    serializedSettings["AntiAliasingSettings"] =
      mAntiAliasingSettings.Serialize();

    serializedSettings["BloomSettings"] = mBloomSettings.Serialize();

    serializedSettings["ExposureSettings"] = mExposureSettings.Serialize();

    serializedSettings["FogSettings"] = mFogSettings.Serialize();

    serializedSettings["ShadowSettings"] = mShadowSettings.Serialize();

    serializedSettings["SkyboxSettings"] = mSkyboxSettings.Serialize();

    serializedSettings["TonemapType"] = mTonemapType;

    return serializedSettings;
  }

  auto
  SceneSettings::GetFogSettings() -> FogSettings&
  {
    return mFogSettings;
  }

  auto
  SceneSettings::GetAmbientSettings() -> AmbientSettings&
  {
    return mAmbientLightSettings;
  }

  auto
  SceneSettings::GetSkyboxSettings() -> SkyboxSettings&
  {
    return mSkyboxSettings;
  }

  auto
  SceneSettings::GetExposureSettings() -> ExposureSettings&
  {
    return mExposureSettings;
  }

  auto
  SceneSettings::GetToneMapType() -> TonemapType&
  {
    return mTonemapType;
  }

  auto
  SceneSettings::GetAntiAliasingSettings() -> AntiAliasingSettings&
  {
    return mAntiAliasingSettings;
  }

  auto
  SceneSettings::GetBloomSettings() -> BloomSettings&
  {
    return mBloomSettings;
  }

  auto
  SceneSettings::GetShadowSettings() -> ShadowSettings&
  {
    return mShadowSettings;
  }
} // namespace Dwarf