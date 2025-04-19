#pragma once

#include "Core/Rendering/FogTypes.hpp"
#include "Utilities/ISerializable.h"
#include "Utilities/JsonHelper/JsonHelper.h"

namespace Dwarf
{
  /// @brief Struct that represents the fog settings of a scene.
  struct FogSettings : public ISerializable
  {
    /// @brief Type of the fog.
    FogType Type = FogType::Linear;
    /// @brief Color of the fog.
    glm::vec3 Color = { 0.3F, 0.3F, 0.3F };
    /// @brief Starting distance of the fog.
    float Start = 20.0F;
    /// @brief Ending distance of the fog (only for linear fog).
    float End = 50.0F;
    float Density = 3.0F;

    /// @brief Constructor.
    FogSettings() = default;

    /// @brief Constructor.
    /// @param serializedSettings The serialized settings.
    FogSettings(nlohmann::json serializedSettings)
    {
      if (serializedSettings.contains("Color"))
      {
        Color = serializedSettings["Color"].get<glm::vec3>();
      }
      if (serializedSettings.contains("Start"))
      {
        Start = serializedSettings["Start"].get<float>();
      }
      if (serializedSettings.contains("End"))
      {
        End = serializedSettings["End"].get<float>();
      }
      if (serializedSettings.contains("title"))
      {
        Type = serializedSettings["Type"].get<FogType>();
      }
    }

    /// @copydoc ISerializable::Serialize
    auto
    Serialize() -> nlohmann::json override
    {
      nlohmann::json json;
      json["Color"] = Color;
      json["Start"] = Start;
      json["End"] = End;
      json["Type"] = static_cast<int>(Type);
      return json;
    }
  };
}