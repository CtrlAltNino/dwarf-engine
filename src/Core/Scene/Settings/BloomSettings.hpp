#pragma once

#include "Utilities/ISerializable.h"

namespace Dwarf
{
  struct BloomSettings : public ISerializable
  {
    bool  Enabled = true;
    float Threshold = 0.7F;
    float Intensity = 1.0F;
    float Radius = 1.0F;

    /// @brief Constructor.
    BloomSettings() = default;

    /// @brief Constructor.
    /// @param serializedSettings The serialized settings.
    BloomSettings(nlohmann::json json)
    {
      if (json.contains("Enabled"))
      {
        Enabled = json["Enabled"].get<bool>();
      }
      if (json.contains("Threshold"))
      {
        Threshold = json["Threshold"].get<float>();
      }
      if (json.contains("Intensity"))
      {
        Intensity = json["Intensity"].get<float>();
      }
      if (json.contains("Radius"))
      {
        Radius = json["Radius"].get<float>();
      }
    }

    /// @copydoc ISerializable::Serialize
    auto
    Serialize() -> nlohmann::json override
    {
      nlohmann::json json;
      json["Enabled"] = Enabled;
      json["Threshold"] = Threshold;
      json["Intensity"] = Intensity;
      json["Radius"] = Radius;
      return json;
    }
  };
}