#include "pch.hpp"

#include "SceneSettings.hpp"

namespace Dwarf
{
  SceneSettings::SceneSettings()
    : mExposureSettings(mObservers)
    , mAntiAliasingSettings(mObservers)
    , mBloomSettings(mObservers)
    , mSkyboxSettings(mObservers)
    , mAmbientLightSettings(mObservers)
    , mShadowSettings(mObservers)
    , mFogSettings(mObservers)
  {
  }

  SceneSettings::SceneSettings(nlohmann::json serializedSettings)
    : mExposureSettings(mObservers,
                        serializedSettings.contains("ExposureSettings")
                          ? serializedSettings["ExposureSettings"]
                          : nlohmann::json{})
    , mAntiAliasingSettings(mObservers,
                            serializedSettings.contains("AntiAliasingSettings")
                              ? serializedSettings["AntiAliasingSettings"]
                              : nlohmann::json{})
    , mBloomSettings(mObservers,
                     serializedSettings.contains("BloomSettings")
                       ? serializedSettings["BloomSettings"]
                       : nlohmann::json{})
    , mSkyboxSettings(mObservers,
                      serializedSettings.contains("SkyboxSettings")
                        ? serializedSettings["SkyboxSettings"]
                        : nlohmann::json{})
    , mAmbientLightSettings(mObservers,
                            serializedSettings.contains("AmbientLightSettings")
                              ? serializedSettings["AmbientLightSettings"]
                              : nlohmann::json{})
    , mShadowSettings(mObservers,
                      serializedSettings.contains("ShadowSettings")
                        ? serializedSettings["ShadowSettings"]
                        : nlohmann::json{})
    , mFogSettings(mObservers,
                   serializedSettings.contains("FogSettings")
                     ? serializedSettings["FogSettings"]
                     : nlohmann::json{})
  {
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

  void
  SceneSettings::RegisterSceneSettingsObserver(ISceneSettingsObserver* observer)
  {
    mObservers.push_back(observer);
  }

  void
  SceneSettings::UnregisterSceneSettingsObserver(
    ISceneSettingsObserver* observer)
  {
    std::erase(mObservers, observer);
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
  SceneSettings::GetToneMapType() -> TonemapType
  {
    return mTonemapType;
  }

  void
  SceneSettings::SetToneMapType(TonemapType type)
  {
    mTonemapType = type;

    for (auto* observer : mObservers)
    {
      if (observer != nullptr)
      {
        observer->OnTonemapChanged();
      }
    }
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