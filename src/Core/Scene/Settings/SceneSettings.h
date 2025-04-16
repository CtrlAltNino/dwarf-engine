#pragma once

#include "Core/UUID.h"
#include "ISceneSettings.h"
#include <nlohmann/json.hpp>
#include <optional>

namespace Dwarf
{
  /// @brief Interface for scene properties.
  class SceneSettings : public ISceneSettings
  {
  private:
    FogSettings          mFogSettings;
    AmbientLightSettings mAmbientLightSettings;
    std::optional<UUID>  mSkyboxMaterial;

  public:
    SceneSettings() = default;
    SceneSettings(nlohmann::json serializedSettings);

    /// @copydoc ISerializable::Serialize
    auto
    Serialize() -> nlohmann::json override;

    /// @copydoc ISceneSettings::GetFogSettings
    auto
    GetFogSettings() -> FogSettings& override;

    /// @copydoc ISceneSettings::GetAmbientLightSettings
    auto
    GetAmbientLightSettings() -> AmbientLightSettings& override;

    /// @copydoc ISceneSettings::GetSkyboxMaterial
    auto
    GetSkyboxMaterial() -> std::optional<UUID>& override;
  };
} // namespace Dwarf