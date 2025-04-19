#pragma once

#include "Core/Rendering/ExposureTypes.hpp"
#include "Utilities/ISerializable.h"

namespace Dwarf
{
  struct ExposureSettings : public ISerializable
  {
    ExposureType Type = ExposureType::Manual;
    float        Exposure = 1.5F;

    /// @brief Constructor.
    ExposureSettings() = default;

    /// @brief Constructor.
    /// @param serializedSettings The serialized settings.
    ExposureSettings(nlohmann::json json)
    {
      if (json.contains("Type"))
      {
        Type = json["Type"].get<ExposureType>();
      }
      if (json.contains("Exposure"))
      {
        Exposure = json["Exposure"].get<float>();
      }
    }

    /// @copydoc ISerializable::Serialize
    auto
    Serialize() -> nlohmann::json override
    {
      nlohmann::json json;
      json["Type"] = Type;
      json["Exposure"] = Exposure;
      return json;
    }
  };
}