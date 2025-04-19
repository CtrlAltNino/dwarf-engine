#pragma once

#include "AmbientSettings.hpp"
#include "AntiAliasingSettings.hpp"
#include "BloomSettings.hpp"
#include "Core/Rendering/AntiAliasingTypes.hpp"
#include "Core/Rendering/TonemapTypes.hpp"
#include "Core/UUID.hpp"
#include "ExposureSettings.hpp"
#include "FogSettings.hpp"
#include "ShadowSettings.hpp"
#include "SkyboxSettings.hpp"
#include "Utilities/ISerializable.hpp"
#include "Utilities/JsonHelper/JsonHelper.hpp"
#include <glm/glm.hpp>
#include <optional>

namespace Dwarf
{
  /// @brief Interface for scene settings.
  class ISceneSettings : public ISerializable
  {
  public:
    /// @brief Destructor.
    virtual ~ISceneSettings() = default;

    /// @brief Returns the fog settings of the scene.
    /// @return The fog settings of the scene.
    virtual auto
    GetFogSettings() -> FogSettings& = 0;

    /// @brief Returns the ambient light settings of the scene.
    /// @return The ambient light settings of the scene.
    virtual auto
    GetAmbientSettings() -> AmbientSettings& = 0;

    /// @brief Returns the UID of the skybox material.
    /// @return The UID of the skybox material.
    virtual auto
    GetSkyboxSettings() -> SkyboxSettings& = 0;

    virtual auto
    GetExposureSettings() -> ExposureSettings& = 0;

    virtual auto
    GetToneMapType() -> TonemapType& = 0;

    virtual auto
    GetAntiAliasingSettings() -> AntiAliasingSettings& = 0;

    virtual auto
    GetBloomSettings() -> BloomSettings& = 0;

    virtual auto
    GetShadowSettings() -> ShadowSettings& = 0;
  };
} // namespace Dwarf