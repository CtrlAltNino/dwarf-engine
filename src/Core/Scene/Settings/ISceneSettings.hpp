#pragma once

#include "AmbientSettings.hpp"
#include "AntiAliasingSettings.hpp"
#include "BloomSettings.hpp"
#include "Core/Rendering/TonemapTypes.hpp"
#include "ExposureSettings.hpp"
#include "FogSettings.hpp"
#include "ISceneSettingsObserver.hpp"
#include "ShadowSettings.hpp"
#include "SkyboxSettings.hpp"
#include "Utilities/ISerializable.hpp"
#include "Utilities/JsonHelper/JsonHelper.hpp"
#include <glm/glm.hpp>

namespace Dwarf
{
  /// @brief Interface for scene settings.
  class ISceneSettings : public ISerializable
  {
  public:
    /// @brief Destructor.
    virtual ~ISceneSettings() = default;

    virtual void
    RegisterSceneSettingsObserver(ISceneSettingsObserver* observer) = 0;

    virtual void
    UnregisterSceneSettingsObserver(ISceneSettingsObserver* observer) = 0;

    /// @brief Returns the fog settings of the scene.
    /// @return The fog settings of the scene.
    [[nodiscard]] virtual auto
    GetFogSettings() -> FogSettings& = 0;

    /// @brief Returns the ambient light settings of the scene.
    /// @return The ambient light settings of the scene.
    [[nodiscard]] virtual auto
    GetAmbientSettings() -> AmbientSettings& = 0;

    /// @brief Returns the UID of the skybox material.
    /// @return The UID of the skybox material.
    [[nodiscard]] virtual auto
    GetSkyboxSettings() -> SkyboxSettings& = 0;

    [[nodiscard]] virtual auto
    GetExposureSettings() -> ExposureSettings& = 0;

    [[nodiscard]] virtual auto
    GetToneMapType() -> TonemapType = 0;

    virtual void
    SetToneMapType(TonemapType type) = 0;

    [[nodiscard]] virtual auto
    GetAntiAliasingSettings() -> AntiAliasingSettings& = 0;

    [[nodiscard]] virtual auto
    GetBloomSettings() -> BloomSettings& = 0;

    [[nodiscard]] virtual auto
    GetShadowSettings() -> ShadowSettings& = 0;
  };
} // namespace Dwarf