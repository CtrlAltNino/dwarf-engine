#pragma once

#include "Core/Rendering/AntiAliasingTypes.hpp"
#include "Utilities/ISerializable.h"

namespace Dwarf
{

  struct AntiAliasingSettings : public ISerializable
  {
    AntiAliasingMethod Type = AntiAliasingMethod::None;
    uint8_t            MsaaSamples = 1;

    /// @brief Constructor.
    AntiAliasingSettings() = default;

    /// @brief Constructor.
    /// @param serializedSettings The serialized settings.
    AntiAliasingSettings(nlohmann::json json)
    {
      if (json.contains("Type"))
      {
        Type = json["Type"].get<AntiAliasingMethod>();
      }
      if (json.contains("MsaaSamples"))
      {
        MsaaSamples = json["MsaaSamples"].get<uint8_t>();
      }
    }

    /// @copydoc ISerializable::Serialize
    auto
    Serialize() -> nlohmann::json override
    {
      nlohmann::json json;
      json["Type"] = Type;
      json["MsaaSamples"] = MsaaSamples;
      return json;
    }
  };
}