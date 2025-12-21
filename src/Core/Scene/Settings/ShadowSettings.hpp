#pragma once

#include "Core/Rendering/ShadowMappingTypes.hpp"
#include "Core/Scene/Settings/ISceneSettingsObserver.hpp"
#include "Utilities/ISerializable.hpp"
#include <cstdint>

namespace Dwarf
{
  struct ShadowSettings : public ISerializable
  {
  private:
    std::reference_wrapper<std::vector<ISceneSettingsObserver*>> Observers;
    bool                                                         Enabled = true;
    uint8_t             CascadeCount = 4;
    float               Distance = 50.0F;
    float               SplitLambda = 0.0F;
    ShadowMapResolution Resolution = ShadowMapResolution::Medium;
    float               DepthBias = 0.001F;
    float               SlopeScaledBias = 1.0F;
    uint8_t             PcfSamples = 1;

  public:
    /// @brief Constructor.
    /// @param serializedSettings The serialized settings.
    ShadowSettings(std::vector<ISceneSettingsObserver*>& observers,
                   nlohmann::json                        json = "")
      : Observers(observers)
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

    void
    NotifyObservers()
    {
      for (auto* observer : Observers.get())
      {
        observer->OnShadowSettingsChanged();
      }
    }

    [[nodiscard]] auto
    GetEnabled() const -> bool
    {
      return Enabled;
    }

    void
    SetEnabled(bool enabled)
    {
      Enabled = enabled;
      NotifyObservers();
    }

    [[nodiscard]] auto
    GetCascadeCount() const -> uint8_t
    {
      return CascadeCount;
    }

    void
    SetCascadeCount(uint8_t cascadeCount)
    {
      CascadeCount = cascadeCount;
      NotifyObservers();
    }

    [[nodiscard]] auto
    GetDistance() const -> float
    {
      return Distance;
    }

    void
    SetDistance(float distance)
    {
      Distance = distance;
      NotifyObservers();
    }

    [[nodiscard]] auto
    GetSplitLambda() const -> float
    {
      return SplitLambda;
    }

    void
    SetSplitLambda(float splitLambda)
    {
      SplitLambda = splitLambda;
      NotifyObservers();
    }

    [[nodiscard]] auto
    GetResolution() const -> ShadowMapResolution
    {
      return Resolution;
    }

    void
    SetResolution(ShadowMapResolution resolution)
    {
      Resolution = resolution;
      NotifyObservers();
    }

    [[nodiscard]] auto
    GetDepthBias() const -> float
    {
      return DepthBias;
    }

    void
    SetDepthBias(float depthBias)
    {
      DepthBias = depthBias;
      NotifyObservers();
    }

    [[nodiscard]] auto
    GetSlopeScaledBias() const -> float
    {
      return SlopeScaledBias;
    }

    void
    SetSlopeScaledBias(float slopeScaledBias)
    {
      SlopeScaledBias = slopeScaledBias;
      NotifyObservers();
    }

    [[nodiscard]] auto
    GetPcfSamples() const -> uint8_t
    {
      return PcfSamples;
    }

    void
    SetPcfSamples(uint8_t pcfSamples)
    {
      PcfSamples = pcfSamples;
      NotifyObservers();
    }
  };
}