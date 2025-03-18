#pragma once

#include <nlohmann/json.hpp>
#include <optional>
#include "ISceneSettings.h"
#include "Core/UUID.h"

namespace Dwarf
{
  /// @brief Interface for scene properties.
  class SceneSettings : public ISceneSettings
  {
  private:
    FogSettings         m_FogSettings;
    GlobalLightSettings m_GlobalLightSettings;
    std::optional<UUID> m_SkyboxMaterial;

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