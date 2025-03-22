#pragma once

#include "Core/UUID.h"
#include "Utilities/ISerializable.h"
#include <glm/glm.hpp>
#include <optional>

namespace Dwarf
{
  /// @brief Enum that represents the type of fog.
  enum class FogType
  {
    /// @brief Linear fog.
    LINEAR,
    /// @brief Exponential fog.
    EXPONENTIAL
  };

  /// @brief Struct that represents the fog settings of a scene.
  struct FogSettings : public ISerializable
  {
    /// @brief Color of the fog.
    glm::vec3 fogColor = { 0.3F, 0.3F, 0.3F };
    /// @brief Starting distance of the fog.
    float fogStart = 20.0F;
    /// @brief Ending distance of the fog (only for linear fog).
    float fogEnd = 50.0F;
    /// @brief Type of the fog.
    FogType type = FogType::LINEAR;

    /// @brief Constructor.
    FogSettings() = default;

    /// @brief Constructor.
    /// @param serializedSettings The serialized settings.
    FogSettings(nlohmann::json serializedSettings)
      : fogColor({ serializedSettings["FogColor"][0],
                   serializedSettings["FogColor"][1],
                   serializedSettings["FogColor"][2] })
      , fogStart(serializedSettings["FogStart"])
      , fogEnd(serializedSettings["FogEnd"])
      , type(static_cast<FogType>(serializedSettings["FogType"]))
    {
    }

    /// @copydoc ISerializable::Serialize
    auto
    Serialize() -> nlohmann::json override
    {
      nlohmann::json json;
      json["FogColor"] = { fogColor.r, fogColor.g, fogColor.b };
      json["FogStart"] = fogStart;
      json["FogEnd"] = fogEnd;
      json["FogType"] = static_cast<int>(type);
      return json;
    }
  };

  /// @brief Struct that represents the global light settings of a scene.
  struct GlobalLightSettings : public ISerializable
  {
    /// @brief Color of the global light.
    glm::vec3 color = { 0.8F, 0.6F, 0.6F };
    /// @brief Intensity of the global light.
    float intensity = 0.2F;

    /// @brief Constructor.
    GlobalLightSettings() = default;

    /// @brief Constructor.
    /// @param serializedSettings The serialized settings.
    GlobalLightSettings(nlohmann::json serializedSettings)
      : color({ serializedSettings["GlobalLightColor"][0],
                serializedSettings["GlobalLightColor"][1],
                serializedSettings["GlobalLightColor"][2] })
      , intensity(serializedSettings["GlobalLightIntensity"])
    {
    }

    /// @copydoc ISerializable::Serialize
    auto
    Serialize() -> nlohmann::json override
    {
      nlohmann::json json;
      json["GlobalLightColor"] = { color.r, color.g, color.b };
      json["GlobalLightIntensity"] = intensity;
      return json;
    }
  };

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

    /// @brief Returns the global light settings of the scene.
    /// @return The global light settings of the scene.
    virtual auto
    GetGlobalLightSettings() -> GlobalLightSettings& = 0;

    /// @brief Returns the UID of the skybox material.
    /// @return The UID of the skybox material.
    virtual auto
    GetSkyboxMaterial() -> std::optional<UUID>& = 0;
  };
} // namespace Dwarf