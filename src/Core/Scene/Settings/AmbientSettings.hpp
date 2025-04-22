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
    float                    Intensity = 0.2F;
    std::array<glm::vec3, 3> Gradient = {
      { { 0.5f, 0.7f, 1.0f }, { 0.6f, 0.6f, 0.6f }, { 0.2f, 0.2f, 0.25f } }
    };
    bool                UseIBL = false;
    std::optional<UUID> CubeMap = std::nullopt;

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
      if (serializedSettings.contains("CubeMap") &&
          !serializedSettings.at("CubeMap").get<std::string>().empty())
      {
        CubeMap = UUID(serializedSettings.at("CubeMap").get<std::string>());
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
      json["CubeMap"] = CubeMap.has_value() ? CubeMap->Serialize() : "";
      return json;
    }
  };
}