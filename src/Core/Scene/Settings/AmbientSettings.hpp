#pragma once

#include "Core/Rendering/AmbientTypes.hpp"
#include "Core/UUID.hpp"
#include "Utilities/ISerializable.hpp"
#include "Utilities/JsonHelper/JsonHelper.hpp"

namespace Dwarf
{
  /// @brief Struct that represents the ambient light settings of a scene.
  struct AmbientSettings : public ISerializable
  {
    AmbientSource Type = AmbientSource::Color;
    /// @brief Color of the ambient light.
    glm::vec3 Color = { 0.2F, 0.2F, 0.2F };
    /// @brief Intensity of the ambient light.
    float                    Intensity = 1.0F;
    std::array<glm::vec3, 3> Gradient = {
      { { 0.5F, 0.7F, 1.0F }, { 0.6F, 0.6F, 0.6F }, { 0.2F, 0.2F, 0.25F } }
    };
    bool                UseIBL = false;
    std::optional<UUID> Cubemap = std::nullopt;

    /// @brief Constructor.
    AmbientSettings() = default;

    /// @brief Constructor.
    /// @param serializedSettings The serialized settings.
    AmbientSettings(nlohmann::json serializedSettings)
    {
      if (serializedSettings.contains("Type"))
      {
        Type = serializedSettings["Type"].get<AmbientSource>();
      }
      if (serializedSettings.contains("Color"))
      {
        Color = serializedSettings["Color"].get<glm::vec3>();
      }
      if (serializedSettings.contains("Intensity"))
      {
        Intensity = serializedSettings["Intensity"].get<float>();
      }
      if (serializedSettings.contains("Gradient"))
      {
        Gradient =
          serializedSettings["Gradient"].get<std::array<glm::vec3, 3>>();
      }
      if (serializedSettings.contains("UseIBL"))
      {
        UseIBL = serializedSettings["UseIBL"].get<bool>();
      }
      if (serializedSettings.contains("Cubemap") &&
          !serializedSettings.at("Cubemap").get<std::string>().empty())
      {
        Cubemap = UUID(serializedSettings.at("Cubemap").get<std::string>());
      }
    }

    /// @copydoc ISerializable::Serialize
    auto
    Serialize() -> nlohmann::json override
    {
      nlohmann::json json;
      json["Type"] = Type;
      json["Color"] = Color;
      json["LightIntensity"] = Intensity;
      json["Gradient"] = Gradient;
      json["UseIBL"] = UseIBL;
      json["Cubemap"] = Cubemap.has_value() ? Cubemap->Serialize() : "";
      return json;
    }
  };
}