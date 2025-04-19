#pragma once

#include "Core/Rendering/ShadowMappingTypes.hpp"
#include "Utilities/ISerializable.h"

namespace Dwarf
{
  struct ShadowSettings : public ISerializable
  {
    bool                Enabled = true;
    uint8_t             CascadeCount = 4;
    float               Distance = 50.0F;
    float               SplitLambda = 0.0F;
    ShadowMapResolution Resolution = ShadowMapResolution::Medium;
    float               DepthBias = 0.001F;
    float               SlopeScaledBias = 1.0F;
    uint8_t             PcfSamples = 1;

    /// @brief Constructor.
    ShadowSettings() = default;

    /// @brief Constructor.
    /// @param serializedSettings The serialized settings.
    ShadowSettings(nlohmann::json json)
    {
      if (json.contains("Enabled"))
      {
        Enabled = json["Enabled"].get<bool>();
      }
      if (json.contains("CascadeCount"))
      {
        CascadeCount = json["CascadeCount"].get<uint8_t>();
      }
      if (json.contains("Distance"))
      {
        Distance = json["Distance"].get<float>();
      }
      if (json.contains("SplitLambda"))
      {
        SplitLambda = json["SplitLambda"].get<float>();
      }
      if (json.contains("Resolution"))
      {
        Resolution = json["Resolution"].get<ShadowMapResolution>();
      }
      if (json.contains("DepthBias"))
      {
        DepthBias = json["DepthBias"].get<float>();
      }
      if (json.contains("SlopeScaledBias"))
      {
        SlopeScaledBias = json["SlopeScaledBias"].get<float>();
      }
      if (json.contains("PcfSamples"))
      {
        PcfSamples = json["PcfSamples"].get<uint8_t>();
      }
    }

    /// @copydoc ISerializable::Serialize
    auto
    Serialize() -> nlohmann::json override
    {
      nlohmann::json json;
      json["Enabled"] = Enabled;
      json["CascadeCount"] = CascadeCount;
      json["Distance"] = Distance;
      json["SplitLambda"] = SplitLambda;
      json["Resolution"] = Resolution;
      json["DepthBias"] = DepthBias;
      json["SlopeScaledBias"] = SlopeScaledBias;
      json["PcfSamples"] = PcfSamples;
      return json;
    }
  };
}