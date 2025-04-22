#pragma once

#include "BloomSettings.hpp"
#include "Core/Rendering/TonemapTypes.hpp"
#include "ISceneSettings.hpp"
#include "SkyboxSettings.hpp"
#include <nlohmann/json.hpp>

namespace Dwarf
{
  /// @brief Interface for scene properties.
  class SceneSettings : public ISceneSettings
  {
  private:
    AmbientSettings      mAmbientLightSettings;
    AntiAliasingSettings mAntiAliasingSettings;
    BloomSettings        mBloomSettings;
    ExposureSettings     mExposureSettings;
    FogSettings          mFogSettings;
    ShadowSettings       mShadowSettings;
    SkyboxSettings       mSkyboxSettings;
    TonemapType          mTonemapType = TonemapType::Reinhard;

  public:
    SceneSettings() = default;
    explicit SceneSettings(nlohmann::json serializedSettings);
    ~SceneSettings() override = default;

    /// @copydoc ISerializable::Serialize
    auto
    Serialize() -> nlohmann::json override;

    /// @copydoc ISceneSettings::GetFogSettings
    auto
    GetFogSettings() -> FogSettings& override;

    /// @copydoc ISceneSettings::GetAmbientLightSettings
    auto
    GetAmbientSettings() -> AmbientSettings& override;

    /// @copydoc ISceneSettings::GetSkyboxMaterial
    auto
    GetSkyboxSettings() -> SkyboxSettings& override;

    auto
    GetExposureSettings() -> ExposureSettings& override;

    auto
    GetToneMapType() -> TonemapType& override;

    auto
    GetAntiAliasingSettings() -> AntiAliasingSettings& override;

    auto
    GetBloomSettings() -> BloomSettings& override;

    auto
    GetShadowSettings() -> ShadowSettings& override;
  };
} // namespace Dwarf