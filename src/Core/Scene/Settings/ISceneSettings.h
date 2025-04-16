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
    glm::vec3 Color = { 0.3F, 0.3F, 0.3F };
    /// @brief Starting distance of the fog.
    float Start = 20.0F;
    /// @brief Ending distance of the fog (only for linear fog).
    float End = 50.0F;
    float Density = 3.0F;
    /// @brief Type of the fog.
    FogType Type = FogType::LINEAR;

    /// @brief Constructor.
    FogSettings() = default;

    /// @brief Constructor.
    /// @param serializedSettings The serialized settings.
    FogSettings(nlohmann::json serializedSettings)
      : Color(serializedSettings.contains("Color")
                ? glm::vec3(serializedSettings["Color"][0],
                            serializedSettings["Color"][1],
                            serializedSettings["Color"][2])
                : glm::vec3(0.3F, 0.3F, 0.3F))
      , Start(serializedSettings.contains("Start")
                ? serializedSettings["Start"].get<float>()
                : 20.0F)
      , End(serializedSettings.contains("End")
              ? serializedSettings["End"].get<float>()
              : 50.0F)
      , Type(serializedSettings.contains("Start")
               ? serializedSettings["Type"].get<FogType>()
               : FogType::LINEAR)
    {
      if (serializedSettings.contains("width"))
      {
        serializedSettings.at("width").get_to(width);
      }
      if (serializedSettings.contains("height"))
      {
        serializedSettings.at("height").get_to(height);
      }
      if (serializedSettings.contains("title"))
      {
        serializedSettings.at("title").get_to(title);
      }
    }

    /// @copydoc ISerializable::Serialize
    auto
    Serialize() -> nlohmann::json override
    {
      nlohmann::json json;
      json["Color"] = { Color.r, Color.g, Color.b };
      json["Start"] = Start;
      json["End"] = End;
      json["Type"] = static_cast<int>(Type);
      return json;
    }
  };

  /// @brief Struct that represents the ambient light settings of a scene.
  struct AmbientLightSettings : public ISerializable
  {
    /// @brief Color of the ambient light.
    glm::vec3 color = { 0.8F, 0.6F, 0.6F };
    /// @brief Intensity of the ambient light.
    float intensity = 0.2F;

    /// @brief Constructor.
    AmbientLightSettings() = default;

    /// @brief Constructor.
    /// @param serializedSettings The serialized settings.
    AmbientLightSettings(nlohmann::json serializedSettings)
      : color({ serializedSettings["LightColor"][0],
                serializedSettings["LightColor"][1],
                serializedSettings["LightColor"][2] })
      , intensity(serializedSettings["LightIntensity"])
    {
    }

    /// @copydoc ISerializable::Serialize
    auto
    Serialize() -> nlohmann::json override
    {
      nlohmann::json json;
      json["LightColor"] = { color.r, color.g, color.b };
      json["LightIntensity"] = intensity;
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

    /// @brief Returns the ambient light settings of the scene.
    /// @return The ambient light settings of the scene.
    virtual auto
    GetAmbientLightSettings() -> AmbientLightSettings& = 0;

    /// @brief Returns the UID of the skybox material.
    /// @return The UID of the skybox material.
    virtual auto
    GetSkyboxMaterial() -> std::optional<UUID>& = 0;
  };
} // namespace Dwarf