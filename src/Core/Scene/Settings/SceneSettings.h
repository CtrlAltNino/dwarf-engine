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
    FogSettings         mFogSettings;
    GlobalLightSettings mGlobalLightSettings;
    std::optional<UUID> mSkyboxMaterial;

  public:
    SceneSettings() = default;
    SceneSettings(nlohmann::json serializedSettings);

    /// @copydoc ISerializable::Serialize
    nlohmann::json
    Serialize() override;

    /// @copydoc ISceneSettings::GetFogSettings
    FogSettings&
    GetFogSettings() override;

    /// @copydoc ISceneSettings::GetGlobalLightSettings
    GlobalLightSettings&
    GetGlobalLightSettings() override;

    /// @copydoc ISceneSettings::GetSkyboxMaterial
    std::optional<UUID>&
    GetSkyboxMaterial() override;
  };
} // namespace Dwarf